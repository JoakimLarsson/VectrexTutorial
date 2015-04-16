;;;
;;; Copyright 2006, 2007, 2008, 2009 by Brian Dominy <brian@oddchange.com>
;;; ported to Vectrex, 2013 by Frank Buss <fb@frank-buss.de>
;;;
;;; This file is part of GCC.
;;;
;;; GCC is free software; you can redistribute it and/or modify
;;; it under the terms of the GNU General Public License as published by
;;; the Free Software Foundation; either version 3, or (at your option)
;;; any later version.
;;;
;;; GCC is distributed in the hope that it will be useful,
;;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;; GNU General Public License for more details.

;;; You should have received a copy of the GNU General Public License
;;; along with GCC; see the file COPYING3.  If not see
;;; <http://www.gnu.org/licenses/>.

	; Declare external for main()
	.module	crt0.tpl
	.area .text
	.globl _main

#define __STACK_TOP 0xcbea

	; Declare all linker sections, and combine them into a single bank
	.bank prog
	.area .text  (BANK=prog)
	.area .ctors (BANK=prog)

	.bank ram(BASE=0xc880,SIZE=0x26a,FSFX=_ram)
	.area .data  (BANK=ram)
	.area .bss   (BANK=ram)

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;
	;;; cartridge init block
	;;;
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.area	.text
	.ascii "g GCE 2015"		; cartrige id and year
	.byte 0x80			; string end
	.word 0xfd0d			; address to music1 in BIOS
	.byte 0xf8, 0x50, 0x20, -0x30	; height, width, rel y, rel x
	.ascii "XXX"  			; game title
	.byte 0x80			; string end
	.byte 0				; header end

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;
	;;; __start : Entry point to the program
	;;;
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.area	.text
	.globl __start
__start:

; TODO: this code crashs
	;; Call any "initializer" functions
;	ldu	#s_.ctors
__ctors_loop:
;	ldy	,u++
;	cmpy	#0
;	beq	__ctors_done
;	jsr	,y
;	bra	__ctors_loop
__ctors_done:

	; copy .data and .bss areas to RAM
	ldx	#l_.text
	ldy	#0xc880
	ldb	#0x26a
copyData:
	lda	,x+
	sta	,y+
	decb
	bne	copyData

	; start C program
	jmp	_main

#music:
#        .word   0xfee8
#        .word   0xfeb6
#        .byte   0x0, 0x80
#        .byte   0x0, 0x80

	.end __start

