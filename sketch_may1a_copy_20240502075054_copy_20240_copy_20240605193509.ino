#include <Wire.h>
#include <WiFi.h>
#include <MPU6050.h>
#include "Arduino_SensorKit.h"

const char* ssid = "PocoX3";
const char* password = "123456789";

WiFiServer server(80);

MPU6050 mpu;
int16_t repsCount = 0;
int16_t gainsCount = 0;
int16_t setsCount = 0;
bool lift = false;
int16_t accelerometerX;
String reps = "Reps = ";
String gains = "Gains = ";
String sets = "Sets =";
void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(38400);
  digitalWrite(9, HIGH);
  delay(3000);
  digitalWrite(9, LOW);
  Serial.println("Connecting to WiFi network...");
  digitalWrite(8, HIGH);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(8, HIGH);
    delay(500);
    digitalWrite(8, LOW);
    delay(500);
    Serial.println("Connecting to WiFi network...");
  }
  digitalWrite(8, LOW);
  digitalWrite(11, HIGH);
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");
  Wire.setClock(400000);
  Wire.begin();
  mpu.initialize();
  Oled.begin();
  tone(10, 1000, 100);
  Oled.clearDisplay();
  Oled.setFlipMode(false);
  delay(1000);
  digitalWrite(11, LOW);
  delay(1000);
  //mpu.CalibrateAccel();
}

void loop() {
  WiFiClient client = server.available();
  if(client) {
    Serial.println("New client connected");
    // Read the request from the client
    String request = client.readStringUntil('\r');
    Serial.println("Request: " + request);
    // Send response to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println();
    String r = String(repsCount);
    String g = String(gainsCount);
    String s = String(setsCount);
    String dataSend = "<"+r+">";
    client.print(dataSend);
    client.println();

    // Close the connection
    client.stop();
    //Serial.println("Client disconnected");
    //Serial.println("");
  }
  mpu.getMotion6(&accelerometerX, NULL, NULL, NULL, NULL, NULL);
  Oled.setFont(u8x8_font_chroma48medium8_r);
  Oled.setCursor(1, 0);
  Oled.print(reps);
  Oled.print(String(repsCount));
  Oled.setCursor(1, 1);
  Oled.print(gains);
  Oled.print(gainsCount);
  Oled.setCursor(1, 2);
  Oled.print(sets);
  Oled.print(setsCount);
  //Oled.setCursor(1, 2);
  //Oled.print("Angle: ");
  //Oled.print(int((((((float)accelerometerX/4096)/4)+0.07)*-1)*100));
  delay(20);
  //Serial.print("AccY = ");
  //Serial.println(accelerometerY);
  if((((float)accelerometerX/4096)+0.07)*-1 > 3.6 & lift == false){
    repsCount++;
    tone(10, 7000, 50);
    lift = true;
    if(lift){
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      if(repsCount%12==0 && repsCount>0)
        gainsCount++;
      if(repsCount%6==0 && repsCount>0)
        setsCount++;
    }
    Serial.print("Reps Count: ");
    Serial.println(repsCount);
  }else if((((float)accelerometerX/4096)+0.03)*-1 < 0 & lift == true){
    lift = false;
    if(!lift){
      digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
    }
  }
  delay(20);
}








/*#include <Arduino.h>
#include <TinyMPU6050.h>
MPU6050 mpu (Wire);

bool lift = false;
int repsCount = 0;

void setup()
{
  // Debug console
  Serial.begin(115200);

  mpu.Initialize();

  // Calibration
  mpu.Calibrate();
  Serial.println("Calibration complete!");
}

void loop()
{
  Serial.print("number of reps: ");
  Serial.println(mpu.GetRawAccY());
  int Xvalue = mpu.GetRawAccY();
  mpu.Execute();
  delay(1000);

  if(Xvalue > 10000 & lift == false){
    repsCount += 1;
    lift = true;
  }else if (Xvalue < -100 & lift == true)
    lift = false;

}*/