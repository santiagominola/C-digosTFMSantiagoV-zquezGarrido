#include "Particle.h"

// dependencies.SdFat=0.0.7
#include "SdFat.h"
#include "APNHelperRK.h"
#include "MQTT.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

byte serverHeimdall[] = { 157, 230, 221, 206 };
MQTT clientHeimdall(serverHeimdall, 1883, 255, 450, callback);

void callback(char* topic, byte* payload, unsigned int length) {
}


const APNHelperAPN apns[1] = {
{ "8956010", "m2m.entel.cl" }
};
APNHelper apnHelper(apns, sizeof(apns) / sizeof(apns[0]));



// Primary SPI with DMA
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
SdFat sd;
const uint8_t chipSelect = A5;
uint16_t analogVal, analogVal2, psiVal, vVal, vVal2;

const unsigned long TEST_INTERVAL_MS = 30000; // Este es el tiempo entre reportes 
unsigned long lastTest = 0;


void logPressureData();


void setup() {
	//Serial.begin();
	//delay(1000);
	apnHelper.setCredentials();
    Particle.connect();
	delay(15000);
	clientHeimdall.connect("clientLogger");
}

void loop() {
	if (clientHeimdall.isConnected()){
            clientHeimdall.loop();
    }
	else{
		clientHeimdall.connect("clientLogger");
		delay(100);
	}
	if (millis() - lastTest >= TEST_INTERVAL_MS) {
		lastTest = millis();
		//tryMeFirst();
		analogVal = analogRead(A0);
		analogVal2 = analogRead(A1);
		//if (analogVal < 500)
		//{
		//	psiVal = ((analogVal - 410) / 9 );
		//}
		//else if (analogVal < 630)
		//{
		//	psiVal = ((analogVal - 500) / 13 ) + 10;
		//}
		//else
		//{
		//	psiVal = map(analogVal, 630, 2420, 20, 130);
		//}
		
		//Serial.print("Analog: ");
		//Serial.println(analogVal);
		vVal = (analogVal * 3300 ) / 4096;
		vVal2 = (analogVal2 * 3300 ) / 4096;

		//Serial.print("Presion C1: ");
		//Serial.print(psiVal);
		//Serial.print(" PSI  @  ");
		//Serial.println(Time.timeStr());
		//logPressureData();
		clientHeimdall.publish("sensor/loggerA0", String(vVal));
		clientHeimdall.publish("sensor/loggerA1", String(vVal2));
		String vValString = String(vVal);
 		Particle.publish("vBatt", vValString, PRIVATE);
	}
}

void logPressureData() {
	File myFile;

	// Initialize the library
	if (!sd.begin(chipSelect, SD_SCK_MHZ(25))) {
		Serial.println("failed to open card");
		return;
	}

	// open the file for write at end like the "Native SD library"
	if (!myFile.open("voltageLog.txt", O_RDWR | O_CREAT | O_AT_END)) {
		Serial.println("opening test.txt for write failed");
		return;
	}
	// if the file opened okay, write to it:
	myFile.print(Time.timeStr());
	myFile.printf(" @ %d mV \n", vVal);

	// close the file:
	myFile.close();
}