; This file is used to initiate a sofwtare interrupt with a flag
; that corresponds to the correct logging operation. These include:
; - store data
; - view velocity
; - view velocity set point (target velocity)
; - view brake state (on/off)
; - view DMS (dead man switch) state (on/off)
                    
                    AREA store_data, CODE
			        EXPORT store
                    EXPORT view_vel
                    EXPORT view_vel_setpoint
                    EXPORT view_brake
                    EXPORT view_dms

SWI_store           EQU 0x0                     ; SWI flag for storing data
SWI_vel             EQU 0x1                     ; SWI flag for viewing velocity 
SWI_vel_set         EQU 0x2                     ; SWI flag for viewing velocity set point (target velocity)
SWI_brake           EQU 0x3                     ; SWI flag for viewing brake state
SWI_dms             EQU 0x4                     ; SWI flag for viewing DMS state

; initiate software interrupt with store flag
store               SWI SWI_store
                    B finish

; initiate software interrupt with view velocity flag
view_vel            SWI SWI_vel
                    B finish

; initiate software interrupt with view velocity set point flag
view_vel_setpoint   SWI SWI_vel_set
                    B finish

; initiate software interrupt with view brake state flag
view_brake          SWI SWI_brake
                    B finish

; initiate software interrupt with view DMS state flag
view_dms            SWI SWI_dms

; return to access_data 
finish              BX LR
			    
                    END