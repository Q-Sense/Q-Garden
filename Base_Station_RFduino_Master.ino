#include <Arduino.h>
#include <RFduinoGZLL.h>
#include <Wire.h>

#define SDApin 4              //Data pin
#define SCLpin 5              //Clock pin
#define address 0x4D          //TC74 temperature sensor adress
#define valvePin 6

device_t role = HOST;         //This RFduino role is: HOST/Master
volatile int sensorValue = 0;
int avgMoisture = 0;
int targetMoisture = 0;
int flagWatering = 0;
byte val = 0;
int x = 0;

void setup()
{
	
	Wire.beginOnPins(5,4);              //clock pin 5, data pin 4
	Serial.begin(9600);
	Serial.write("Program Runnig");     //Debug
	delay(100);
	RFduinoGZLL.begin(role);

}


void loop()
{

	int temperature;

	Wire.beginTransmission(address);      //starts the transmission
	
	Wire.write(val);

	Wire.requestFrom(address, 1);
	if (Wire.available()) {
		temperature = Wire.read();
		//Serial.println(temperature);      //Debug
	}
	Serial.println(sensorValue);

	sensorValue = sensorValue * 100/1023;
	Serial.println(sensorValue);
	sensorValue = 100 - sensorValue;        //converts the soil moisture sensor data to percentage        
	Serial.println(sensorValue);

	//*********************end of getting temperature**********************
	if (temperature > 0){
	//***********sendind data to slave device*********************
	Wire.beginTransmission(4);   // transmit to device #4
	Wire.write(sensorValue);     // sends temperature
	Wire.write(temperature);	 // sends average moisture
	Wire.write(flagWatering);
	Wire.endTransmission();      // stop transmitting
	Serial.println("sended");
	//Serial.println(sensorValue);
	//******************end of dending data to slave********************
}
	delay(50);
	//*****************requesting data from slave***************************
	//recebe os valores de humidade pretendidos.
	
	int iscRead = 0;
	Wire.requestFrom(4,	1);
	targetMoisture= Wire.read();
	//Serial.print(targetMoisture);
	//Serial.println();
	//****************end of request****************************
	/*

//Code for several sensors

	avgMoisture = 0;
	for (int i = 0; i < 2; i++){
		
		avgMoisture += sensorValue[i];
		//avgMoisture = int(avgMoisture / 2);
	}
	
	*/
	if ((targetMoisture - sensorValue>5) || flagWatering ){
		if (avgMoisture < targetMoisture){
			flagWatering = 1;
		}
		else{
		        flagWatering = 0;
	        }
	}

	delay(3000);
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len){ //Receives data from the Slave RFduinos
	
	if (device == 7){        
	    sensorValue = atoi(data);
	    Serial.print("recebido: "); //Debug
	    Serial.println(data);
        }
	
}
