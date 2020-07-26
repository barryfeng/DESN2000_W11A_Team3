; Store and access data from memory
; Data is stored in static memory bank 2

; R0 -> string
; R1 -> SWI operation
            
            EXPORT data_start
            AREA log_data, CODE

bank_2      EQU 0x8200 0000
data_len    EQU 0xFFFFFF32              ; set this value to the NEGATIVE of the length of struct data
SWI_store   EQU 0x0
SWI_curr    EQU 0x1                     ; current data
SWI_vel     EQU 0x2

            ENTRY

data_start  LDR r2, =bank_2             ; address of memory bank
            LDR r3, [r2]

empty       CMP r3, #0                  ; check to see that data is not currently stored in memory
            BNE interrupt
            ADD r2, r2, #4              ; check next word
            LDR r3, [r2]                ; load word at empty memory location
            B empty

interrupt   ADR r4, jumptable
            LDR pc, [r4, r1, LSL #2]    ; move to correct SWI instruction

jumptable   DCD store, curr_data

store       SWI SWI_store
            B store_hand                ; handler for store instrcution

curr_data   SWI SWI_curr
            B curr_hand                 ; handler for viewing data

store_hand  MOV r4, #0                  ; offset
            STRB r1, [r2, r4]           ; store byte in memory - character is one byte
            ADD r4, r4, #1              ; increment offset
            CMP r1, #0                  ; check if reached the null terminator
            BNE store
            B SWI_exit

curr_hand   ; TODO

SWI_exit    MOVS pc, lr
