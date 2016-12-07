#include <tinyav_spiflash.h>
#define PAGE_TO_ADDRESS(a) (a << 8)
#define ADDRESS_TO_PAGE(a) (a >> 8)
#define ADDRESS_BYTE0(a) ((a & 0x110000) >> 16)
#define ADDRESS_BYTE1(a) ((a & 0x001100) >> 8)
#define ADDRESS_BYTE2(a) (a & 0x000011)

//Pass a single-byte command to the device
void spiflash::bytecommand(uint8_t code)
{
    digitalWrite(_cs, LOW);
    SPI.transfer(code);
    digitalWrite(_cs, HIGH);
}

//Initialize the device by setting up the select pin
void spiflash::init(int cs)
{
	spiflash::_cs = cs;
	pinMode(_cs, OUTPUT);
	digitalWrite(_cs, HIGH);
}

//Enable writing/block erase/chip erase
void spiflash::writeEnable() 
{ bytecommand(FLASH_WEN); }

//disable writing/block erase/chip erase
void spiflash::writeDisable()
{ bytecommand(FLASH_WDIS); }

//Erase chip by writing 1's to all memory locations
void spiflash::chipErase()//
{
	writeEnable(); //the flash chip wants a write enable command before each write

	bytecommand(FLASH_CHIPERASE);

    for (int i = 0; i < NUM_PAGES; i++)     //reset the page table
	  page_table[i] = 0;
  
  head = 0;                                 //set the head to lowest address

  while(isBusy());                  //wait until page write completes
  writeEnable();                    //another write enable for sanity
}

//reads the status register to determine if the chip is busy
int spiflash::isBusy()
{
    //retrieve status reg, binary and with bitmask for Write In Progress
    return (readStatusRegister() & FLASH_SR_WIP);  
}

//read status register
uint8_t spiflash::readStatusRegister()
{
	digitalWrite(_ce, LOW);
	SPI.transfer(FLASH_RDSR);
	int sreg = SPI.transfer(0x00);
	digitalWrite(_ce, HIGH);
	return sreg;
}

//write the status register to unprotect the sectors.
//NOT TESTED. DO NOT USE.
/*
void spiflash::unprotect()
{
  digitalWrite(_ce, LOW);
  SPI.transfer(FLASH_WRSR);
  SPI.transfer(0x00);
  digitalWrite(_ce, HIGH);
}

*/

//Writes to the next available page in program memory
int spiflash::writeSequentialPage(uint8_t *data, int numbytes)
{
    if (numbytes > PAGE_SIZE)
        return -1;

	while(page_table[head])
		head++;
	
	if (head >= NUM_PAGES)
		return -1;
	
	long addr = PAGE_TO_ADDRESS(head);
	writeBytes(addr, data, numbytes);
}

void spiflash::writeBytes(long address, uint8_t *data, int numbytes)
{
  writeEnable();
  digitalWrite(_ce, LOW);
  SPI.transfer(FLASH_PP);
  SPI.transfer(address >> 16);
  SPI.transfer(address >> 8);
  SPI.transfer(address);
  for (int i = 0; i < numbytes; i++)
  {
     SPI.transfer(*data);
     data++;
  }
  digitalWrite(_ce, HIGH);
  page_table[address >> 8] = 1;
  while(isBusy());
}

int nextWrittenPage(int readhead)
{
    do
    {
        readhead++;
        if (readhead > NUM_PAGES)
        {
            readhead = -1;
            break;
        }
    }while (page_table[readhead] == 0);
    
    return readhead;
}

void spiflash::readPage(int page, uint8_t *data)
{
    digitalWrite(_ce, LOW);

    long addr = PAGE_TO_ADDRESS(page);

    SPI.transfer(FLASH_FASTREAD);
    SPI.transfer(ADDRESS_BYTE0(addr));
    SPI.transfer(ADDRESS_BYTE1(addr));
    SPI.transfer(ADDRESS_BYTE2(addr));
    SPI.transfer(0x0);
    for (int i = 0; i < PAGE_SIZE; i++)
    {
        *data = SPI.transfer(0x0);
        data++
    }
}

void spiflash::readBytes(long address, uint8_t *data, int numbytes)
{
  digitalWrite(_ce, LOW);
  SPI.transfer(FLASH_FASTREAD);
  SPI.transfer(ADDRESS_BYTE0(address));
  SPI.transfer(ADDRESS_BYTE1(address));
  SPI.transfer(ADDRESS_BYTE2(address));              //send the address as three bytes
  SPI.transfer(0x00);                 //dummy byte (required by chip)
  for (int i = 0; i < numbytes; i++)
  {
    *data = SPI.transfer(0x00);       //send dummy bytes to retrieve sequential addresses
    data++;
  }
 
  digitalWrite(_ce, HIGH);
}
