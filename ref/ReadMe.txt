Raw Setup:
-------------------------------------------------------------------------------------------------------------
PCB Creation:

1) In Eagle, use DRC > Masks to set Cream min to 5.5 mil and max to 7mil.Make sure this actually adjusts cream field sizes! 
   The comparator footprint in the TI library is wronng.
2) Export to gerber files.  Use the cream gerber layer to create the stencil.
3) Use the Full Spectrum Professional Series CO2 20x12 Laser to vector cut.
	Laser Settings:
	Vector Speed: 80%
	Vector Current: 20%
	Power: 0.2%
4) Line up stencil on board and secure.  Apply solder paste on PCB using the stencil as aid. 
	Tips: 
	1) Take the syringe tip off of the solder paste syringe
	2) Spread ~2-3 lines of solder paste, just above the stencil
	3) Using a Solder Spreader card, slowly spread the paste over the board (try to only swipe once!)
5) Place components.
6) Use the Torch T200N Reflow Oven to melt solder paste and secure components.

References:

Laser Cutter: https://github.com/psu-epl/psu-epl.github.com/wiki/Lasercutter
Reflow Oven:  https://github.com/psu-epl/psu-epl.github.com/wiki/Torch-T200N-Reflow-Oven 
-------------------------------------------------------------------------------------------------------------
Bootloader:

1) Plug in Dragon AVR (Located in FAB60-04).
2) Open AtmelStudio.
3) Under Tools, go to Device Programming.
4) Select DragonAVR-Atmega32U4-ISP.
5) Apply.
6) Click Check and verify the voltage.
7) Go to Memory-Flash.
8) Browse for Caterina-lilypadusb.hex in given GitHub folder.
9) Click Program.
10) After programming, go to Fuse and set the fuses to the following:
	Extended byte: 0xCE
	High byte:     0xD0
	Low byte:      0xFF
11) Go to Lock Bit and set it to 0xFF.
12) The bootloader should now be on the TinyAv.
-------------------------------------------------------------------------------------------------------------
Programming:

1) Once bootloader is on TinyAv, it can now be plugged into a USB port (via MicroUSB).
2) Under Device Manager in Windows, the TinyAV should be seen as a port named Lilypad USB.
3) Arduino IDE sketches can now be uploaded to the TinyAV using:
	Board: Arduino USB
	Port: Lilypad USB (COMX)
	Programmer: AVRISP mkll
4) Go to Sketch>Include Library and include the the 3 .zip libraries located in the src folder:
	Adafruit_LSM9DS0_Library-master.zip
	Adafruit_Sensor-master.zip
	tinyav.zip
5) Go to File>Open and open the RocketAv arduino sketch located in the src folder.
6) Ensure the Lilypad is powered on, plugged in via microUSB, and is detected under Tools>Ports
7) Upload the sketch on Arduino IDE
--------------------------------------------------------------------------------------------------------------
Launch Procedure:

1) Set up rocket
2) Turn on the tinyav board. Connect to USB port.
3) Get IMU sample data:
	Ensure xm is calibrated correctly (1g should be about integer 2000)
	Check sample rate
4) Upload correct sketch:
	Ensure the arduino sketch is not in debug mode
	Set a launch countdown timer in the sketch
5) Erase the memory chip
6) Insert motor with igniter. 
7) Set the arduino into launch mode. Start a stopwatch.
8) Detach the USB cable.
9) Place the tinyav board in the rocket.
10) Attach ignition control leads to igniter leads.
11) Clear launch zone.
12) Launch. 

