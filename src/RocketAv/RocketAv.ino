#include <SPI.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_Sensor.h>
#include <tinyav_spiflash.h>

#define SENSOR_STALL 2

//#define SERIAL_DEBUG

// Define enable pins
#define MEM_EN 22
#define GYR_EN A2
#define XM_EN A3

#define BAUD 115200

// Hardware SPI setup for sensor
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(XM_EN, GYR_EN);

typedef struct sensor_sample {
  long timestamp;
  int xm_x;
  int xm_y;
  int xm_z;
  int gyr_x;
  int gyr_y;
  int gyr_z;
  int mag_x;
  int mag_y;
  int mag_z;
  int temp;
  char line;
}sensor_sample;

int samples_per_page = PAGE_SIZE/sizeof(sensor_sample);

union page
{
  sensor_sample entries[12];
  uint8_t bytes [PAGE_SIZE];
} pdata;

void setupSensor()
{
  // 1.) Set the accelerometer range
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G);
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);

  // 3.) Setup the gyroscope
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS);
}
 spiflash flash;

int sample_number;
  char mode;
int current_page;

void setup() 
{
  Serial.begin(BAUD);
  Serial.println("~~RocketAV software version 1.0~~");
  flash.init(MEM_EN);
  Serial.print("Please select: [i]dle mode, [l]aunch mode, [d]ownload data, [e]rase memory? ");
  if (!lsm.begin()) {
    Serial.println("Oops ... unable to initialize the LSM9DS0. Check your wiring!");
  }

  setupSensor();
  sample_number = 0;
  mode = 'i';
  flash.unprotect();
}

void loop() 
{
// Wait for USB port communication

  char input;
  int nextpage;
  if(Serial1)
  {
  if(Serial.available())
  { 
    input=Serial.read();
    switch (input)
    {
      case 'i':
        mode = 'i';
        Serial.println("Entering idle mode...");
        break;
      
      case 'l':
        mode = 'l';
        sample_number = 0;
        Serial.println("Entering launch mode...");
        Serial.println("Beginning 60s launch timer...");
        delay(120000);
        break;
        
      case 'd':
        nextpage = 0;
        for(int i = 0; i < PAGE_SIZE; i++) {pdata.bytes[i] = 0;}
        Serial.print("Reading from flash head at 0x");
        Serial.println(flash.getHead(), HEX);
        for(uint16_t i = 0; i < flash.getHead(); i++)
        {

//          Serial.print("Reading page ");
//          Serial.println(i);
          uint32_t addr = flash.readPage(i, pdata.bytes);
//          Serial.print("Actual address ");
//          Serial.println(addr, HEX);
          for (int j = 0; j < samples_per_page; j++)
          {
            serialPrintSample(pdata.entries[j]);
          }          
        };

        break;
      case 'e':
        Serial.print("Erasing memory... ");
        flash.chipErase();
        Serial.println("Done!");
        break;
      }
      Serial.print("Please select: [i]dle mode, [l]aunch mode, [d]ownload data, [e]rase memory? ");
    }
  }
    
  if (mode == 'i')
  {
    ;
  }  
  else if (mode == 'l')
  {
    delay(SENSOR_STALL);
    lsm.read();
    sensor_sample curr;

    curr.timestamp = millis();
    curr.xm_x = lsm.accelData.x;
    curr.xm_y = lsm.accelData.y;
    curr.xm_z = lsm.accelData.z;
    curr.gyr_x = lsm.gyroData.x;
    curr.gyr_y = lsm.gyroData.y;
    curr.gyr_z = lsm.gyroData.z;
    curr.mag_x = lsm.magData.x;
    curr.mag_y = lsm.magData.y;
    curr.mag_z = lsm.magData.z;
    curr.temp = lsm.temperature;
    curr.line = 0xA;
    pdata.entries[sample_number] = curr;

    #ifdef SERIAL_DEBUG
    if(Serial1)
    {
      Serial.print("Sample ");
      Serial.print(sample_number);
      Serial.print(":");
      serialPrintSample(curr);
    }
    #endif

    sample_number++;

    if(sample_number >= samples_per_page)
    {
      int err;
      err = flash.writeSequentialPage(pdata.bytes, PAGE_SIZE);
      
      sample_number = 0;
      for(int i = 0; i < PAGE_SIZE; i++) {pdata.bytes[i] = 0;}

    #ifdef SERIAL_DEBUG
    Serial.print("Printing page");
    Serial.println(err);
    Serial.print("Head location: ");
    Serial.println(flash.getHead(), HEX);
    Serial.print("Address equivalent: ");
    Serial.println(((uint32_t)flash.getHead()) << 8, HEX);
    #endif
    }

  }
}


void serialPrintSample(sensor_sample curr_data)
{
  Serial.print(curr_data.timestamp);
  Serial.print(',');
  Serial.print(curr_data.xm_x);
  Serial.print(',');
  Serial.print(curr_data.xm_y);
  Serial.print(',');   
  Serial.print(curr_data.xm_z);
  Serial.print(','); 
  Serial.print(curr_data.gyr_x);
  Serial.print(',');
  Serial.print(curr_data.gyr_y);
  Serial.print(',');
  Serial.print(curr_data.gyr_z);
  Serial.print(',');
  Serial.print(curr_data.mag_x);
  Serial.print(',');
  Serial.print(curr_data.mag_y);
  Serial.print(',');
  Serial.print(curr_data.mag_z);
  Serial.print(',');
  Serial.println(curr_data.temp);
}
