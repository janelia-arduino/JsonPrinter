#include "Arduino.h"
#include "Streaming.h"
#include "Array.h"
#include "Flash.h"
#include "ConstantVariable.h"
#include "ArduinoJson.h"
#include "JsonStream.h"


char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

StaticJsonBuffer<200> jsonBuffer;

JsonObject& root = jsonBuffer.parseObject(json);

const char* sensor = root["sensor"];
long time = root["time"];
JsonArray& data = root["data"];
double latitude = root["data"][0];
double longitude = root["data"][1];


const unsigned int BAUDRATE = 9600;

JsonStream json_stream(Serial);

unsigned int print_count = 0;

void setup()
{
  Serial.begin(BAUDRATE);
}


void loop()
{
  if ((print_count % 2) == 0)
  {
    json_stream.setPrettyPrint();
  }
  else
  {
    json_stream.setCompactPrint();
  }

  json_stream.beginObject();

  json_stream.addKey("json_object");
  json_stream.add(&root);

  json_stream.addKey("json_array");
  json_stream.add(&data);

  json_stream.endObject();

  generic_serial.getStream() << "\n";

  ++print_count;
  delay(2000);
}
