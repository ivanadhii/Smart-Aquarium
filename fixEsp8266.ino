#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

SoftwareSerial NodeMCU(D2,D3);

#define BLYNK_TEMPLATE_ID "FILL WITH YOUR BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "FILL WITH YOUR BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "FILL WITH YOUR BLYNK_AUTH_TOKEN"

char auth[] = "FILL WITH YOUR ";
char ssid[] = "YOUR WIFI SSID"; 
char pass[] = "YOUR WIFI PASSWORD"; 

String inputString = ""; 
float sensor1Value, sensor2Value, sensor3Value, sensor4Value, sensor5Value; 

void setup() {
  Serial.begin(9600);   
  NodeMCU.begin(4800);
  Blynk.begin(auth, ssid, pass); 

  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

}

void loop() {
  Blynk.run(); 
  Blynk.syncAll();
  
  while (NodeMCU.available()) {
    
    char inChar = (char)NodeMCU.read();
    inputString += inChar;

    // Jika karakter baru baris (\n) diterima, maka proses string tersebut
    if (inChar == '\n') {
      // Filter data dari string
      if (inputString.startsWith("sensor 1:")) {
        String valueStr = inputString.substring(9); // Mengambil nilai setelah "sensor 1: "
        sensor1Value = valueStr.toFloat(); // Mengubah nilai menjadi float
        Serial.print(sensor1Value);
        Serial.println(" | Sending Chiller Temperature");
        Blynk.virtualWrite(V0, sensor1Value);
      } else if (inputString.startsWith("sensor 2:")) {
        String valueStr = inputString.substring(9); // Mengambil nilai setelah "sensor 2: "
        sensor2Value = valueStr.toFloat(); // Mengubah nilai menjadi float
        Serial.print(sensor2Value);
        Serial.println(" | Sending Aquarium Temperature");
        Blynk.virtualWrite(V1, sensor2Value);
      } else if (inputString.startsWith("sensor 3:")) {
        String valueStr = inputString.substring(9); // Mengambil nilai setelah "sensor 3: "
        sensor3Value = valueStr.toFloat(); // Mengubah nilai menjadi float
        Serial.print(sensor3Value);
        Serial.println(" | Sending PID Value");
        Blynk.virtualWrite(V2, sensor3Value);
      } else if (inputString.startsWith("sensor 4:")) {
        String valueStr = inputString.substring(9); // Mengambil nilai setelah "sensor 4: "
        sensor4Value = valueStr.toFloat(); // Mengubah nilai menjadi float
        Serial.print(sensor4Value);
        Serial.println(" | Sending Heatsink Temperature");
        Blynk.virtualWrite(V3, sensor4Value);
      } else if (inputString.startsWith("sensor 5:")) {
        String valueStr = inputString.substring(9); // Mengambil nilai setelah "sensor 5: "
        sensor5Value = valueStr.toFloat(); // Mengubah nilai menjadi float
        Serial.print(sensor5Value);
        Serial.println(" | Sending Amps Value");
        Blynk.virtualWrite(V4, sensor5Value);
      }
      
      // Reset string untuk input berikutnya
      inputString = "";
    }
  }
}
