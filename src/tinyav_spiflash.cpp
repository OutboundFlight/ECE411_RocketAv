// the sensor communicates using SPI, so include the library:
#include <tinyav_spiflash.h>

void spiflash::init(int cs)
{
	spiflash::_cs = cs;
	pinMode(_cs, OUTPUT);
	digitalWrite(_cs, HIGH);
}

void spiflash::writeEnable()
{
  digitalWrite(FLASH_SS, LOW);
  SPI.transfer(FLASH_WEN);
  digitalWrite(FLASH_SS, HIGH);
}

void spiflash::writeDisable()
{
	digitalWrite(FLASH_SS, LOW);
	SPI.transfer(FLASH_WDIS);
	digitalWrite(FLASH_SS, HIGH);
}

void spiflash::chipErase()//
{
	writeEnable();
	SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
  digitalWrite(FLASH_SS, LOW);
  SPI.transfer(FLASH_CHIPERASE);
  digitalWrite(FLASH_SS, HIGH);
	SPI.endTransaction();
  for (int i = 0; i < NUM_PAGES; i++)
	  page_table[i] = 0;
  
  head = 0;

  while(isBusy());  
  writeEnable();
}

int spiflash::isBusy()
{
  digitalWrite(FLASH_SS, LOW);
  SPI.transfer(FLASH_RDSR);
  int sreg = SPI.transfer(0x00);
  digitalWrite(FLASH_SS, HIGH);
  return (sreg & FLASH_SR_WIP);

}

uint8_t spiflash::readStatusRegister()
{
	digitalWrite(FLASH_SS, LOW);
	SPI.transfer(FLASH_RDSR);
	int sreg = SPI.transfer(0x00);
	digitalWrite(FLASH_SS, HIGH);
	return sreg;
}

void spiflash::unprotect()
{
  digitalWrite(FLASH_SS, LOW);
  SPI.transfer(FLASH_WRSR);
  SPI.transfer(0x00);
  digitalWrite(FLASH_SS, HIGH);
}

int spiflash::writeSequentialPage(uint8_t *data, int numbytes)
{
	while(page_table[head])
		head++;
	
	if (!(head < NUM_PAGES))
		return -1;
	
	long addr = head << 8;
	writeBytes(addr, data, numbytes);
}

void spiflash::writeBytes(long address, uint8_t *data, int numbytes)
{
  writeEnable();
  digitalWrite(FLASH_SS, LOW);
  SPI.transfer(FLASH_PP);
  SPI.transfer(address >> 16);
  SPI.transfer(address >> 8);
  SPI.transfer(address);
  for (int i = 0; i < numbytes; i++)
  {
     SPI.transfer(*data);
     data++;
  }
  digitalWrite(FLASH_SS, HIGH);
  page_table[address >> 8] = 1;
  while(isBusy());
}

void spiflash::readBytes(long address, uint8_t *data, int numbytes)
{
  digitalWrite(FLASH_SS, LOW);
  SPI.transfer(FLASH_FASTREAD);
  SPI.transfer(address >> 16);
  SPI.transfer(address >> 8);
  SPI.transfer(address);              //send the address as three bytes
  SPI.transfer(0x00);                 //dummy byte (required by chip)
  for (int i = 0; i < numbytes; i++)
  {
    *data = SPI.transfer(0x00);       //send dummy bytes to retrieve sequential addresses
    data++;
  }
 
  digitalWrite(FLASH_SS, HIGH);
}