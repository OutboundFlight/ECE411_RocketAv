//A custom Arduino SPI library for the M25P10-A flash chip.
//The chip can be initialized as an object by creating an object and calling
//the spiflash::init(int _cs) function and passing _cs as a parameter.
//
//Note that this library does not do initialization of the SPI bus as a whole.
//
//Once initialized, the chip can be written to using the class methods. The
//class also has a small amount of memory management for tracking which pages
//have been written and writing to the next available unwritten page.


#ifndef _SPIFLASH_H_
#define _SPIFLASH_H_


#include <SPI.h>


//chip-specific flash command codes
#define FLASH_RDSR 0x05
#define FLASH_WRSR  0x02
#define FLASH_WEN 0x06
#define FLASH_PP 0x02
#define FLASH_READ 0x03
#define FLASH_FASTREAD 0x0B
#define FLASH_CHIPERASE 0xC7
#define FLASH_WDIS 0x04

#define FLASH_SR_WIP 0x01

#define NUM_PAGES 512
#define PAGE_SIZE 256

//implementation-specific select lines for SPI devices for the tinyav system
#ifndef XM_SS
#define XM_SS 21
#endif

#ifndef GYR_SS
#define GYR_SS 20
#endif

#ifndef FLASH_SS
#define FLASH_SS 22
#endif

//
#ifndef SPICLOCK
#define SPICLOCK 2000000
#endif



class spiflash
{
	public:
		void writeEnable();
		void writeDisable();
		void chipErase();
		int isBusy();
		uint8_t readStatusRegister();
		void unprotect();
		void writeBytes(long address, uint8_t *data, int numbytes);
		void readBytes(long address, uint8_t *data, int numbytes);
		void init(int cs);
		int writeSequentialPage(uint8_t *data, int numbytes);
		
	protected:
		uint8_t head;               //pointer to current memory location
		uint8_t _cs;                //select line for this device
		bool page_table[NUM_PAGES]; //tracks which pages have been written
};

#endif
