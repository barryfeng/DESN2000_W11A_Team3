; This file is used to carry out the required logging operations
; while in supervisor mode. Data is stored and retrieved from static
; memory bank 2. A software interrupt is initiated with a
; corresponding flag in the data management assmebly file. A jump table
; is used to go to the correct label/function based on what the SWI
; flag is.

; There are 5 different SWI logging operations:
; - store data
; - view velocity
; - view velocity set point (target velocity)
; - view brake state (on/off)
; - view DMS (dead man switch) state (on/off)

; NOTE: registers R0-R3 contain the data to be stored (only when storing
; data). These registers will thus not be used in this program except
; for when data needs to be returned.
                
                AREA HANDLER, CODE
				PRESERVE8
				
				EXPORT SWI_Handler

STA_MEM2_BASE   EQU 0x82000000                  ; Address of static memory bank 2
SWI_NUM	  		EQU 5                           ; There are 5 different SWI operations

VEL_BYTES       EQU 20                          ; Number of bytes needed to search backwards for velocity           
VEL_SET_BYTES   EQU 12                          ; Number of bytes needed to search backwards for velocity set point
BRAKE_BYTES     EQU 4                           ; Number of bytes needed to search backwards for brake state
DMS_BYTES       EQU 2                           ; Number of bytes needed to search backwards for DMS state

SWI_Handler
                LDR R5, =STA_MEM2_BASE          ; Load adress of static memory bank 2
                LDR R6, [R5]                    ; Load current data in static memory bank 2
                
                LDR IP, [LR, #-4]               ; Load SWI flag into IP
				BIC IP, IP, #0XFF000000         ; Clear top 8 bits of IP

				CMP IP, #SWI_NUM                ; Check function index is in range
				LDRLO PC, [PC, IP, LSL #2]      ; Branch to correct handler

				B SWI_OUT_OF_RANGE

SWI_JUMP_TABLE                                  ; Jump table with each of the logging operations
				DCD SWI_store
				DCD SWI_view_vel
                DCD SWI_view_vel_setpoint
                DCD SWI_view_brake
                DCD SWI_view_dms

; Store the 4 pieces of data (R0-R3)
; into memory.

SWI_store       STMFD R6!, {R0-R3}              ; Store data into memory and update base register after data transfer
				
				B SWI_EXIT

; Access the value of the velocity
; and return the 32 bit result.

SWI_view_vel    MOV R7, #VEL_BYTES
                SUB R8, R5, R7                  ; Go back VEL_BYTES in memory to get to the location of velocity 
                LDR R7, [R8]                    ; Load word (velocity)
                MOV R0, R7                      ; Return velocity      
				
				B SWI_EXIT

; Access the value of the velocity set
; point and return the 32 bit result.

SWI_view_vel_setpoint
                MOV R7, #VEL_SET_BYTES
                SUB R8, R5, R7                  ; Go back VEL_SET_BYTES in memory to get to the location of velocity set point
                LDR R7, [R8]                    ; Load word (velocity set point)
                MOV R0, R7                      ; Return velocity set point      
				
				B SWI_EXIT

; Access the value of the brake state
; and return the 8 bit result.

SWI_view_brake  MOV R7, #BRAKE_BYTES
                SUB R8, R5, R7                  ; Go back BRAKE_BYTES in memory to get to the location of brake state 
                LDRB R7, [R8]                   ; Load byte (brake state)
                MOV R0, R7                      ; Return brake state      
				
				B SWI_EXIT

; Access the value of the DMS state
; and return the 8 bit result.

SWI_view_dms    MOV R7, #DMS_BYTES
                SUB R8, R5, R7                  ; Go back DMS_BYTES in memory to get to location of DMS state 
                LDRB R7, [R8]                   ; Load byte (DMS state)
                MOV R0, R7                      ; Return DMS state      
				
				B SWI_EXIT

; Branch here if an unknown SWI flag
; is used with the SWI command in the
; data management file.

SWI_OUT_OF_RANGE
				MOV R0, #-1					    ; Throw error

SWI_EXIT
                MOVS PC, LR						; Exit supervisor mode

				END
