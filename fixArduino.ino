#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno(3,2);

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define OUTPUT_PIN 5 // Define the pin for the output

// Define PID parameters
double Kp = 2, Ki = 5, Kd = 1;
double setpoint = 17.0; // Setpoint temperature

// Define variables for PID
double temperatureC2, output;
double integral, lastInput;
unsigned long lastTime;
double deltaTime = 1000; 
int power;

//NTC
const int sensorPin = A0;

//ACS
const int ampPin = A1; 
const float sensitivity = 0.066; 


OneWire oneWire(4); 
DallasTemperature sensors(&oneWire);


LiquidCrystal_I2C lcd(0x27, 20, 4); 

unsigned long previousMillis = 0;
const long interval = 500;


float prevTemperatureC1 = 0;
float prevTemperatureC2 = 0;

// PID function
double ComputePID(double temperatureC2) {
  unsigned long now = millis();
  double error = setpoint - temperatureC2;
  integral += (error * (now - lastTime));
  double derivative = (temperatureC2 - lastInput) / (now - lastTime);

  output = Kp * error - Ki * integral - Kd * derivative; 

  lastInput = temperatureC2;
  lastTime = now;
  return output;
}

void setup(){
	
	Serial.begin(9600);
	ArduinoUno.begin(4800);

  // Inisialisasi sensor
  sensors.begin();

  // Inisialisasi LCD dengan modul backpack I2C
  lcd.init();
  lcd.backlight();

  // Set the pin mode for the output
  pinMode(OUTPUT_PIN, OUTPUT);

}

void loop(){
// Minta sensor untuk membaca suhu
  sensors.requestTemperatures();

  // Baca suhu dari sensor pertama dalam derajat Celsius
  float temperatureC1 = sensors.getTempCByIndex(0);

  // Baca suhu dari sensor kedua dalam derajat Celsius
  float temperatureC2 = sensors.getTempCByIndex(1);

  double output = ComputePID(temperatureC2); // Compute PID

  // Map the PID output to the appropriate range
  output = constrain(output, 0, 255);
  analogWrite(OUTPUT_PIN, output);

  delay(deltaTime); // Delay for the specified sampling time

  power = output / 255 * 100;


  //NTC 100K
  int sensorValue = analogRead(sensorPin); // read analog value sensor
  
  // conversion analog reading to temperature in Celsius
  float resistance = (1023.0 / sensorValue) - 1.0;
  resistance = 10000.0 / resistance;
  float heat = log(resistance / 10000.0) / 3950.0;
  heat += 1.0 / (25 + 273.15); // 25 derajat Celcius
  heat = 1.0 / heat - 273.15; // conversion Celcius

  //ACS
  int ampValue = analogRead(ampPin); // Baca nilai analog dari sensor
  float voltage = ampValue * (5.0 / 1023.0); // Konversi nilai analog menjadi tegangan

  // calculate ampere
  float current = (voltage - 2.5) / sensitivity;
  float amps = 12 * current;

  //LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CHL Temp: ");
  lcd.print(temperatureC1);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("AQR Temp: ");
  lcd.print(temperatureC2);
  lcd.print(" C");

  lcd.setCursor(0, 3);
  lcd.print("PID PWR : ");
  lcd.print(power);
  lcd.print("%");

  lcd.setCursor(0, 2);
  lcd.print("HOT Temp: ");
  lcd.print(heat);
  lcd.print(" C");
	
  // sending data to ESP8266
  ArduinoUno.print("sensor 1:");
  ArduinoUno.println(temperatureC1);

  ArduinoUno.print("sensor 2:");
  ArduinoUno.println(temperatureC2);

  ArduinoUno.print("sensor 3:");
  ArduinoUno.println(power);

  ArduinoUno.print("sensor 4:");
  ArduinoUno.println(heat);

  ArduinoUno.print("sensor 5:");
  ArduinoUno.println(amps);

  delay(250);	
}
