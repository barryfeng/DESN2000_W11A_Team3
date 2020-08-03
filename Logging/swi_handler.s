				AREA	IRQ, CODE
				PRESERVE8
				
				EXPORT SWI_Handler

STA_MEM2_BASE   EQU 0x82000000
SWI_NUM	  		EQU 5

VEL_BYTES       EQU 20
VEL_SET_BYTES   EQU 12
BRAKE_BYTES     EQU 4
DMS_BYTES       EQU 2

SWI_Handler
                LDR R5, =STA_MEM2_BASE                          ; static memory bank 2
                LDR R6, [R5]
                
                LDR IP, [LR, #-4]								; Load SWI to IP
				BIC IP, IP, #0XFF000000							; Clear top 8 bits of IP

				CMP IP, #SWI_NUM								; Check function index is in range
				LDRLO PC, [PC, IP, LSL #2]						; Branch to correct handler

				B SWI_OUT_OF_RANGE

SWI_JUMP_TABLE													; jump table
				DCD SWI_store
				DCD SWI_view_vel
                DCD SWI_view_vel_setpoint
                DCD SWI_view_brake
                DCD SWI_view_dms

; NOTE: registers R0-R3 contain the data to be stored

SWI_store       STMFD R6!, {R0-R3}                              ; load data into memory and update base register after transfer
				
				B SWI_EXIT

SWI_view_vel    MOV R7, #VEL_BYTES
                SUB R8, R5, R7                                  ; go back VEL_BYTES in memory to get to location of velocity 
                LDR R7, [R8]                                    ; load word (velocity)
                MOV R0, R7                                      ; return velocity      
				
				B SWI_EXIT

SWI_view_vel_setpoint
                MOV R7, #VEL_SET_BYTES
                SUB R8, R5, R7                                  ; go back VEL_SET_BYTES in memory to get to location of velocity setpoint
                LDR R7, [R8]                                    ; load word (velocity set point)
                MOV R0, R7                                      ; return velocity set point      
				
				B SWI_EXIT

SWI_view_brake  MOV R7, #BRAKE_BYTES
                SUB R8, R5, R7                                  ; go back BRAKE_BYTES in memory to get to location of brake state 
                LDRB R7, [R8]                                   ; load byte (brake state)
                MOV R0, R7                                      ; return brake state      
				
				B SWI_EXIT

SWI_view_dms    MOV R7, #DMS_BYTES
                SUB R8, R5, R7                                  ; go back DMS_BYTES in memory to get to location of DMS state 
                LDRB R7, [R8]                                   ; load byte (DMS state)
                MOV R0, R7                                      ; return DMS state      
				
				B SWI_EXIT

SWI_OUT_OF_RANGE
				MOV R0, #-1										; throw error

SWI_EXIT
                MOVS PC, LR										; exit supervisor mode

				END
