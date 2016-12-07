#include <tinyav_spiflash.h>

#define BAUD 115200

spiflash flash;


void setup() {
  Serial.begin(BAUD);

  // start the SPI library:
  SPI.begin();

  flash.init(FLASH_SS);
  
  SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));

  // initalize the  data ready and chip select pins:
  pinMode(FLASH_SS, OUTPUT);
  digitalWrite(FLASH_SS, HIGH);
  pinMode(XM_SS, OUTPUT);
  digitalWrite(XM_SS, HIGH);
  pinMode(GYR_SS, OUTPUT);
  digitalWrite(GYR_SS, HIGH);
  flash.writeEnable();

  SPI.endTransaction();
  delay(5000);
}

void loop() 
{
  char input;
  if (Serial.available() > 0)
  {
    input = Serial.read();
    uint8_t buff[256];
    switch (input)
    {
      case 'e':
        Serial.print("Erasing chip... ");
        flash.chipErase();
        Serial.println("DONE");
        break;
      case 'r':
        flash.readBytes(0, buff, 256);
        for (int i = 0; i<256; i++)
        {
          Serial.print(buff[i], HEX);
          Serial.print('.');
        }
          Serial.println('\n');
          break;
        
      case 'A':
        buff[0] = 0xAA;
        buff[1] = 0xAA;
        flash.writeBytes(0, buff, 2);
        break;
      case 'B':
        buff[0] = 0xBB;
        buff[1] = 0xBB;
        flash.writeBytes(0, buff, 2);
        flash.writeBytes(2, buff, 2);
        break;
    }
  }
}

