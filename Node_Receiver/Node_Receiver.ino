//************************************************************
// this is a simple example that uses the painlessMesh library to send Button status and to receive Sensor's Data
//
// this code was edited by Sachin Soni for techiesms YouTube channel.
// visit the channel to see the project
// http://www.youtube.com/techiesms
//
//************************************************************


// Necessary Libraries
#include "painlessMesh.h"
#include <ArduinoJson.h>


// WiFi Credentials
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

//Variables


//Pin Declaration


Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;


// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() 
{
  

// Serializing in JSON Format 
  DynamicJsonDocument doc(1024);
  String msg ;
  serializeJson(doc, msg);
  // msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  Serial.println(msg);
  taskSendMessage.setInterval((TASK_SECOND * 1));
}
// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {

  String json;
  DynamicJsonDocument doc(1024);
  json = msg.c_str();
  DeserializationError error = deserializeJson(doc, json);

  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }

   String MQ131a = doc["MQ131a"];
   String MQ7a = doc["MQ7a"];
   String PM10a = doc["PM10a"];

   String MQ131b = doc["MQ131b"];
   String MQ7b = doc["MQ7b"];
   String PM10b = doc["PM10b"];

   String MQ131g = doc["MQ131g"];
   String MQ7g = doc["MQ7g"];
   String PM10g = doc["PM10g"];

   Serial.print(",");
   
   Serial.print(MQ131a);
   Serial.print(",");
   Serial.print(MQ7a);
   Serial.print(",");
   Serial.print(PM10a);
   Serial.print(",");

   Serial.print(MQ131b);
   Serial.print(",");
   Serial.print(MQ7b);
   Serial.print(",");
   Serial.print(PM10b);
   Serial.print(",");

   Serial.print(MQ131g);
   Serial.print(",");
   Serial.print(MQ7g);
   Serial.print(",");
   Serial.print(PM10g);
   Serial.println(",");


  // Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  Serial.begin(115200);

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
