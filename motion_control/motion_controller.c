/**
 * FILENAME :           motion_controller.c
 * DESCRIPTION :        PWM Generation for DESN2000 ELEC W11A Group 3 
 * 
 * NOTES :
 * 
 * AUTHOR :             Barry Feng   
 * 
 * START DATE :         20 Jun 20
 * 
 * CHANGES :
 * -- 20/06/2020 --     File created.
 */

#include <motion_controller.h>

extern LightRail light_rail;

/**
 * This function samples the ADC0 output and reads the voltage feedback
 * from the motor. The ADC is started using AD0CR and the result is obtained
 * using a bitshift and bitmask.
 * 
 * The ADC value is then converted to an equivalent voltage using a magic
 * number multiplication (to reduce latency caused by integer division). The
 * equivalent voltage is given by the equation:
 * 
 * VOLTAGE = (result * 3.3 / 1023 << 16)
 * 
 * Note that the 16 bit left shift is to convert the voltage to a Q22.0 number.
 */
static uint32_t get_voltage(void) {

    static uint16_t result;

    AD0CR = AD0CR | (1 << 24);
    while (!(AD0DR1 & 0x80000000));
    result = AD0DR1;
    result = (result >> 6);
    result = (result & 0x000003FF);

    return ((0x34DA * result) >> (ADC_SHIFT - VEL_SHIFT));
}

/**
 * This function samples the output of three standard HC-SR04 ultrasonic 
 * sensors and uses this as an input to the assistive braking capability.
 * 
 * PINOUT --
 * TRIGGER PINS -- All three HC-SR04 can be connected to P3.0 GPIO.
 * ECHO PINS    -- #1 HC-SR04 connected to P3.1 GPIO.
 *              -- #2 HC-SR04 connected to P3.2 GPIO.
 *              -- #3 HC-SR04 connected to P3.3 GPIO.
 * 
 * RETURNS --
 *  The function will return an adjusted_distance in Q22.0 notation, for
 *  ease of calculation, the following scaling factor has been applied.
 *      DISTANCE = (RETURN >> 16 * 0.034/2) (0.34 for speed of sound in air).
 */
static uint32_t get_ultrasonic_data(void) {
    uint32_t duration1 = 0, duration2 = 0, duration3 = 0;

    FIO3PIN &= ~(ULTRAS_ALL);
    delay_timer2(ULTRAS_SETUP_TIME);

    FIO3PIN |= 0x1 << ULTRAS_TRIGGER;
    delay_timer2(ULTRAS_TRIG_TIME);
    FIO3PIN &= ~(0x1 << ULTRAS_TRIGGER);

    start_timer2_stopwatch();
    while (
        ((FIO3PIN >> ULTRAS_1_ECHO) & 1) || 
        ((FIO3PIN >> ULTRAS_2_ECHO) & 1) ||
        ((FIO3PIN >> ULTRAS_1_ECHO) & 1)) {
        if (!((FIO3PIN >> ULTRAS_1_ECHO) & 1)) {
            duration1 = split_timer2_stopwatch();
        } else if (!((FIO3PIN >> ULTRAS_2_ECHO) & 1)) {
            duration2 = split_timer2_stopwatch();
        } else if (!((FIO3PIN >> ULTRAS_3_ECHO) & 1)) {
            duration3 = split_timer2_stopwatch();
        }
    }

    reset_timer2_stopwatch();

    return ((duration1 + duration2 + duration3) * DIVIDE_3) << Q_SHIFT;
}

/**
 * This function sets a new PWM duty cycle. The PWM0 match register is 
 * set to the argument duty_cycle and PWM0 is latched to the new duty cycle.
 */
static void set_pwm(int duty_cycle) {
    PWM0MR1 = duty_cycle;
    PWM0LER = (1 << 1);
}

/**
 * This function runs a single iteration or step of the light rail controller.
 * The controller structure, compensation values and dms state are statically
 * defined and will only be initialised on the first controller iteration.
 * 
 * The function will also ensure that the controller is initialised once by
 * checked the initialised member of the pi_controller structure.
 * 
 * The DMS and brake states are then checked and updated accordingly. Next, 
 * the the PI controller is stepped and the controller output is passed to
 * the set_pwm function as feedback.
 * 
 * The function ensures that power is only applied to the motors if the brakes
 * are disabled.
 */
void run_controller(void) {
    static Controller pi_controller;
    static uint32_t compensation = 0;
    static uint8_t dms_state = 0, mem_dms_state = 0;

    if (!pi_controller.initialised) {
        pi_controller = init_controller(kP, kI);
    }

    if (get_ultrasonic_data() <= MIN_OBSTACLE_DIST) {
        set_brake();
    }

    update_dms_state(dms_state, mem_dms_state);
    update_brake_state();

    compensation = step_controller(
        light_rail.vel_setpoint,
        light_rail.velocity = get_vel(),
        pi_controller);

    if (light_rail.vel_setpoint != 0) {
        release_brake();
    }

    if (!(light_rail.brake_state)) {
        set_pwm(abs(compensation));
    }
}

/**
 * This function updates the light rail's DMS state by reading from GPIO
 * pin P0.10 on the FVGA daughter board. The function uses edge-triggering
 * to detect changes in the DMS. If a LOW->HIGH change is detected, the
 * DMS system is reset (control is given to the driver). If a HIGH->LOW
 * change is detected, the DMS system is set (control is taken from the
 * driver and brakes are set).
 */
static void update_dms_state(uint8_t dms_state, uint8_t mem_dms_state) {
    // Read from DMS P0.10
    dms_state = ((FIO0PIN >> 10) & 0x1);

    if (dms_state != mem_dms_state) {
        if (dms_state == 1) {
            reset_dms();
        } else {
            set_dms();
        }
        mem_dms_state = dms_state;
    }
}

/**
 * This function updates the light rail's brakes based on the light_rail
 * struct's brake_state value.
 */
static void update_brake_state(void) {
    if (light_rail.brake_state) {
        set_brake();
		return;
    }
    release_brake();
}

/**
 * This function actiates the light rail's brakes by driving the P1.0 GPIO
 * pin HIGH. The light rail's velocity setpoint is also set to 0. 
 */
static void set_brake(void) {
    light_rail.vel_setpoint = 0;
    light_rail.brake_state = 1;
    spi_write(BRAKE_ACTIVE);
}

/**
 * This function releases the light rail's brakes by driving the P1.0 GPIO
 * pin LOW.
 */
static void release_brake(void) {
    light_rail.brake_state = 0;
    spi_write(BRAKE_INACTIVE);
}

/**
 * This function sets the DMS system. If called, the brakes are activated
 * and the light rail's velocity setpoint is set to 0.
 */
static void set_dms(void) {
    set_brake();
    light_rail.dms_state = 0x1;
}

/**
 * This function resets the DMS system. If called, the brakes are released
 * and normal light rail operation can continue.
 */
static void reset_dms(void) {
    release_brake();
    light_rail.dms_state = 0x0;
}

/**
 * This function returns a binary fixed-point velocity which is Q22.0.
 * The returned result is also right shifted by 16 bits from the true velocity
 * to maintain resolution. The velocity in meters per second is obtained by 
 * adjusting the voltage by a scaling factor of (100/(2.5 * 3.6) = 11.11). This
 * is approximated to 11 to return an integral result.
 */ 
static uint32_t get_vel(void) {
    uint32_t voltage = get_voltage();
    uint32_t vel_ms = voltage * 11;

    return vel_ms;
}
