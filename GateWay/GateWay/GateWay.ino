#include <Arduino.h>
#include <WiFiManager.h>
// #include <FirebaseESP32.h>
#include <Firebase_ESP_Client.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>
// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
#include <SPI.h>              // include libraries
#include <LoRa.h>

// WiFi Manage
int timeout = 120; // seconds to run for
// WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
WiFiManager wm;

// Lora
const long frequency = 433E6;  // LoRa Frequency

const int csPin = 5;          // LoRa radio chip select
const int resetPin = 14;        // LoRa radio reset (Chân 22 của esp32)
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin
int avai = 0;
String valT = "";
String valH = "";
String valP = "";
String storeT = "";
String storeH = "";
String storeP = "";

String timeT = "5";
String timeH = "5";
String timeP = "5";
String store_timeT = "5";
String store_timeH = "5";
String store_timeP = "5";

String buffer[32];
int index_Mess = 0;
int current_index = 0;

// Firebase

#define FIREBASE_HOST "enviromentmoritering-default-rtdb.firebaseio.com/" //2 cái này lấy trong FB nhé
#define API_KEY "AIzaSyBQIn4S4QqTubmohR0n7DPo7Kpc-6sx--M"

#define USER_EMAIL "nhattruong05022003@gmail.com" // email của mình
#define USER_PASSWORD "truong05022003"



// const char* ssid = "NhatTruong"; 
// const char* password = "05022003";

// const char* ssid = "Gg"; 
// const char* password = "ggmedi123";

FirebaseData stream;
FirebaseData firebaseData;
FirebaseData firebaseData1;
FirebaseData firebaseData2;

FirebaseData firebaseData3;
FirebaseData firebaseData4;
FirebaseData firebaseData5;

FirebaseData firebaseData6;
FirebaseData firebaseData7;

FirebaseData firebaseData8;

FirebaseAuth auth;
FirebaseConfig config;

int tempVal = 20;
int humiVal = 60;
float pressVal = 1013.25;
long long timeTemp = 5000;
long long timeHumi = 5000;
long long timePress = 5000;
long long t1, t2, t3, t4;
int sensor1_on_off = 1;
int sensor2_on_off = 1;
String path = "/";

long long dummy;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Config wifi
  WiFi.mode(WIFI_STA);
    // Supress Debug information
  wm.setDebugOutput(false); // ko in cac dong debug
  // Remove any previous network settings
  wm.resetSettings();
  wm.setConfigPortalTimeout(timeout);
  if(!wm.startConfigPortal("WiFiManager")){
    Serial.println("Failed to connect and hit timeout");
    delay(1000);
    ESP.restart();
    delay(5000);
  }
  // Serial.print("IP address : ");
  // Serial.println(WiFi.localIP());

  // WiFi.begin(ssid, password);
  // Serial.print("Connecting to : ");
  // Serial.println(ssid);
  // while (WiFi.status() != WL_CONNECTED){
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println();

  config.host = FIREBASE_HOST;
  config.api_key = API_KEY;
  /* Assign the RTDB URL (required) */
  config.database_url = FIREBASE_HOST;
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD; 

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);

  firebaseData.setResponseSize(1024); // minimum size is 1024 bytes
  firebaseData1.setResponseSize(1024); // minimum size is 1024 bytes
  firebaseData2.setResponseSize(1024); // minimum size is 1024 bytes
  firebaseData3.setResponseSize(1024); // minimum size is 1024 bytes
  firebaseData4.setResponseSize(1024); // minimum size is 1024 bytes
  firebaseData5.setResponseSize(1024); // minimum size is 1024 bytes
  firebaseData6.setResponseSize(1024); // minimum size is 1024 bytes
  firebaseData7.setResponseSize(1024); // minimum size is 1024 bytes
  firebaseData8.setResponseSize(1024); // minimum size is 1024 bytes
  
  stream.setResponseSize(1024); // minimum size is 1024 bytes

  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Receiver");

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();

  valT = String(Firebase.RTDB.getInt(&firebaseData1, path + "Temp"));
  valH = String(Firebase.RTDB.getInt(&firebaseData, path + "Humi"));
  valP = String(Firebase.RTDB.getInt(&firebaseData2, path + "Press"));
}

void loop() {
  if(avai > 0){
    String message = buffer[current_index];
    int flag = 0;
    String temp = "";
    for(char ch : message){
      if(flag){
        temp += ch;
      }
      if(ch == 's'){
        flag = 1;
        temp += ch;
      }
      else if(ch == 'e'){
        break;
      }
    }
    if(flag == 1){
      message = temp;
      Serial.println(message);
      Serial.println(message.length());
      if(message.length() <= 5){
        if(message[1] == 'B'){
          if(message[2] == '1'){
            Firebase.RTDB.setInt(&firebaseData6, path + "sensor1", 1);
            // Firebase.RTDB.getInt(&firebaseData6, path + "sensor1");
            sensor1_on_off = 1;
          }
          else{
            Firebase.RTDB.setInt(&firebaseData7, path + "sensor2", 1);
            // Firebase.RTDB.getInt(&firebaseData7, path + "sensor2");
            sensor2_on_off = 1;
          }
        }
        else if(message[1] == 'T'){
          if(message[2] == '1'){
            Firebase.RTDB.setInt(&firebaseData6, path + "sensor1", 0);
            // Firebase.RTDB.getInt(&firebaseData6, path + "sensor1");
            sensor1_on_off = 0;
          }
          else{
            Firebase.RTDB.setInt(&firebaseData7, path + "sensor2", 0);
            // Firebase.RTDB.getInt(&firebaseData7, path + "sensor2");
            sensor2_on_off = 0;
          }
        }
      }
      else{
        int sT = 0;
        int sH = 0;
        int sP = 0;
        timeT = "";
        timeP = "";
        timeH = "";
        // Serial.println(message);
        for(int index = 0; index < message.length(); index++){
          if(sT == 1 && (message[index] >= '0' && message[index] <= '9')){
            if(timeT == 0){
              timeT = message[index];
            }
            else{
              timeT += message[index];
            }
          }
          if(sP == 1 && (message[index] >= '0' && message[index] <= '9')){
            if(timeP == 0){
              timeP = message[index];
            }
            else{
              timeP += message[index];
            }
          }
          if(sH == 1 && (message[index] >= '0' && message[index] <= '9')){
            if(timeH == 0){
              timeH = message[index];
            }
            else{
              timeH += message[index];
            }
          }
          
          
          if(message[index] == 'T'){
            sT = 1;
            sP = 0;
            sH = 0;
          }
          else if(message[index] == 'P'){
            sT = 0;
            sP = 1;
            sH = 0;
          }
          else if(message[index] == 'H'){
            sT = 0;
            sP = 0;
            sH = 1;
          }
        }
      }
      if(timeT == "")
        timeT = store_timeT;
      else{
        store_timeT = timeT;
      }
      if(timeH == "")
        timeH = store_timeH;
      else{
        store_timeH = timeH;
      }
      if(timeP == "")
        timeP = store_timeP;
      else{
        store_timeP = timeP;
      }
      Serial.println(timeT);
      Serial.println(timeH);
      Serial.println(timeP);
      timeTemp = timeT.toInt() * 1000;
      timeHumi = timeH.toInt() * 1000;
      timePress = timeP.toInt() * 1000;
      Firebase.RTDB.setInt(&firebaseData3, path + "timeTemp", timeTemp);
      Firebase.RTDB.getInt(&firebaseData3, path + "timeTemp");
      Firebase.RTDB.setInt(&firebaseData4, path + "timeHumi", timeHumi);
      Firebase.RTDB.getInt(&firebaseData4, path + "timeHumi");
      Firebase.RTDB.setInt(&firebaseData5, path + "timePress", timePress);
      Firebase.RTDB.getInt(&firebaseData5, path + "timePress");
    }
    else{
      int sT = 0;
      int sH = 0;
      int sP = 0;
      valT = "";
      valP = "";
      valH = "";
      // Serial.println(message);
      for(int index = 0; index < message.length(); index++){
        if(sT == 1 && (message[index] >= '0' && message[index] <= '9')){
          if(valT == 0){
            valT = message[index];
          }
          else{
            valT += message[index];
          }
        }
        if(sP == 1 && (message[index] >= '0' && message[index] <= '9')){
          if(valP == 0){
            valP = message[index];
          }
          else{
            valP += message[index];
          }
        }
        if(sH == 1 && (message[index] >= '0' && message[index] <= '9')){
          if(valH == 0){
            valH = message[index];
          }
          else{
            valH += message[index];
          }
        }
        
        
        if(message[index] == 'T'){
          sT = 1;
          sP = 0;
          sH = 0;
        }
        else if(message[index] == 'P'){
          sT = 0;
          sP = 1;
          sH = 0;
        }
        else if(message[index] == 'H'){
          sT = 0;
          sP = 0;
          sH = 1;
        }
      }
      if(valT == "" || valT.length() < 3)
        valT = storeT;
      else{
        storeT = valT;
      }
      if(valH == "" || valH.length() < 3)
        valH = storeH;
      else{
        storeH = valH;
      }
      if(valP == "" || valP.length() < 5)
        valP = storeP;
      else{
        storeP = valP;
      }
      Serial.println(valT);
      Serial.println(valH);
      Serial.println(valP);
    }
    current_index = current_index > 31 ? 0 : current_index + 1;
    avai --;
  }
    // put your main code here, to run repeatedly:
  if (Firebase.ready() && (millis() - t1 > timeTemp || t1 == 0) && sensor1_on_off == 1)
  {
    tempVal = valT.toInt() > 125 ? valT.toInt() / 100 : valT.toInt();
    if(tempVal <= 125)
      Firebase.RTDB.setInt(&firebaseData1, path + "Temp", tempVal);
    t1= millis();
  }
  if (Firebase.ready() && (millis() - t2 > timeHumi || t2 == 0) && sensor1_on_off == 1)
  {
    humiVal = valH.toInt() > 100 ? valH.toInt() / 100 : valH.toInt();
    if(humiVal <= 100)
      Firebase.RTDB.setInt(&firebaseData, path + "Humi", humiVal);
    t2= millis();
  }

  if (Firebase.ready() && (millis() - t3 > timePress || t3 == 0) && sensor2_on_off == 1)
  {
    pressVal = valP.toInt() / 100.0;
    if(pressVal <= 1100)
      Firebase.RTDB.setFloat(&firebaseData8, path + "Press", pressVal);
    t3= millis();
  }

  if (Firebase.ready() && (millis() - t4 > 2500 || t4 == 0))
  {
    int dummy = Firebase.RTDB.getInt(&firebaseData2, path + "Press");
    t4= millis();
  }
}

void LoRa_rxMode(){
  LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.enableInvertIQ();                // active invert I and Q signals
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void onReceive(int packetSize) {
  String message_receive = "";
  while (LoRa.available()) {
    message_receive += (char)LoRa.read();
  }
  buffer[index_Mess] = message_receive;
  index_Mess = index_Mess > 31 ? 0 : index_Mess + 1;
  avai ++;
}

void onTxDone() {
  Serial.println("TxDone");
  LoRa_rxMode();
}