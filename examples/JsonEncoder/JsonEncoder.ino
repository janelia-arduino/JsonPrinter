#include <Arduino.h>
#include <Streaming.h>
#include <Array.h>
#include <Vector.h>
#include <ConstantVariable.h>
#include <ArduinoJson.h>
#include <JsonStream.h>


const long BAUDRATE = 115200;

JsonStream json_stream(Serial);

CONSTANT_STRING(constant_string,"I am a constant string!");
CONSTANT_STRING(constant_string_0,"..0..");
CONSTANT_STRING(constant_string_1,"..1..");
CONSTANT_STRING(constant_string_2,"..2..");
CONSTANT_STRING(constant_string_3,"..3..");
const ConstantString constant_string_array[] =
  {
    constant_string_0,
    constant_string_1,
    constant_string_2,
    constant_string_3,
  };
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

  // all valid json must begin with an object or array
  json_stream.beginObject();

  // you can write key/value pairs together
  const int months = 12;
  json_stream.write("months",months);

  // or you can write key/value pairs separately
  json_stream.writeKey(constant_string_key);
  json_stream.write(13);

  // write key with null value
  json_stream.writeNull("empty");

  int test = 123;
  json_stream.write("test",test);
  json_stream.write("(bool)test",(bool)test);

  bool tested = true;
  json_stream.write("tested",tested);

  // write key separately when the value is an array or object
  json_stream.writeKey("another_object");
  json_stream.beginObject();

  json_stream.writeKey("count");
  json_stream.beginArray();
  json_stream.write(1);
  json_stream.write(2);
  json_stream.write(3);
  json_stream.endArray();

  json_stream.write("string",String("red rover"));
  json_stream.write("chars","jump over");

  // after beginning an object or an array do not forget to end it
  json_stream.endObject();

  json_stream.write("constant_string",constant_string);

  json_stream.write("constant_string_array",constant_string_array);

  json_stream.writeKey("String array");
  json_stream.beginArray();
  json_stream.write(String("A"));
  json_stream.write(String("B"));
  json_stream.write(String("C"));
  json_stream.endArray();

  json_stream.writeKey("char array array");
  json_stream.beginArray();
  json_stream.write("X");
  json_stream.write("Y");
  json_stream.write("Z");
  json_stream.endArray();

  json_stream.write(constant_string_key,"yep!");
  json_stream.write(const_string_key,"yep!!");

  json_stream.write("baudrate",BAUDRATE);

  json_stream.write("constant_string_ptr",constant_string_ptr);

  double pi = 3.141592653589;
  json_stream.write("pi_default_prec",pi);
  json_stream.writeDouble("pi_2_digit_prec",pi,2);

  json_stream.writeKey("json_char_write");
  int len = strlen(json);
  for (unsigned int i=0;i<len;++i)
  {
    json_stream.writeChar(json[i]);
  }

  json_stream.writeKey("json_byte_write");
  len = strlen(json);
  for (unsigned int i=0;i<len;++i)
  {
    json_stream.writeByte(json[i]);
  }

  json_stream.endObject();

  json_stream.writeNewline();
  json_stream.writeNewline();

  json_stream.beginArray();
  json_stream.write(1);
  json_stream.write(2);
  // make sure newlines are automatically disabled when inside an array
  json_stream.writeNewline();
  json_stream.write(3);
  json_stream.endArray();
  json_stream.writeNewline();

  ++print_count;
  delay(2000);
}
