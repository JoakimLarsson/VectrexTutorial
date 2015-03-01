;
;  Fixed to compile with AS6809 by Joakim Larsson Edström 2015
; 
;   Assembles with
;  /usr/local/bin/as6809 -l -og snd1.asm
;  /usr/local/bin/aslink -m -nws -b .text=0x0 snd1.rel
;  /usr/local/bin/srec_cat.exe snd1.s19 -o snd1.bin -binary
;
; 
;***************************************************************************
; DEFINE SECTION
;***************************************************************************
                .INCLUDE "VECTREX.INC"     ; vectrex function includes

; start of vectrex memory with cartridge name...
		.area .text
;***************************************************************************
; HEADER SECTION
;***************************************************************************
        .ascii     "g GCE 1998"            ; 'g' is copyright sign
	.byte      0x80                    ; text ending with 0x80
        .word      music1                  ; music from the rom
        .byte      0xF8, 0x50, 0x20, -0x55 ; height, width, rel y, rel x
        .ascii     "VECTOR LIST TEST"      ; Banner text
	.byte      0x80	                   ; text ending with 0x80
                                
        .byte      0                       ; end of game header
;***************************************************************************
; CODE SECTION
;***************************************************************************
; here the cartridge program starts off
main:
                LDA     #1                      ; one means, we are about to
                                                ; start a piece of music
                STA     Vec_Music_Flag          ; store it in appropriate RAM
                                                ; location
main_loop:
                JSR     DP_to_C8                ; DP to RAM
                LDU     #music1                 ; get some music, here music1
                JSR     Init_Music_chk          ; and init new notes
                JSR     Wait_Recal              ; Vectrex BIOS recalibration
                JSR     Do_Sound                ; ROM function that does the
                                                ; sound playing
                BRA     main_loop               ; and repeat forever
;***************************************************************************
                .END main
;***************************************************************************
