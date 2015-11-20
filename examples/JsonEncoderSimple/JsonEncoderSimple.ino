#include "Arduino.h"
#include "Streaming.h"
#include "Array.h"
#include "Flash.h"
#include "ConstantVariable.h"
#include "ArduinoJson.h"
#include "JsonStream.h"


const unsigned int BAUDRATE = 9600;

JsonStream json_stream(Serial);

// desired json:
// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038,2.01]}

// when saving JSON in a char array backslash escape string quotes
char desired_json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038,2.01]}";

unsigned int print_count = 0;

void setup()
{
  Serial.begin(BAUDRATE);

  json_stream.setPrettyPrint();
}


void loop()
{
  Serial << "desired JSON:" << "\n" << desired_json << "\n";

  if ((print_count % 2) == 0)
  {
    json_stream.setPrettyPrint();
    Serial << "pretty printed encoded JSON result:" << "\n";
  }
  else
  {
    json_stream.setCompactPrint();
    Serial << "compact printed encoded JSON result:" << "\n";
  }

  // all valid json must begin with an object or array
  json_stream.beginObject();

  // you can add key/value pairs
  json_stream.add("sensor","gps");

  // the key/value pairs can be many different types
  long time = 1351824120;
  json_stream.add(String("time"),time);

  // add key separately when the value is an array or object
  json_stream.addKey("data");

  json_stream.beginArray();

  // add values without keys when inside an array
  double data_0 = 48.756080;
  json_stream.add(data_0);

  // add as many values as you want
  float data_1 = 2.302038;
  json_stream.add(data_1);

  // specify precision of doubles if you want
  float data_2 = 2.01;
  unsigned char double_digits = 2;
  json_stream.addDouble(data_2,double_digits);

  // when beginning an object or an array do not forget to end it
  json_stream.endArray();

  // when beginning an object or an array do not forget to end it
  json_stream.endObject();

  // newlines can be written to the stream
  // but the newline method is automatically disabled
  // if inside an array or object
  json_stream.newline();

  ++print_count;
  delay(2000);
}
