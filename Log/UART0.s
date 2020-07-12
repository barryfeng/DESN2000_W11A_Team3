; Set up UART0 configuration and transmit message

; Reference: https://www.circuitbasics.com/basics-uart-communication/
;            https://www.lammertbies.nl/comm/info/serial-uart
;            UNSW DESN2000 lecture notes
;            NXP LPC24XX User Manual
            
            EXPORT UART0_start
            AREA UART0, CODE, READONLY 
PINSEL0     EQU 0xE002C000
U0START     EQU 0xE000C000
LCR0        EQU 0xC
LSR0        EQU 0x14
RAMSTART    EQU 0x40000000

            ENTRY

UART0_start LDR  SP, =RAMSTART          ; initialise stack pointer
            BL   uartConfig             ; harware and software config

                                        ; A1 contains character from string
loop        CMP  A1, #0                 ; check for null terminator
            BLNE transmit
            BNE  loop

done        B    done                   ; infinite loop when done


uartConfig  STMIA SP!, {R5, R6, LR}
            
            ; UART pin configuration
            LDR  R5, =PINSEL0           ; use PINSELO to configure pins
            LDR  R6, [R5]               
            BIC  R6, R6, 0xF0
            ORR  R6, R6, #0x50          ; set TXD0 and RXD0 pins
            STR  R6, [R5]
            
            ; UART protocol configuration
            LDR  R5, =UOSTART
            MOV  R6, #0x83              ; 8 bits, no parity, 1 stop bit
            STRB R6, [R5, #LCR0]        ; write control byteto LCR
            MOV  R6, #0x73              ; 9600 baud - 17.75MHz APB clk
            STRB R6, [R5]               ; store control byte 
            MOV  R6, #3                 ; set DLAB = 0
            STRB R6, [R5, #LCR0]        ; Tx and Rx buffers set up
            
            LDMDB SP!, {R5, R6, PC}


transmit    STMIA SP!, {R5, R6, LR}

            LDR  R5, =UOSTART
wait        LDRB R6, [R5, #LSR0]        ; get buffer status
            CMP  R6, #0x20              ; THRE HI - transmit buffer empty?
            BNE  wait                   ; loop until buffer is empty
            STRB A1, [R5]               ; A1 contains the character to be transmitted

            LDMDB SP!, {R5, R6, PC}
            
            END