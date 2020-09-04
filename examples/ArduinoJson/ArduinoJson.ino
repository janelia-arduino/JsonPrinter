#include <Arduino.h>
#include <Streaming.h>
#include <Array.h>
#include <Vector.h>
#include <ConstantVariable.h>
#include <ArduinoJson.h>
#include <JsonStream.h>


char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";


const long BAUD = 115200;

JsonStream json_stream(Serial);
StaticJsonDocument<200> json_document;

unsigned int print_count = 0;

void setup()
{
  Serial.begin(BAUD);
  DeserializationError error = deserializeJson(json_document,json);
  if (error)
  {
    Serial << "deserializeJson() failed with code " << error.c_str() << "\n";
    return;
  }
}

void loop()
{
  delay(2000);

  JsonObject root_object = json_document.as<JsonObject>();
  JsonArray data_array = root_object["data"];

  if ((print_count % 2) == 0)
  {
    json_stream.setPrettyPrint();
  }
  else
  {
    json_stream.setCompactPrint();
  }

  json_stream.beginObject();

  json_stream.write("json_object",root_object);

  // newline automatically disabled when inside an object
  json_stream.writeNewline();

  // you can either use write
  json_stream.write("json_array",data_array);

  // or you can use writeJson
  json_stream.writeJson("json_array2",data_array);

  json_stream.endObject();

  json_stream.writeNewline();

  ++print_count;
}
