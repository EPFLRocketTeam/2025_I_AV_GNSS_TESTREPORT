#include <SparkFun_u-blox_GNSS_v3.h> //http://librarymanager/All#SparkFun_u-blox_GNSS_v3

unsigned long lastLoopMicros = 0;
unsigned long loopCount = 0;
unsigned long now_gps = 0;
int32_t latitude = 0;
int time_delay_loop = 10; //ms
int CpollingWait = 10; //ms
int freq_nav = 10; //hz

SFE_UBLOX_GNSS_SERIAL myGNSS; // SFE_UBLOX_GNSS_SERIAL uses Serial (UART). For I2C or SPI, see Example1 and Example3

#define mySerial Serial1 // Use Serial1 to connect to the GNSS module. Change this if required

void setup()
{
  Serial.begin(115200);
  delay(1000); 
  Serial.println("test config : time_delay_loop: " + String(time_delay_loop) + " CpollingWait: " + String(CpollingWait) + " freq_nav: " + String(freq_nav));

  mySerial.begin(38400); // u-blox F9 and M10 modules default to 38400 baud. Change this if required

  //myGNSS.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

  while (myGNSS.begin(mySerial) == false) //Connect to the u-blox module using mySerial (defined above)
  {
    Serial.println(F("u-blox GNSS not detected. Retrying..."));
    delay (1000);
  }

  myGNSS.setUART1Output(COM_TYPE_UBX); //Set the UART1 port to output UBX only (turn off NMEA noise)
  myGNSS.setNavigationFrequency(freq_nav, VAL_LAYER_RAM); // cette ligne doit être avant setI2CpollingWait car modifie aussi le pollingwait
  myGNSS.setAutoPVT(true); // Active l’envoi automatique des messages NAV-PVT

}

void loop()
{
  
  
  unsigned long now = micros();
  

  unsigned long loopDuration = now - lastLoopMicros;
  lastLoopMicros = now;
  loopCount++;
 
    unsigned long now_function = micros();
    
    int32_t new_latitude = myGNSS.getLatitude(CpollingWait);
    int32_t longitude = myGNSS.getLongitude(CpollingWait);
    int32_t altitude = myGNSS.getAltitudeMSL(CpollingWait);
    int32_t speed = myGNSS.getGroundSpeed(CpollingWait);
    int32_t velN = myGNSS.getNedNorthVel(CpollingWait);
    int32_t velE = myGNSS.getNedEastVel(CpollingWait);
    int32_t velD = myGNSS.getNedDownVel();
    uint32_t CarrierSolution = myGNSS.getCarrierSolutionType(CpollingWait);
    uint8_t fixType = myGNSS.getFixType(CpollingWait);
    uint8_t numSV = myGNSS.getSIV(CpollingWait);
    
    unsigned long time_to_get_data = micros() - now_function;

    Serial.println(F("=== DATA ==="));
    Serial.print(F("Lat: "));
    Serial.print(new_latitude);
    Serial.print(F(" | Lon: "));
    Serial.print(longitude);
    Serial.print(F(" (° * 10^-7)"));

    Serial.print(F(" | Alt: "));
    Serial.print(altitude);
    Serial.print(F(" mm"));

    Serial.print(F(" | Speed: "));
    Serial.print(speed / 1000.0);
    Serial.print(F(" m/s"));

    Serial.print(F(" | VelN: "));
    Serial.print(velN / 1000.0);

    Serial.print(F(" | VelE: "));
    Serial.print(velE / 1000.0);

    Serial.print(F(" | VelD: "));
    Serial.print(velD / 1000.0);
    Serial.print(F(" m/s"));

    Serial.print(F(" | CarrierSolution: "));
    Serial.print(CarrierSolution);
  
    Serial.print(F(" | Fix type: "));
    Serial.print(fixType); // 4 = RTK Float, 5 = RTK Fixed

    Serial.print(F(" | Satellites: "));
    Serial.println(numSV);

    // Affiche une info simple pour chaque passage dans la loop
    Serial.print(F("Loop #"));
    Serial.print(loopCount);
    Serial.print(F(" | Durée: "));
    Serial.print(loopDuration);
    Serial.println(F(" µs"));
    Serial.print(F(" | Time to get data: "));
    Serial.print(time_to_get_data);
    Serial.println(F(" µs"));



  delay(time_delay_loop);
}
