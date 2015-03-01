;  From http://vectrexmuseum.com/share/coder/html/dot2.asm
;  Fixed to compile with AS6809 by Joakim Larsson Edström 2015
; 
;   Assembles with
;  /usr/local/bin/as6809 -l -og dt2.asm
;  /usr/local/bin/aslink -m -nws -b .text=0x0 dt2.rel
;  /usr/local/bin/srec_cat.exe dt2.s19 -o dt2.bin -binary
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
        .byte      0xF8, 0x50, 0x20, -0x70 ; height, width, rel y, rel x
        .ascii     "PLOT A LIST OF DOTS"   ; Banner text
	.byte      0x80	                   ; text ending with 0x80
                                
        .byte      0                       ; end of game header
;***************************************************************************
; CODE SECTION
;***************************************************************************
; here the cartridge program starts off
main:
                JSR     Wait_Recal              ; Vectrex BIOS recalibration
                JSR     Intensity_5F            ; Sets the intensity of the
                                                ; vector beam to $5f
                JSR     Delay_3                 ; delay for 30 cycles
                LDA     #50                     ; load 50
                STA     VIA_t1_cnt_lo           ; 50 as scaling
                LDA     #6                      ; load A with 6, dots - 1
                STA     Vec_Misc_Count          ; set it as counter for dots
                LDX     #dot_list               ; load the address of dot_list
                JSR     Dot_List                ; Plot a series of dots
                BRA     main                    ; and repeat forever
;***************************************************************************
; DATA SECTION
;***************************************************************************
dot_list:
                .byte       30,-70                 ; seven dots, relative
                .byte      -40, 10                 ; position, Y, X
                .byte        0, 30
                .byte       40, 10
                .byte       10, 30
                .byte        5, 30
                .byte       -10,40
;***************************************************************************
                .END  main
;***************************************************************************
