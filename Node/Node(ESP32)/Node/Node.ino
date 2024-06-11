#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <SPI.h>              
#include <LoRa.h>
#include "image.h"

// RTOS
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

// Define two tasks 
void TaskWebServer( void *pvParameters );
void TaskUart( void *pvParameters );

HardwareSerial SerialPort(2);

// define the number of bytes you want to access
#define EEPROM_SIZE 6

// Lora
const long frequency = 433E6;  // LoRa Frequency

const int csPin = 5;          // LoRa radio chip select
const int resetPin = 14;        // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin

// Wifi
/* điền SSID & Password của ESP32*/
const char* ssid = "Truong"; 
const char* password = "05022003";
int c = 0;
int storeT = 0;
int storeH = 0;
int storeP = 0;
String msg = "";
String timeT = "5";
String timeH = "5";
String timeP = "5";
String input = "";

int sensor1_on_off = 1;
int sensor2_on_off = 1;
        
/* Gắn cho ESP32 IP tĩnh */
IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
// IPAddress local_ip(192,168,19,1);
// IPAddress gateway(192,168,19,1);
IPAddress subnet(255,255,255,0);
WebServer server(80);

String SendHTML();

void setup() {
  Serial.begin(115200);
  SerialPort.begin(19200, SERIAL_8N1, 16, 17);
  SerialPort.setRxTimeout(1000);

  // Lora
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(frequency)) {
    Serial.println("LoRa init failed. Check your connections.");
    // while (true);                       // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");
  Serial.println();
  LoRa_txMode();

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", handle_OnConnect);
  server.on("/sensor1on", handle_sensor1on);
  server.on("/sensor1off", handle_sensor1off);
  server.on("/sensor2on", handle_sensor2on);
  server.on("/sensor2off", handle_sensor2off);
  server.on("/get", HTTP_GET, handleInput);
  server.on("/ImgSensor1.jpg", handleSensor1);
  server.on("/ImgSensor2.jpg", handleSensor2);
  server.on("/Logo.jpg", handleLogo);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  xTaskCreatePinnedToCore(
  TaskWebServer
  ,  "Task WebServer" // A name just for humans
  ,  2048        // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
  ,  NULL // Task parameter which can modify the task behavior. This must be passed as pointer to void.
  ,  1 // Priority
  ,  NULL // Task handle is not used here - simply pass NULL
  , 0);

  xTaskCreatePinnedToCore(
  TaskUart
  ,  "Task UART" // A name just for humans
  ,  2048        // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
  ,  NULL // Task parameter which can modify the task behavior. This must be passed as pointer to void.
  ,  2  // Priority
  ,  NULL // Task handle is not used here - simply pass NULL
  , 1);
}
void loop() {

}
// Web server
void handleSensor1(){
  server.send_P(200, "image/gif", ImgSensor1, sizeof(ImgSensor1));
}
void handleSensor2(){
  server.send_P(200, "image/gif", ImgSensor2, sizeof(ImgSensor2));
}
void handleLogo(){
  server.send_P(200, "image/gif", Logo, sizeof(Logo));
}
void handleInput() {
  input = server.arg("input");
  if(c == 0){
    c += 1;
  }
  else{
  int sT = 0;
  int sH = 0;
  int sP = 0;
  timeT = "";
  timeP = "";
  timeH = "";
  msg = 's' + input + 'e'; // sT20/H10/P5e
  // Handle so lieu tren web
  for(int index = 0; index < msg.length(); index++){
    if(sT == 1 && (msg[index] >= '0' && msg[index] <= '9')){
      if(timeT == 0){
        timeT = msg[index];
      }
      else{
        timeT += msg[index];
      }
    }
    if(sP == 1 && (msg[index] >= '0' && msg[index] <= '9')){
      if(timeP == 0){
        timeP = msg[index];
      }
      else{
        timeP += msg[index];
      }
    }
    if(sH == 1 && (msg[index] >= '0' && msg[index] <= '9')){
      if(timeH == 0){
        timeH = msg[index];
      }
      else{
        timeH += msg[index];
      }
    }
    
    
    if(msg[index] == 'T'){
      sT = 1;
      sP = 0;
      sH = 0;
    }
    else if(msg[index] == 'P'){
      sT = 0;
      sP = 1;
      sH = 0;
    }
    else if(msg[index] == 'H'){
      sT = 0;
      sP = 0;
      sH = 1;
    }
  }
  Serial.println("Input received: " + input);
  // Gui uart cho stm32
  if(timeT.toInt() >= 5 && timeH.toInt() >= 5 && timeP.toInt() >= 5){
    LoRa_sendMessage(msg);
    for(char ch : msg){
      SerialPort.write(ch);
    }
  }
    c = 0;
    server.send(200, "text/html", SendHTML());
  }

  
}
void handle_OnConnect() {
  if(c == 0){
    c += 1;
  }
  else{
    Serial.println("Connect !");
    c = 0;
    
  }
  server.send(200, "text/html", SendHTML()); 
}
void handle_sensor1on() {
  if(c == 0){
    c += 1;
  }
  else{
    
  }
  msg = "sB1e";
  LoRa_sendMessage(msg);
  Serial.println("Sensor 1 on !");
  for(char ch : msg){
    SerialPort.print(ch);
  }
  c = 0;
  sensor1_on_off = 1;
  
  server.send(200, "text/html", SendHTML()); 
}
void handle_sensor1off() {
  if(c == 0){
    c += 1;
  }
  else{
    
  }
  msg = "sT1e";
  LoRa_sendMessage(msg);
  Serial.println("Sensor 1 off !");
  for(char ch : msg){
    SerialPort.print(ch);
  }
  c = 0;
  sensor1_on_off = 0;
  server.send(200, "text/html", SendHTML()); 
}
void handle_sensor2on() {
  if(c == 0){
    c += 1;
  }
  else{

  }
  msg = "sB2e";
  LoRa_sendMessage(msg);
  Serial.println("Sensor 2 on !");
  for(char ch : msg){
    SerialPort.print(ch);
  }
  c = 0;
  sensor2_on_off = 1;
  server.send(200, "text/html", SendHTML()); 
}
void handle_sensor2off() {
  if(c == 0){
    c += 1;
  }
  else{

  }
  msg = "sT2e";
  LoRa_sendMessage(msg);
  Serial.println("Sensor 2 off !");
  for(char ch : msg){
    SerialPort.print(ch);
  }
  c = 0;
  sensor2_on_off = 0;
  
  server.send(200, "text/html", SendHTML());
  
}
void handle_NotFound(){
  server.send(404, "text/plain", "Page Not found");
}

String SendHTML() {
  String htmlContent = "<!DOCTYPE html>\n";
  htmlContent += "<html lang=\"en\">\n";
  htmlContent += "<head>\n";
  htmlContent += "<meta charset=\"UTF-8\">\n";
  htmlContent += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  htmlContent += "<link href=\"https://fonts.googleapis.com/css2?family=Lato:ital,wght@0,300;0,400;0,700;0,900;1,300;1,400;1,700;1,900&display=swap\" rel=\"stylesheet\">\n";
  htmlContent += "<link rel=\"stylesheet\" type=\"text/css\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css\">\n";
  htmlContent += "<style>\n";
  htmlContent += "@import url(\"https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;500;600;700;800;900&display=swap\");\n";
  htmlContent += "*{padding:0;margin:0;text-decoration:none;list-style:none;box-sizing:border-box;font-family:\"Lato\",sans-serif;scroll-behavior:smooth}\n";
  htmlContent += "body{background:url(img/backgound.jpg);background-size:cover;background-position:center}\n";
  htmlContent += "header{position:fixed;height:30vh;width:100%;padding:20px 90px;background:white;display:flex;justify-content:space-between;align-items:center;z-index:100;border-top-right-radius:10px;border-top-left-radius:10px;overflow:hidden}\n";
  htmlContent += ".logo{font-size:32px;color:#fff;text-decoration:none;font-weight:700}.nav_link a{position:relative;font-size:18px;color:black;font-weight:500;text-decoration:none;margin-left:40px;transition:.3s}\n";
  htmlContent += ".nav_link a::before{content:\"\";position:absolute;top:100%;left:0;width:0;height:3px;background:#149279}\n";
  htmlContent += ".nav_link a:hover::before{width:100%;transition:.5s}section{height:auto;background:transparent;padding-top:10vh;overflow:hidden}\n";
  htmlContent += ".container{margin:0 auto;width:80%;padding:15px 9%;padding-top:200px}\n";
  htmlContent += ".container .heading{font-family:'Arial',sans-serif;font-size:36px;color:#333;text-align:center;text-shadow:2px 2px 4px rgba(0,0,0,.3);background:linear-gradient(to right,#f09819,#edde5d);-webkit-background-clip:text;-webkit-text-fill-color:transparent;letter-spacing:2px;text-transform:uppercase;text-align:center;padding-bottom:15px;padding-top:15px;color:#fff;text-shadow:0 5px 10px rgba(0,0,0,.2);font-size:50px}\n";
  htmlContent += ".container .box-specs{display:grid;grid-template-columns:repeat(auto-fit,minmax(270px,1fr));gap:15px}\n";
  htmlContent += ".container .box-specs .box{box-shadow:0 5px 10px rgba(0,0,0,.2);border-radius:5px;background:#fff;text-align:center;padding:30px 20px}\n";
  htmlContent += ".container .box-specs .box img { height: 80px; }\n";
  htmlContent += ".container .box-specs .box h3 { color: #444; font-size: 22px; padding: 10px 0; }\n";
  htmlContent += ".container .box-specs .box p { color: #777; font-size: 15px; line-height: 1.8; }\n";
  htmlContent += ".container .box-specs .box .btn { margin-top: 10px; display: inline-block; background: #333; color: #fff; font-size: 17px; border-radius: 5px; padding: 8px 25px; }\n";
  htmlContent += ".container .box-specs .box .btn:hover { letter-spacing: 1px; }\n";
  htmlContent += ".container .box-specs .box:hover { box-shadow: 0 10px 15px rgba(0,0,0,.3); transform: scale(1.03); }\n";
  htmlContent += ".container .box-control { padding-top: 30px; margin: 0 auto; display: grid; grid-template-columns: repeat(auto-fit, minmax(270px, 1fr)); gap: 15px; display: flex; justify-content: center; }\n";
  htmlContent += ".container .box-control .box { min-width: 200px; max-width: 400px; width: 40%; box-shadow: 0 5px 10px rgba(0,0,0,.2); border-radius: 5px; background: #fff; text-align: center; padding: 30px 20px; }\n";
  htmlContent += ".container .box-control .box img { height: 80px; }\n";
  htmlContent += ".container .box-control .box h3 { color: #444; font-size: 22px; padding: 10px 0; }\n";
  htmlContent += ".wrapper { width: 450px; background: #fff; padding: 30px; box-shadow: 0px 0px 10px rgba(0,0,0,0.1); margin: 0 auto; }\n";
  htmlContent += ".wrapper h4 { margin-bottom: 30px; }\n";
  htmlContent += ".wrapper .input-data { height: 40px; width: 100%; position: relative; border: none; }\n";
  htmlContent += ".wrapper .input-data input { height: 100%; width: 100%; border: none; font-size: 17px; border-bottom: 2px solid silver; }\n";
  htmlContent += ".wrapper .input-data input:focus { outline: none; }\n";
  htmlContent += ".wrapper .input-data label { position: absolute; bottom: 10px; left: 0; color: grey; pointer-events: none; transition: all 0.3s ease; }\n";
  htmlContent += ".input-data input:focus ~ label, .input-data input:valid ~ label { transform: translateY(-20px); font-size: 15px; color: #4158d0; }\n";
  htmlContent += ".wrapper .input-data .underline { position: absolute; bottom: 0px; height: 2px; width: 100%; }\n";
  htmlContent += ".input-data .underline:before { position: absolute; content: ''; height: 100%; width: 100%; background: #4158d0; transform: scaleX(0); transition: transform 0.3s ease; }\n";
  htmlContent += ".input-data input:focus ~ .underline:before, .input-data input:valid ~ .underline:before { transform: scaleX(1); }\n";
  htmlContent += ".wrapper .btn {padding-bottom: 2%; padding-top: 2%; background-color: #007bff; color: #fff; border: none; text-decoration: none; border-radius: 5px; cursor: pointer; }\n";
  htmlContent += ".wrapper .btn:hover { background-color: #0056b3; }\n";
  htmlContent += ".wrapper .btn { transition: background-color 0.3s ease; }\n";
  htmlContent += "h3{text-align: center; padding-bottom: 2%; padding-top: 2%;}\n";
  htmlContent += "</style>\n";
  htmlContent += "<title>Document</title>\n";
  htmlContent += "</head>\n";
  htmlContent += "<body>\n";
  htmlContent += "<header>\n";
  htmlContent += "<a href=\"#\" class=\"logo\"><img src=\"Logo.jpg\" alt=\"\"></a>\n";
  htmlContent += "<nav>\n";
  htmlContent += "<div class=\"nav_link\">\n";
  htmlContent += "<a href=\"#Spec\">Spec</a>\n";
  htmlContent += "<a href=\"#control\">Control</a>\n";
  htmlContent += "<a href=\"#Contact\">Contact Us</a>\n";
  htmlContent += "</div>\n";
  htmlContent += "</nav>\n";
  htmlContent += "</header>\n";
  htmlContent += "<div class=\"container\">\n";
  htmlContent += "<div class=\"box-control\">\n";
  htmlContent += "<div class=\"box\">\n";
  htmlContent += "<img src=\"ImgSensor1.jpg\" id=\"sensor1\"  alt=\"\">\n";
  htmlContent += "<h3>Cảm biến SHT31</h3>\n";
  htmlContent += "<a href=\"/sensor1on\" class=\"btn\" id=\"btnSensor1On\">ON</a>\n";
  htmlContent += "<a href=\"/sensor1off\" class=\"btn\" id=\"btnSensor1Off\">OFF</a>\n";
  htmlContent += "</div>\n";
  htmlContent += "<div class=\"box\">\n";
  htmlContent += "<img src=\"ImgSensor2.jpg\" id=\"sensor2\" alt=\"\">\n";
  htmlContent += "<h3>Cảm biến BMP180</h3>\n";
  htmlContent += "<a href=\"/sensor2on\" class=\"btn\" id=\"btnSensor2On\">ON</a>\n";
  htmlContent += "<a href=\"/sensor2off\" class=\"btn\" id=\"btnSensor2Off\">OFF</a>\n";
  htmlContent += "</div>\n";
  htmlContent += "</div>\n";
  htmlContent += "</div>\n";

  htmlContent += "<h3>T (Nhiệt độ):" + timeT + "s H (Độ ẩm):" + timeH + "s P (Áp suất):" + timeP + "s</h3>";


  htmlContent += "<div class=\"wrapper\">\n";
  htmlContent += "<form action=\"/get\" method=\"GET\">\n";
  htmlContent += "<div class=\"input-data\">\n";
  htmlContent += "<input type=\"text\" id=\"input\" name=\"input\" required>\n";
  htmlContent += "<label for=\"input\">Lệnh:</label>\n";
  htmlContent += "<div class=\"underline\"></div>\n";
  htmlContent += "</div>\n";
  htmlContent += "<br><br>\n";
  htmlContent += "<input type=\"submit\" value=\"Nhập\" class=\"btn\">\n";
  htmlContent += "</form>\n";
  htmlContent += "</div>\n";

  htmlContent += "<h3>Input received: " + input + "\n</h3>" + "<br>";

  htmlContent += "</body>\n";
  htmlContent += "</html>\n";


  return htmlContent;
}

// Lora
void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(String message) {
  // LoRa_txMode();                        // set tx mode'
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
  delay(50);
}


void TaskWebServer( void *pvParameters ){
  (void) pvParameters;
  while(1){
    server.handleClient();
  }
  
}
void TaskUart( void *pvParameters ){
  (void) pvParameters;
  while(1){
    if(sensor1_on_off != 0 || sensor2_on_off != 0){
      String uart_receive = "";
      if(SerialPort.available()){
        uart_receive = SerialPort.readString();
        Serial.println(uart_receive);
        LoRa_sendMessage(uart_receive);
      }
    }
    else{
      String uart_receive = "";
      uart_receive = SerialPort.readString();
      Serial.println(uart_receive);
    }
  }
}