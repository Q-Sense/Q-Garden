#include <LiquidCrystal.h>


#include "Wire.h"
#define address 0x4D            //TC74 sensor adress
#define valvePin 10             //Pin 10 defines the state of the eletric solenoid valve

byte val = 0;
const int botao = 6;
const int botao2 = 7;
const int botao3 = 8;
boolean troca = false;
boolean troca1 = false;
boolean troca2 = false;
boolean troca3 = false;

boolean change = false;
int hum_atm = 0;                    //Humidity at the moment (atm) received from the master
int temperature;                    //Temperature value received from the master
volatile int hum_value = 40;        //Humidity value introduced in the buttons: begins on 40%
volatile int count=0;
int onoff = 0;

//Initializes the LCD display and selects the inputs and outputs
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {

	Wire.begin(4);
	Serial.begin(9600);
	lcd.begin(16, 2);

	pinMode(botao, INPUT);
	pinMode(botao2, INPUT);
	pinMode(botao3, INPUT);
        pinMode(10, OUTPUT);
	digitalWrite(botao, HIGH);
	digitalWrite(botao2, HIGH);
	digitalWrite(botao3, HIGH);

}

void loop() {
	
	const unsigned long oneMinute = 0.1*60 * 1000UL;
	static unsigned long lastSampleTime = 0 - oneMinute;

	unsigned long now = millis();
		
	int estado = 0;
	int estado2 = 0;
	int estado3 = 0;

	//Reads the buttons
	estado = digitalRead(botao);
	estado2 = digitalRead(botao2);
	estado3 = digitalRead(botao3);

	//Controls the LCD menu

	if (estado2 == HIGH) {
		//delay(250);
		hum_value = hum_value - 5;          //Button to decrease the value of the threshold humidity (percentage of humidity set by the user)
		change = true;
	}
	if (estado3 == HIGH) {
		//delay(250);
		hum_value = hum_value + 5;          //Button to increase the value of the threshold humidity (percentage of humidity set by the user)
		change = true;
	}

	if (hum_value < 0){
		hum_value = 0;                      //Threshold value can't go bellow 0%
	}
	if (hum_value >= 100){
		hum_value = 100;                    //Threshold value can't go over 100%
		
	}
	 
//Prints the value of the threshold humidity defined in the buttons

                Wire.onRequest(requestEvent);     //Sends threshold humidity value to the master
		lcd.setCursor(0, 0);
		lcd.print("TG:");
		lcd.print(hum_value);            //Prints the threshold humidity in the LCD
                Serial.println("botoes:");       //Debug
                Serial.println(hum_value);
		lcd.print("%");

//Prints the humidity value received from the master
               
                Wire.onReceive(receiveEvent); //Receives humidity and temperature values from the master
	        
                lcd.setCursor(8, 0);
	        lcd.print("HUM:");            //Actual humidity received from master
	        Serial.println(hum_atm);      //Debug
                      
	        lcd.print(hum_atm);
	        lcd.print("%");

	      	
//Prints the temperature value received from the master 

		lcd.setCursor(0, 1);
		lcd.print("TEMP:");
		lcd.print(temperature);       //Actual temperature received from master
                Serial.println(temperature);  //Debug
		lcd.print((char)223);         //print graus symbol º

                lcd.setCursor(9, 1);              

//Turns ON or OFF the solenoid valve
              
                if(hum_atm <= hum_value){              //turns ON pin 10 so the solenoid valve can be activated
                    lcd.print("ON"); 
                    Serial.println("ON");              //Debug
                    digitalWrite(valvePin, HIGH);
                }
                else{                                 //turns OFF pin 10 so the solenoid valve can be deactivated
                    lcd.print("OFF");
                    Serial.println("OFF");            //Debug
                    digitalWrite(valvePin, LOW);
                }
                
		delay(250);
                change = true;
                
//Cleans the LCD screen

               if(change = true){
                   delay(150);
                   change = false;
                   lcd.clear();
               }

}

void requestEvent(){
  
    Wire.write(hum_value);            //Sends threshold humidity value to the master
  
}

void receiveEvent(int value){
    while(1 < Wire.available()){
      
      hum_atm = Wire.read();          //Receives humidity value from the master
      temperature = Wire.read();      //Receives temperature value from the master
      onoff = Wire.read();            
      
    }
      
}
