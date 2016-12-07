Raw Setup:

PCB Creation:

1) In Eagle, use DRC > Masks to set Cream min to 5.5 mil and max to 7mil.Make sure this actually adjusts cream field sizes! The comparator footprint in the TI library is wronng.
2) Export to gerber file
3) User laser cutter at low power to cut stencil
4) Apply solder paste on PCB using the stencil as aid
5) Place components
6) User reflow oven to melt solder paste and secure components

Bootloader:

1) Plug in Dragon AVR 
2) Open AtmelStudio
3) Under Tools, go to Device Programming
4) Select DragonAVR-Atmega32U4-ISP
5) Apply
6) Click Check and verify the voltage
7) Go to Memory-Flash
8) Browse for Caterina-lilypadusb.hex 
9) Click Program
10) After programming, go to Fuse and set the fuses to the following:
	Extended byte: 0xCE
	High byte:     0xD0
	Low byte:      0xFF
11) Go to Lock Bit and set it to 0xFF
12) The bootloader should now be on the TinyAv

Programming:

1) Once bootloader is on TinyAv, it can now be plugged into a USB port (via MicroUSB)
2) Under Device Manager in Windows, the TinyAV should be seen as a port named Lilypad USB
3) Arduino IDE sketches can now be uploaded to the TinyAV using:
	Board: Arduino USB
	Port: Lilypad USB (COMX)
	Programmer: AVRISP mkll

	

