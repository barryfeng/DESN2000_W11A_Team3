; Exception Vector Table:
; These instructions are mapped to address 0x0000 0000. 
; Unimplemented handlers are currently infinite loops.
; This has been moved into it's own file for readability.

                PRESERVE8						; Preserve 8-bit Alignment
                AREA    RESET, CODE, READONLY

ResetVect       LDR     PC, Reset_Addr			; 0x0000
UndefVect       LDR     PC, Undef_Addr			; 0x0004
SWIVect         LDR     PC, SWI_Addr			; 0x0008
PAbtVect        LDR     PC, PAbt_Addr			; 0x000C
DabtVect        LDR     PC, DAbt_Addr			; 0x0010
                NOP                             ; 0x0014 Reserved Vector 
IRQVect         LDR     PC, IRQ_Addr			; 0x0018
FIQVect         LDR     PC, FIQ_Addr			; 0x001C

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Reset_Addr      DCD     Reset_Handler
Undef_Addr      DCD     Undef_Handler
SWI_Addr        DCD     SWI_Handler
PAbt_Addr       DCD     PAbt_Handler
DAbt_Addr       DCD     DAbt_Handler
                DCD     0                 		; Reserved Address 
IRQ_Addr        DCD     IRQ_Handler
FIQ_Addr        DCD     FIQ_Handler

Undef_Handler   B       Undef_Handler			; Currently an infinite loop

				IMPORT  SWI_Handler				; handler for software interrupt

PAbt_Handler    B       PAbt_Handler			; Currently an infinite loop
DAbt_Handler    B       DAbt_Handler			; Currently an infinite loop
IRQ_Handler		B		IRQ_Handler				; Currently an infinite loop
FIQ_Handler     B       FIQ_Handler				; Currently an infinite loop
Reset_Handler   B       Reset_Handler           ; Currently an infinite loop

				END