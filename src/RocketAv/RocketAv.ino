#include <SPI.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_Sensor.h>
#include <tinyav_spiflash.h>

// Define enable pins
#define MEM_EN 22
#define GYR_EN A2
#define XM_EN A3

// Hardware SPI setup for sensor
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(XM_EN, GYR_EN);


union sensorData {
struct Data {
  long timestamp
  int xm_x
  int xm_y
  int xm_z
  int gyr_x
  int gyr_y
  int gyr_z
  int mag_x
  int mag_y
  int mag_z
  int temp
  char line = 0xA;
}
uint8_t()
}

address 

sensordata bufferData[11] = {0}

void setupSensor()
{
  // 1.) Set the accelerometer range
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G);
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS);
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);

  // 3.) Setup the gyroscope
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS);
}



void setup() 
{
  Serial.begin(9600);
  Serial.println("~~RocketAV software version 1.0~~");
  spiflash flash;
  flash.init(MEM_EN);
}

void loop() 
{
// Wait for USB port communication
  #ifndef ESP8266
    while (!Serial);
  #endif


  
// Prompt for mode select
  Serial.println("Please select: [i]dle, [l]aunch, or [o]ptions?");
  input=Serial.read();



// Options
  if (input="o") {
    Serial.println("Please select: [d]ownload data or [e]rase memory");
    input=Serial.read();
  // Download data
    nextpage=0;    
    if (input="d") {
      while (nextpage!=-1){
        nextpage=flash.nextWrittenPage()
        flash.readSequentialPage(nextpage,bufferData)
        for (i=1:10){
          Serial.print(bufferData[i])
        }
      }
    }
  // Erase data
    if (input="e") {
      flash.chipErase()
      Serial.println("Memory erased!")
    }
  }



// Launch
  else if (input="l") { 
  // Try to initialise and warn if chip not detected
    if (!lsm.begin()) {
      Serial.println("Oops ... unable to initialize the LSM9DS0. Check your wiring!");
      break;
    }
    Serial.println("Found LSM9DS0 9DOF");
    Serial.println("");
    Serial.println("");

  // Read sensor
    lsm.read();
    databuffer[a]=[millis(),lsm.accelData.x,lsm.accelData.y,lsm.accelData.z,lsm.gyroData.x,lsm.gyroData.y,lsm.gyroData.z

    
  // Write to flash memory
  }



// Idle
  else if (input="i"){
    
  }


  
// Invalid input message
  else {
    Serial.println("INVALID INPUT, YA DINGUS!!!")
  }
  
}
