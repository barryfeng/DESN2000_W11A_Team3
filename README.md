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



