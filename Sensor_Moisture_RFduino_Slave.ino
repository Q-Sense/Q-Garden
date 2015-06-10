#include <Arduino.h>
#include <RFduinoGZLL.h>
#include <stdlib.h>
#include <stdio.h>
//O n� device tem de ser incrementado para cada sensor
//Para se fazer hopping o device central tera de passar a ser host


device_t role = DEVICE7;        //Adress of this RFduino
int sensorPin = 2;
unsigned int sensorValue = 0;
char *string;
char buffer[5];
void setup()
{
	pinMode(sensorPin, INPUT);
	RFduinoGZLL.begin(role);
	Serial.begin(9600);
	//Serial.write("Starting...");    //Debug
	delay(1);

}

void loop()
{
	sensorValue = analogRead(sensorPin);      //Reads the moisture sensor
	//string=intToString(sensorValue);
	
	int x = sensorValue;
	char xstr[] = "0000";
	xstr[3] = '0' + (x % 10U); // ones digit
	xstr[2] = '0' + ((x / 10U) % 10U); // tens digit
	xstr[1] = '0' + ((x / 100U) % 10U); // hundreds digit
	xstr[0] = '0' + ((x / 1000U) % 10U); // thousands digit

	
	Serial.println(xstr);      //Debug
	delay(2500);
	
	RFduinoGZLL.sendToHost(xstr);      //Sends data for the RFduino Master
	//Serial.write("Sended\n");        //Debug
	delay(500);
  

}



