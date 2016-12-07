#ifndef _SPIFLASH_H_
#define _SPIFLASH_H_


#include <SPI.h>

#define FLASH_RDSR 0x05
#define FLASH_WRSR  0x02
#define FLASH_WEN 0x06
#define FLASH_PP 0x02
#define FLASH_READ 0x03
#define FLASH_FASTREAD 0x0B
#define FLASH_CHIPERASE 0xC7
#define FLASH_WDIS 0x04

#define XM_SS 21
#define GYR_SS 20
#define FLASH_SS 22

#define SPICLOCK 1000000

#define NUM_PAGES 512
#define PAGE_SIZE 256

#define FLASH_SR_WIP 0x01

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
		uint8_t head;
		uint8_t _cs;
		bool page_table[NUM_PAGES];
};

#endif