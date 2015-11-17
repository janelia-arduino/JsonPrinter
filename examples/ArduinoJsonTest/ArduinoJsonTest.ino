#include "Arduino.h"
#include "Streaming.h"
#include "Array.h"
#include "Flash.h"
#include "ConstantVariable.h"
#include "GenericSerial.h"
#include "ArduinoJson.h"
#include "JsonPrinter.h"


char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

StaticJsonBuffer<200> jsonBuffer;

JsonObject& root = jsonBuffer.parseObject(json);

const char* sensor = root["sensor"];
long time = root["time"];
JsonArray& data = root["data"];
double latitude = root["data"][0];
double longitude = root["data"][1];


const unsigned int BAUDRATE = 9600;

GenericSerial generic_serial(Serial);
JsonPrinter json_printer(generic_serial);

unsigned int print_count = 0;

void setup()
{
  generic_serial.begin(BAUDRATE);
}


void loop()
{
  if ((print_count % 2) == 0)
  {
    json_printer.setPrettyPrint();
  }
  else
  {
    json_printer.setCompactPrint();
  }

  json_printer.startObject();

  json_printer.addKey("json_object");
  json_printer.add(&root);

  json_printer.addKey("json_array");
  json_printer.add(&data);

  json_printer.stopObject();

  generic_serial.getStream() << "\n";

  ++print_count;
  delay(2000);
}
