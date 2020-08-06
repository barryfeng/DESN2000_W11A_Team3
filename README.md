# DESN2000_W11A_Team3
DESN2000_W11A_Team3 Light Rail Controller

## File Association
### System Initialisation (./system)
* system_init.c
* system_init.h
* system_timer.c
* system_timer.h

### Diagnostic Code Loading/Execution (./diagnostics)
* diagnostics.c
* diagnostics.h
* SPI_MMC.c
* SPI_MMC.h

### Data Logging/API Design (./logging)
* access_data.c
* access_data.h
* data_management.s
* exception_vector_table.s
* swi_handler.s

### Motion Controller (./motion_control)
#### Features included: Dead Man's Switch, Determining Tram Speed, DC Motor Control
* motion_controller.c
* motion_controller.h
* fixed_point_pid.c
* fixed_point_pid.h
* speed_limit.c
* speed_limit.h

### User Interface
* delay.c
* delay.h
#### LCD
* lcd_main.c
* lcd_main.h
* lcd_hw.c
* lcd_hw.h
* lcd_grph.c
* lcd_grph.h
* lcd_cfg.h
* sdram.c
* sdram.h
* font5x7.c
* font5x7.h
* fontVel.c
* fontVel.h
* fontVelLabel.c
* fontVelLabel.h
* fontBrakeLabel.c
* fontBrakeLabel.h
* font_macro.h
#### Touchscreen
* touch.c
* touch.h
* touch_screen_read.c
* touch_screen_read.h
* button_press.c
* button_press.h

## System Architecture
### Motion Controller
#### Determining current speed using ADC
* The feedback voltage from the DC motor is obtained using the LPC2478 ADC. This voltage is then manipulated using a scaling factor of: 𝑣_𝑚𝑠 = 𝑉_𝐴𝐷𝐶 (100/(2.5⋅3.6)) ≪ 16 to obtain velocity (m/s) in Q22 notation.
* The current light-rail speed is then fed into fixed-point proportional integral controller which updates the PWM duty cycle to compensate for error. The velocity in the light rail's central struct is also updated.

#### Setpoint Verification
* The velocity setpoint entered by the driver is compared to the maximum permitted velocity at the current location (get_vel_limit_data()). If the setpoint exceed this speed limit, the setpoint is set to the speed limit.
* The obstacle detection code then checks whether an object is within 4m of the
HC-SR04 sensors interfaced via GPIO. Three sensors are used and their readings
are averaged for redundancy. If an object is detected, the brakes are applied via SPI.
* The DMS state is also checked here. If the DMS has been released, brakes are
applied via SPI.
* If the setpoint is validated, the setpoint is passed to the PI controller.

#### Proportional-Integral Controller
* A standard integration of a proportional integral controller is used. The error is obtained by taking the difference between the current velocity and velocity setpoint of the light rail. The proportional and integral terms are evaluated and summed to form the controller output.
* All arithmetic inside the PI controller is done using fixed point arithmetic to
reduce overhead for the ARM CPU.
* The controller output is then checked for saturation and overflow. If saturation or overflow (for uint_16t) occurs, the output is capped at the 16-bit out_max value specified in controller initialisation.

### Diagnostic Code Loading/Execution (./diagnostics)
* The diagnostic code is loaded by copying data from an inserted SD card into
the SDRAM.
* Interfacing with the SD card is performed over the SPI and MMC interfaces.
* The code loading process uses a set of stardard APIs defined by NXP in the
SPI_MMC file. 
* When started, the SPI_MMC APIs are called and data is read in continuous 512B blocks and stored in MMCRData arrays. These arrays are copied to the diagnostic_code array before being cleared by the subsequent write cycle.
* This loaded diagnostic code is then run using a function pointer using
run_diag_code() in diagnostics.c.
* A series of checks are also performed to ensure that the diagnostic process
cannot be entered when the light rail is in operation (velocity > 0, brakes disengaged).

### User Interface

#### LCD
##### LCD Startup
* External SDRAM is setup for frame buffer.
* LCD controller is setup for specific LCD configuration in lcd_config.h and then LCD is turned on.
* LCD is filled with a 16-bit depth RGB colour (CUSTOM_1). Borders are placed around all LCD features. Labels are placed above most LCD features (excluding velocity and brake).  
##### LCD Run
* LCD mode is checked; standby mode if velocity = 0 AND brake is applied, else, active mode. In standby mode, only throttle (+) and SD controls remain with colour backlighting. Once the operator interacts with throttle (+), increasing the velocity setpoint to above 0 and thus releasing the brake, the LCD will enter active mode. In active mode, most features (excluding dead man's switch (DMS) and max velocity (MAX V)) will have colour backlighting.
* Velocity is extracted from the light-rail struct in Q22 notation. It is right shifted 16 bits to obtain its value in m/s and is multiplied by 3.6 to convert this value to km/h. This integer casted value is then converted into a string and printed on the LCD.
* Dead man's switch (DMS) state is checked, if it is ON, the DMS indicator on the LCD will enter an ON state (YELLOW), otherwise, it will remain in an OFF state (LIGHT_GREY). Maximum velocity (MAX V) state is checked, if the current state velocity = 50 km/h, the MAX V indicator on the LCD will enter an ON state (YELLOW), otherwise, will remain in an OFF state (LIGHT_GREY).
* The current and next stop location is displayed on the LCD. The list of stops iterates once the brake is applied and reverses when the end of the list is reached.
* System interupt handler will loop through the above process every 20 ms.  

#### Touchscreen    
##### Press Button
* Takes in x and y value coordinates from LCD screen and scales them to the correct screen size based on the given LCD parameters.
* Runs through a list of if statements to determine which button has been pressed.
##### 'inside_x' Functions
* Takes in scaled coordinates and determines whether they align to the button listed in their name.
* i.e. inside_brake calculates whether the coordinates are within a radius of the circular brake button.
##### Touchscreen Starup (touch_init)
* GPIO and SPI hardware interfaces set up for touchscreen reading operations.
##### Touchscreen Variable Read (touch_read_xy)
* Takes in pointers for variables X, Y, Z1 and Z2 relating to position and pressure calculation variables, and sets them based on touch_read(0xXX) output, where:
    * X coordinate = 0xD8
    * Y coordinate = 0x98
    * Z1 coordinate = 0xB8
    * Z2 coordinate = 0xC8
##### Touchscreen Controller Read (touch_read)
* Takes in a command (i.e. hex number) and returns the value based on the return of the S0SPDR value due to the command.
##### Touchscreen Read Checking (touch_screen_press)
* Initialises variables X, Y, Z1, Z2, Pressure, Threshold, with zeroes, except for Threshold = 100 for the minimum press pressure for a touch to be executed.
* Calls touch_read_xy to populate the positioning variables and pressure_calc to populate the pressure variable.
* Executes a button press if the calculated pressure is above the minimum press threshold (100).
##### Pressure Calculator
* Takes the given positioning variables from the touchscreen peripheral and a pointer to pressure to update with a value determined by a given algorithm from the touchscreen's data sheet.

# Version History
## v1.0.0a1
* PID Controller code partially (mostly) implemented.
* PLL clock code implemented.
* Custom coded logging program.
* LPC24XX pin references added.

## v2.0.0a1
* Fixed point PI controller fully implemented.
* SD card loading and diagnostic code execution implemented.
* Data logging and retrieval API implemented.
* LCD and touchscreen operation implemented.



