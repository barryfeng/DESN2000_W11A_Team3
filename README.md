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
* placeholder

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



