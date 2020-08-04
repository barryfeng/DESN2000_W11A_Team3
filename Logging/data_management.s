                    AREA store_data, CODE
			        EXPORT store
                    EXPORT view_vel
                    EXPORT view_vel_setpoint
                    EXPORT view_brake
                    EXPORT view_dms

SWI_store           EQU 0x0
SWI_vel             EQU 0x1
SWI_vel_set         EQU 0x2
SWI_brake           EQU 0x3
SWI_dms             EQU 0x4

store               SWI SWI_store
                    B finish

view_vel            SWI SWI_vel
                    B finish

view_vel_setpoint   SWI SWI_vel_set
                    B finish

view_brake          SWI SWI_brake
                    B finish

view_dms            SWI SWI_dms

finish              BX LR
			    
                    END