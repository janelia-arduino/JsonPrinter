#include "Arduino.h"
#include "Streaming.h"
#include "Array.h"
#include "Flash.h"
#include "ConstantVariable.h"
#include "GenericSerial.h"
#include "ArduinoJson.h"
#include "JsonStream.h"


const unsigned int BAUDRATE = 9600;

JsonStream json_stream(Serial);

CONSTANT_STRING(constant_string,"I am a constant string!");
CONSTANT_STRING(constant_string_0,"..0..");
CONSTANT_STRING(constant_string_1,"..1..");
CONSTANT_STRING(constant_string_2,"..2..");
CONSTANT_STRING(constant_string_3,"..3..");
CONSTANT_STRING(constant_string_key,"constant_string");
const String const_string_key("constString");
ConstantString* constant_string_ptr = &constant_string;

char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
char json_pretty[] = "{\n  \"sensor\": \"gps\",\n  \"time\": 1351824120,\n  \"data\": [\n    48.756080,\n    2.302038\n  ]\n}";

unsigned int print_count = 0;

void setup()
{
  Serial.begin(BAUDRATE);

  json_stream.setPrettyPrint();
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

  json_stream.addKey(constant_string_key);
  json_stream.add(13);

  const int months = 12;
  json_stream.add("months",months);

  json_stream.addNull("empty");

  int test = 123;
  json_stream.add("test",test);
  json_stream.add("(bool)test",(bool)test);

  bool tested = true;
  json_stream.add("tested",tested);

  json_stream.addKey("another_object");
  json_stream.beginObject();

  json_stream.addKey("count");
  json_stream.beginArray();
  json_stream.add(1);
  json_stream.add(2);
  json_stream.add(3);
  json_stream.endArray();

  json_stream.add("string",String("red rover"));
  json_stream.add("chars","jump over");

  json_stream.endObject();

  json_stream.add("constant_string",constant_string);

  json_stream.addKey("constant_string_array");
  json_stream.beginArray();
  json_stream.add(constant_string_0);
  json_stream.add(constant_string_1);
  json_stream.add(constant_string_2);
  json_stream.add(constant_string_3);
  json_stream.endArray();

  json_stream.addKey("String array");
  json_stream.beginArray();
  json_stream.add(String("A"));
  json_stream.add(String("B"));
  json_stream.add(String("C"));
  json_stream.endArray();

  json_stream.addKey("char array array");
  json_stream.beginArray();
  json_stream.add("X");
  json_stream.add("Y");
  json_stream.add("Z");
  json_stream.endArray();

  json_stream.add("response",JsonStream::SUCCESS);
  json_stream.add("error",JsonStream::ERROR);

  json_stream.add(constant_string_key,"yep!");
  json_stream.add(const_string_key,"yep!!");

  json_stream.add("baudrate",BAUDRATE);

  json_stream.add("constant_string_ptr",constant_string_ptr);

  double pi = 3.141592653589;
  json_stream.add("pi_default_prec",pi);
  json_stream.addDouble("pi_2_digit_prec",pi,2);

  json_stream.addKey("json_char_write");
  int len = strlen(json);
  for (unsigned int i=0;i<len;++i)
  {
    json_stream.writeChar(json[i]);
  }

  json_stream.addKey("json_byte_write");
  len = strlen(json);
  for (unsigned int i=0;i<len;++i)
  {
    json_stream.writeByte(json[i]);
  }

  json_stream.endObject();

  json_stream.linefeed();
  json_stream.linefeed();

  ++print_count;
  delay(2000);
}
