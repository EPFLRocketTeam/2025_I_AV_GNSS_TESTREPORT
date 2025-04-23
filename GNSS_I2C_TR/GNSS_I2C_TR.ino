#include <Wire.h>
#include <SparkFun_u-blox_GNSS_v3.h>

SFE_UBLOX_GNSS myGNSS;

unsigned long lastLoopMicros = 0;
unsigned long loopCount = 0;
unsigned long now_gps = 0;
int time_delay_loop = 10; //ms
int CpollingWait = 10; //ms
int freq_nav = 10; //hz

void setup()
{
  Serial.begin(115200);
  delay(1000); // Laisse le temps à la console série de s'initialiser

  Wire.begin();
  // Active les logs si besoin :
  // myGNSS.enableDebugging();

  while (!myGNSS.begin())
  {
    Serial.println(F("u-blox GNSS non détecté à l'adresse I2C par défaut. Nouvelle tentative..."));
    delay(500);
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); // On veut uniquement les messages UBX (pas de NMEA ni RTCM)
  myGNSS.setNavigationFrequency(freq_nav, VAL_LAYER_RAM); // cette ligne doit être avant setI2CpollingWait car modifie aussi le pollingwait
  myGNSS.setAutoPVT(true); // Active l’envoi automatique des messages NAV-PVT
  myGNSS.setI2CpollingWait(CpollingWait);

Serial.println("test config : time_delay_loop: " + String(time_delay_loop) + " CpollingWait: " + String(CpollingWait) + " freq_nav: " + String(freq_nav));
}

void loop()
{
  
  unsigned long now = micros();
  

  unsigned long loopDuration = now - lastLoopMicros;
  lastLoopMicros = now;
  loopCount++;
 
    unsigned long now_function = micros();
    
    int32_t latitude = myGNSS.getLatitude();
    int32_t longitude = myGNSS.getLongitude();
    int32_t altitude = myGNSS.getAltitudeMSL();
    int32_t speed = myGNSS.getGroundSpeed();
    int32_t velN = myGNSS.getNedNorthVel();
    int32_t velE = myGNSS.getNedEastVel();
    int32_t velD = myGNSS.getNedDownVel();
    uint32_t CarrierSolution = myGNSS.getCarrierSolutionType();
    uint8_t fixType = myGNSS.getFixType();
    uint8_t numSV = myGNSS.getSIV();
    
    unsigned long time_to_get_data = micros() - now_function;

    Serial.println(F("=== DATA ==="));
    Serial.print(F("Lat: "));
    Serial.print(latitude);
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
