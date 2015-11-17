#include "Arduino.h"
#include "Streaming.h"
#include "Array.h"
#include "Flash.h"
#include "ConstantVariable.h"
#include "GenericSerial.h"
#include "ArduinoJson.h"
#include "JsonPrinter.h"


const unsigned int BAUDRATE = 9600;

GenericSerial generic_serial(Serial);
JsonPrinter json_printer(generic_serial);

CONSTANT_STRING(constant_string,"I am a constant string!");
CONSTANT_STRING(constant_string_0,"..0..");
CONSTANT_STRING(constant_string_1,"..1..");
CONSTANT_STRING(constant_string_2,"..2..");
CONSTANT_STRING(constant_string_3,"..3..");
CONSTANT_STRING(constant_string_key,"constant_string");
const String const_string_key("constString");
ConstantString* constant_string_ptr = &constant_string;

unsigned int print_count = 0;

void setup()
{
  generic_serial.begin(BAUDRATE);

  json_printer.setPrettyPrint();
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

  json_printer.addKey(constant_string_key);
  json_printer.add(13);

  const int months = 12;
  json_printer.add("months",months);

  json_printer.addNull("empty");

  int test = 123;
  json_printer.add("test",test);
  json_printer.add("(bool)test",(bool)test);

  bool tested = true;
  json_printer.add("tested",tested);

  json_printer.addKey("another_object");
  json_printer.startObject();

  json_printer.addKey("count");
  json_printer.startArray();
  json_printer.add(1);
  json_printer.add(2);
  json_printer.add(3);
  json_printer.stopArray();

  json_printer.add("string",String("red rover"));
  json_printer.add("chars","jump over");

  json_printer.stopObject();

  json_printer.add("constant_string",constant_string);

  json_printer.addKey("constant_string_array");
  json_printer.startArray();
  json_printer.add(constant_string_0);
  json_printer.add(constant_string_1);
  json_printer.add(constant_string_2);
  json_printer.add(constant_string_3);
  json_printer.stopArray();

  json_printer.addKey("String array");
  json_printer.startArray();
  json_printer.add(String("A"));
  json_printer.add(String("B"));
  json_printer.add(String("C"));
  json_printer.stopArray();

  json_printer.addKey("char array array");
  json_printer.startArray();
  json_printer.add("X");
  json_printer.add("Y");
  json_printer.add("Z");
  json_printer.stopArray();

  json_printer.add("response",JsonPrinter::SUCCESS);
  json_printer.add("error",JsonPrinter::ERROR);

  json_printer.add(constant_string_key,"yep!");
  json_printer.add(const_string_key,"yep!!");

  json_printer.add("baudrate",BAUDRATE);

  json_printer.add("constant_string_ptr",constant_string_ptr);

  double pi = 3.141592653589;
  json_printer.add("pi_default_prec",pi);
  json_printer.add("pi_8_digit_prec",pi,8);

  json_printer.stopObject();

  json_printer.linefeed();
  json_printer.linefeed();

  ++print_count;
  delay(2000);
}
