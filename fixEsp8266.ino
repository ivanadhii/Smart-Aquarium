#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

SoftwareSerial NodeMCU(D2,D3);

#define BLYNK_TEMPLATE_ID "TMPL6hM7iAwWf"
#define BLYNK_TEMPLATE_NAME "Axolotl Aquarium"
#define BLYNK_AUTH_TOKEN "J3925trk0yUL2PfKQUOTBG8DrWaf9nzs"

char auth[] = "J3925trk0yUL2PfKQUOTBG8DrWaf9nzs"; // Masukkan token otentikasi dari Blynk
char ssid[] = "construction"; // Masukkan nama jaringan WiFi Anda
char pass[] = "unavailable"; // Masukkan kata sandi WiFi Anda

String inputString = ""; // String untuk menyimpan data dari Serial
float sensor1Value, sensor2Value, sensor3Value, sensor4Value, sensor5Value; // Variabel untuk menyimpan nilai sensor

void setup() {
  Serial.begin(9600);   // Inisialisasi Serial Monitor
  NodeMCU.begin(4800);
  Blynk.begin(auth, ssid, pass); // Mulai koneksi Blynk dengan otentikasi dan kredensial WiFi

  // Inisialisasi koneksi WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

}

void loop() {
  Blynk.run(); // Memanggil Blynk.run di dalam loop
  Blynk.syncAll();
  
  while (NodeMCU.available()) {
    // Baca data dari Serial Monitor
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
