####
TARGET = msp430g2553
BOARD = LAUNCHPAD
CC = msp430-gcc
CFLAGS = -mmcu=$(TARGET) -lm -D "_DEV_$(BOARD)" -g
CFLAGS_SIM = -mmcu=msp430f2131
#VPATH = /home/macrobull/lib/msp430/include

OSC_OBJ = oscilloscope_2ch_conseq2.o uart.o adc.o
AQI_OBJ = AQI.o uart.o adc.o

main: clean clear osc

flash:
	mspdebug rf2500 "prog osc.elf"

osc: $(OSC_OBJ)
	$(CC) -o osc.elf $(CFLAGS) $(OSC_OBJ)
	msp430-objdump -d osc.elf > asm.asm

	
aqi: $(AQI_OBJ)
	$(CC) -o aqi.elf $(CFLAGS) $(AQI_OBJ)
	

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clear:
	clear

clean:
	-rm *.elf *.o
	#-rm $(VPATH)/*.o
