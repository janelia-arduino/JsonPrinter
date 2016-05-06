#include "Arduino.h"
#include "Streaming.h"
#include "Array.h"
#include "Vector.h"
#include "ConstantVariable.h"
#include "ArduinoJson.h"
#include "JsonStream.h"


const unsigned int BAUDRATE = 9600;

JsonStream json_stream(Serial);

// desired json:
// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038,2.01]}

// when saving JSON in a char array backslash escape string quotes
const char desired_json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038,2.01]}";

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

  // you can write key/value pairs
  json_stream.write("sensor","gps");

  // the key/value pairs can be many different types
  long time = 1351824120;
  json_stream.write(String("time"),time);

  // write key separately when the value is an array or object
  json_stream.writeKey("data");

  json_stream.beginArray();

  // write values without keys when inside an array
  double data_0 = 48.756080;
  json_stream.write(data_0);

  // write as many values as you want
  float data_1 = 2.302038;
  json_stream.write(data_1);

  // specify precision of doubles if you want
  float data_2 = 2.01;
  unsigned char double_digits = 2;
  json_stream.writeDouble(data_2,double_digits);

  // after beginning an object or an array do not forget to end it
  json_stream.endArray();

  // after beginning an object or an array do not forget to end it
  json_stream.endObject();

  // newlines can be written to the stream
  // but the writeNewline method is automatically disabled
  // if inside an array or object
  json_stream.writeNewline();

  Serial << "entire object written as string outside of any other objects:" << "\n";
  json_stream.write(desired_json);
  Serial << "\n";

  Serial << "keys and outer quotation marks are not written outside of objects:" << "\n";
  json_stream.write("desired_json",desired_json);
  Serial << "\n";

  Serial << "keys and outer quotation marks are written inside of objects:" << "\n";
  json_stream.beginObject();
  json_stream.write("desired_json",desired_json);
  json_stream.endObject();
  Serial << "\n";

  // you can write JSON stored as a string, but it is not validated in any way
  Serial << "JSON strings are never quoted:" << "\n";
  json_stream.beginObject();
  json_stream.writeJson("desired_json",desired_json);
  json_stream.endObject();
  Serial << "\n";

  // you can write c-style arrays
  int c_style_array[5] = {5,4,3,2,1};
  json_stream.beginObject();
  json_stream.write("c_style_array",c_style_array);
  json_stream.endObject();
  Serial << "\n";

  // you can write Arrays
  Array<int,5> array(c_style_array);
  json_stream.beginObject();
  json_stream.write("array",array);
  json_stream.endObject();
  Serial << "\n";

  // you can write Vectors
  Vector<int> vector;
  vector.setStorage(c_style_array,5);
  json_stream.beginObject();
  json_stream.write("vector",vector);
  json_stream.endObject();
  Serial << "\n";

  ++print_count;
  delay(2000);
}
