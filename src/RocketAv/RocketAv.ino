#include <SPI.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_Sensor.h>
#include <tinyav_spiflash.h>

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
  setupSensor();
  Serial.print("Please select: [i]dle mode, [l]aunch mode, [d]ownload data, [e]rase memory? ");
  if (!lsm.begin()) {
    Serial.println("Oops ... unable to initialize the LSM9DS0. Check your wiring!");
  }
  sample_number = 0;
  mode = 'i';
}

void loop() 
{
// Wait for USB port communication

  char input;
  int nextpage;
  if(Serial1)
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
        break;
        
      case 'd':
        nextpage = 0;
        for(int i = 0; i < PAGE_SIZE; i++) {pdata.bytes[i] = 0;}
        do
        {
          Serial.print("Reading page ");
          Serial.println(nextpage);
          flash.readPage(nextpage, pdata.bytes);
          nextpage = flash.nextWrittenPage(nextpage);

        
          for (int i = 0; i < samples_per_page; i++)
          {
            serialPrintSample(pdata.entries[i]);
          }          
        } while (nextpage != -1);

        break;
      case 'e':
        Serial.print("Erasing memory... ");
        flash.chipErase();
        Serial.println("Done!");
        break;
      }
      Serial.print("Please select: [i]dle mode, [l]aunch mode, [d]ownload data, [e]rase memory? ");
    }
    
  if (mode == 'i')
  {
    ;
  }  
  else if (mode == 'l')
  {
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
    if(Serial1)
    {
      Serial.print("Sample ");
      Serial.print(sample_number);
      Serial.print(":");
      serialPrintSample(curr);
    }

    sample_number++;

    if(sample_number >= samples_per_page)
    {
      int err;
      err = flash.writeSequentialPage(pdata.bytes, PAGE_SIZE);

      if (err == -1)
      {
        if (Serial1) Serial.println("ERROR: memory write out of bounds");
      }
      else
      {
        if (Serial1)
        {
          Serial.print("Writing page ");
          Serial.println(err);
        }
      }
      
      sample_number = 0;
      for(int i = 0; i < PAGE_SIZE; i++) {pdata.bytes[i] = 0;}

    }      
  }
}


void serialPrintSample(sensor_sample curr_data)
{
  Serial.print(curr_data.timestamp, HEX);
  Serial.print(',');
  Serial.print(curr_data.xm_x, HEX);
  Serial.print(',');
  Serial.print(curr_data.xm_y, HEX);
  Serial.print(',');   
  Serial.print(curr_data.xm_z, HEX);
  Serial.print(','); 
  Serial.print(curr_data.gyr_x, HEX);
  Serial.print(',');
  Serial.print(curr_data.gyr_y, HEX);
  Serial.print(',');
  Serial.print(curr_data.gyr_z, HEX);
  Serial.print(',');
  Serial.print(curr_data.mag_x, HEX);
  Serial.print(',');
  Serial.print(curr_data.mag_y, HEX);
  Serial.print(',');
  Serial.print(curr_data.mag_z, HEX);
  Serial.print(',');
  Serial.println(curr_data.temp, HEX);
}
