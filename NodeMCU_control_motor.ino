#include <ESP8266WiFi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Servo.h>


Servo myServo;

int servoPin = 2; //D4
int motorControl1_1 = 5 ; // fix this b4 run 
int motorControl1_2 = 4; // fix this b4 run 
int motorControl2_1 = 14; // fix this b4 run 
int motorControl2_2 = 12; // fix this b4 run 

const char *ssid = "God_AP";
const char *password = "God123456";



// Set the static IP configuration
// IPAddress ip(192, 168, 11, 4);
// IPAddress gateway(192, 168, 11, 1);
// IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80);

void setup() {
  myServo.attach(servoPin);
  pinMode(motorControl1_1, OUTPUT);
  pinMode(motorControl1_2, OUTPUT);
  pinMode(motorControl2_1, OUTPUT);
  pinMode(motorControl2_2, OUTPUT);
  Serial.begin(115200);
  
  // Set up the Access Point with static IP
  //WiFi.mode(WIFI_AP);
  //WiFi.softAPConfig(ip, gateway, subnet);
  //WiFi.softAP(ssid_ap, password_ap);
  //Serial.println("Access Point started");
  //Serial.print("IP Address: ");
  //Serial.println(WiFi.softAPIP());
  WiFi.begin(ssid,password);
  Serial.println("Connect to AP");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  // Start the server
  server.begin();
}

void moveServo(int angle) {
  int servoPosition = map(angle, 0, 180, 0, 180);
  myServo.write(servoPosition);
}

// Function to run the DC motor forward
void runDCMotor() {
  Serial.println("CW");
  // first motor
  digitalWrite(motorControl1_1,LOW);
  digitalWrite(motorControl1_2,HIGH);
  //second motor
  digitalWrite(motorControl2_1,LOW);
  digitalWrite(motorControl2_2,HIGH);
}

// Function to run the DC motor backward
void reverseDCMotor() {
  Serial.println("CCW");
  digitalWrite(motorControl1_1,HIGH);
  digitalWrite(motorControl1_2,LOW);
  digitalWrite(motorControl2_1,HIGH);
  digitalWrite(motorControl2_2,LOW);
}

// Function to stop the DC motor
void stopDCMotor() {
  Serial.println("Stop motors");
  digitalWrite(motorControl1_1,LOW);
  digitalWrite(motorControl1_2,LOW);
  digitalWrite(motorControl2_1,LOW);
  digitalWrite(motorControl2_2,LOW);
}

void loop() {
  // Check for incoming client connections
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    
    // Read data from the client
    while (client.connected()) {
      if (client.available()) {
        String data = client.readStringUntil('\n');
        Serial.println("Received data: " + data);

        // Parse the received data and split into degree, acc, and brk
        int degree, acc, brk;
        sscanf(data.c_str(), "%d_%d_%d", &degree, &brk, &acc);

        // Print the parsed values to the Serial monitor
        Serial.print("Degree: ");
        Serial.println(degree);
        Serial.print("Acceleration: ");
        Serial.println(acc);
        Serial.print("Brake: ");
        Serial.println(brk);

        moveServo(degree);

        if(acc==255 && brk==0){
          runDCMotor();
        }else if(brk==255 && acc==0){
          reverseDCMotor();
        }else{
          stopDCMotor();
        }
      }
    }

    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}
