/*******************************************************
*   IR_Receive ESP 32
*   Decodes a IR-remote controler and send the signals
*   to Home Assistant using MQTT
*   Using decoder Freenove/Freenove_IR_Lib_for_ESP32
*   (github)
*   2024-12-11 HATO
********************************************************/

//#define DEBUG   // This will print status messages

#include <WiFi.h>
#include <PubSubClient.h>
#include "Freenove_IR_Lib_for_ESP32.h"

#define ZERO  0xff6897
#define ONE   0xff30cf
#define TWO   0xff18e7
#define TREE  0xff7a85
#define FOUR  0xff10ef
#define FIVE  0xff38c7
#define SIX   0xff5aa5
#define SEVEN 0xff42bd
#define EIGHT 0xff4ab5
#define NINE  0xff52ad
#define POWER 0xffa25d
#define MODE  0xff629d
#define SPEEK 0xffe21d
#define PLAY  0xff22dd
#define REV   0xff02fd
#define FWD   0xffc23d
#define EQ    0xffe01f
#define VOL-  0xffa857
#define VOL+  0xff906f
#define RPT   0xff9867
#define USD   0xffb04f
#define REPEAT 0xffffff

#define wifi_ssid "xxxxxx"
#define wifi_password "xxxxxx"
#define mqtt_server "192.168.1.36"
#define mqtt_user "xxxxxx"
#define mqtt_password "xxxxxx"

#define state_zero    "remote/ir/input/zero"
#define state_one     "remote/ir/input/one"
#define state_two     "remote/ir/input/two"
#define state_tree    "remote/ir/input/tree"
#define state_four    "remote/ir/input/four"
#define state_five    "remote/ir/input/five"
#define state_six     "remote/ir/input/six"

#define HOSTNAME HA_IRremote   // WiFi name

uint32_t IR_code;

Freenove_ESP32_IR_Recv ir_recv(15); //IRpin ESP32-GPIO15

WiFiClient espClient;
PubSubClient client(espClient);
 

void setup_wifi(){
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(wifi_ssid);
  WiFi.hostname("HOSTNAME");
  WiFi.begin(wifi_ssid, wifi_password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi connected \n");
  Serial.print("IP adress: ");
  Serial.println(WiFi.localIP());
}


void reconnect(){
  // Loop until we are connected
  while(!client.connected()){
    Serial.print("Atempting MQTT connection ...");
    // Create random client ID
    String clientId = "MQTT_Relay-";
    clientId += String(random(0xffff),HEX);

    // Attempt to connect
    if(client.connect(clientId.c_str(), mqtt_user, mqtt_password)){
      Serial.println("connected");
    }else{
      Serial.print("failed, rc = ");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    
  }
} 

void callback(char* topic1,byte* payload, unsigned int length){
#ifdef  DEBUG
  Serial.print("Message arrived on topic: ");
  Serial.print(topic1);
  Serial.print(". Message: ");
#endif
}





void setup(){
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
}

void loop(){

  static bool ZeroActive, OneActive, TwoActive, TreeActive, FourActive, FiveActive, SixActive = false;

  if(!client.connected()){
    reconnect();
  }
  client.loop();

  ir_recv.task();
  if(ir_recv.nec_available()){
    IR_code =  ir_recv.data();
#if DEBUG    
    Serial.printf("IR Protocol:%s, IR_Code: %#x\r\n", ir_recv.protocol(), IR_code);
 #endif   
  }

  switch(IR_code){
    case ZERO:
      if(!ZeroActive){
        ZeroActive = true;
        client.publish(state_zero, "ON");
      }else{
        ZeroActive = false;
        client.publish(state_zero, "OFF");
      }
      break;
    case ONE:
      if(!OneActive){
        OneActive = true;
        client.publish(state_one, "ON");
      }else{
        OneActive = false;
        Serial.println("OnActive = FALSE");
        client.publish(state_one, "OFF");
      }
      break;
    case TWO:
      if(!TwoActive){
        TwoActive = true;
        client.publish(state_two, "ON");
      }else{
        TwoActive = false;
        client.publish(state_two, "OFF");
      }
      break;
    case TREE:
      if(!TreeActive){
        TreeActive = true;
        client.publish(state_tree, "ON");
      }else{
        TreeActive = false;
        client.publish(state_tree, "OFF");
      }
      break;
    case FOUR:
      if(!FourActive){
        FourActive = true;
        client.publish(state_four, "ON");
      }else{
        FourActive = false;
        client.publish(state_four, "OFF");
      }
      break;
    case FIVE:
      if(!FiveActive){
        FiveActive = true;
        client.publish(state_five, "ON");
      }else{
        FiveActive = false;
        client.publish(state_five, "OFF");
      }
      break; 
    case SIX:
      if(!SixActive){
        SixActive = true;
        client.publish(state_six, "ON");
      }else{
        SixActive = false;
        client.publish(state_six, "OFF");
      }
      break;               
    default:
      //
      break;
  }

}


