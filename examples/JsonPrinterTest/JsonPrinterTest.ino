#include "Arduino.h"
#include "Streaming.h"
#include "Array.h"
#include "Flash.h"
#include "ConstantVariable.h"
#include "GenericSerial.h"
#include "JsonPrinter.h"


const unsigned int BAUDRATE = 9600;

GenericSerial generic_serial(Serial);
JsonPrinter json_printer(generic_serial);

CONSTANT_STRING(const_str,"I am a constant string!");
CONSTANT_STRING(const_str_0,"..0..");
CONSTANT_STRING(const_str_1,"..1..");
CONSTANT_STRING(const_str_2,"..2..");
CONSTANT_STRING(const_str_3,"..3..");

void setup()
{
  Serial.begin(BAUDRATE);

  json_printer.setPrettyPrint();
}


void loop()
{
  json_printer.startObject();

  const int months = 12;
  json_printer.add("months",months);

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

  json_printer.add("str",String("red rover"));
  json_printer.add("chars","jump over");

  json_printer.stopObject();

  json_printer.add("const_str",const_str);

  json_printer.addKey("const_str_array");
  json_printer.startArray();
  json_printer.add(const_str_0);
  json_printer.add(const_str_1);
  json_printer.add(const_str_2);
  json_printer.add(const_str_3);
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

  json_printer.add("DOUBLE_DIGITS",JsonPrinter::DOUBLE_DIGITS);
  json_printer.add("response",JsonPrinter::SUCCESS);

  json_printer.stopObject();

  generic_serial.getSerial() << endl;

  delay(2000);
}
