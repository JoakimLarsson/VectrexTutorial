
LB=/usr/local/bin/aslib
LN=/usr/local/bin/aslink
AS=/usr/local/bin/as6809
AFLAGS=-l -og 
CC=/usr/local/libexec/gcc/m6809-unknown-none/4.3.4/cc1

BINS  = dot2.bin vecpos.bin line1.bin line2.bin sound1.bin bouncer1.bin
OBJS  = $(BINS:.bin=.o) crt0.o
CRT0  = $(BINS:.bin=crt0.o)
RELS  = $(BINS:.bin=.rel)
LSTS  = $(BINS:.bin=.lst) crt0.lst
CLST  = $(BINS:.bin=crt0.lst)
RSTS  = $(BINS:.bin=.rst) crt0.rst
MAPS  = $(BINS:.bin=.map)
ROMS  = $(BINS:.bin=.rom)
RAMS  = $(BINS:.bin=.ram)
ASRC  = $(BINS:.bin=.s)
S19S  = $(BINS:.bin=.s19)
S19S += $(BINS:.bin=_ram.s19)

CLEAN_LIST= $(S19S) $(CRT0) $(ASRC) $(OBJS) $(RELS) $(LSTS) $(CLST) $(RSTS) $(MAPS) *~ $(RAMS) $(ROMS)

.PHONY: clean all

all: $(BINS)

%.bin: %.s19 %_ram.s19
	srec_cat $*_ram.s19 -offset -0xc880 -o $*.ram -binary || echo -n
	@touch $*.ram
	srec_cat $*.s19 -o $*.rom -binary
	cat $*.rom $*.ram > bin/$*.bin

%.s19 %_ram.s19: %.o %crt0.o
	$(LN) -m -nws -b .text=0x0 $*.s19 $*crt0.o $*.o

clean:
	@- $(RM) $(CLEAN_LIST)

%.o: %.asm
	$(AS) -l -og $<
	mv $*.rel $*.o

%.asm:
	cat crt0.tpl | sed -e s/XXX/`echo $* | sed -e "s/crt0//" | tr '[:lower:]' '[:upper:]'`/ > $*.asm

.s.o:
	$(AS) -l -og $<
	mv $*.rel $*.o

.c.o:
#	$(CC) $< -dumpbase $* -O3 -mno-direct -mint16 -msoft-reg-count=0 -auxbase $* -o $*.s
	$(CC) $< -dumpbase $* -O3 -mint8 -msoft-reg-count=0 -auxbase $* -o $*.s
	$(AS) -l -og $*.s
	mv $*.rel $*.o
