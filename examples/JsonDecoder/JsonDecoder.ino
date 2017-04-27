#include "Arduino.h"
#include "Streaming.h"
#include "Array.h"
#include "Vector.h"
#include "ConstantVariable.h"
#include "ArduinoJson.h"
#include "JsonStream.h"


const long BAUDRATE = 115200;
const long STRING_LENGTH_RESPONSE = 4000;
char result_target[] = "\"result\":";
char error_target[] = "\"error\":";

JsonStream json_stream(Serial1);

char response[STRING_LENGTH_RESPONSE];

void sendGoodRequest()
{
  json_stream.beginArray();
  json_stream.write("?");
  json_stream.endArray();
  json_stream.writeNewline();
}

void sendBadRequest()
{
  json_stream.beginArray();
  json_stream.write("X");
  json_stream.endArray();
  json_stream.writeNewline();
}

void setup()
{
  Serial.begin(BAUDRATE);
  Serial1.begin(BAUDRATE);

  json_stream.setCompactPrint();
}

void loop()
{
  long bytes_read;
  bool found;

  // test readJsonIntoBuffer with good request
  sendGoodRequest();

  bytes_read = json_stream.readJsonIntoBuffer(response);
  Serial << "good request bytes_read = " << bytes_read << "\n";
  Serial << "good request response = " << response << "\n\n";

  delay(1000);

  // test readJsonAndFind with something that should be found
  sendGoodRequest();

  found = json_stream.readJsonAndFind(result_target);
  Serial << "result_target = " << result_target << "\n";
  Serial << "good request found = " << found << "\n\n";

  delay(1000);

  // test readJsonAndFind with something that should not be found
  sendGoodRequest();

  found = json_stream.readJsonAndFind(error_target);
  Serial << "error_target = " << error_target << "\n";
  Serial << "good request found = " << found << "\n\n";

  delay(1000);

  // test readJsonIntoBuffer with bad request
  sendBadRequest();

  bytes_read = json_stream.readJsonIntoBuffer(response);
  Serial << "bad request bytes_read = " << bytes_read << "\n";
  Serial << "bad request response = " << response << "\n\n";

  delay(1000);

  // test readJsonAndFind with something that should not be found
  sendBadRequest();

  found = json_stream.readJsonAndFind(result_target);
  Serial << "result_target = " << result_target << "\n";
  Serial << "bad request found = " << found << "\n\n";

  delay(1000);

  // test readJsonAndFind with something that should be found
  sendBadRequest();

  found = json_stream.readJsonAndFind(error_target);
  Serial << "error_target = " << error_target << "\n";
  Serial << "bad request found = " << found << "\n\n";

  delay(1000);

  // test readJsonAndFind with const char *
  sendGoodRequest();

  const char result_target_const[] = "\"result\":";
  found = json_stream.readJsonAndFind(result_target_const);
  Serial << "result_target_const = " << result_target_const << "\n";
  Serial << "good request found = " << found << "\n\n";

  delay(1000);

  // test readJsonAndFind with String
  sendGoodRequest();

  String result_target_string = "\"result\":";
  found = json_stream.readJsonAndFind(result_target_string);
  Serial << "result_target_string = " << result_target_string << "\n";
  Serial << "good request found = " << found << "\n\n";

  delay(1000);

  // test readJsonAndFind with ConstantString
  sendGoodRequest();

  CONSTANT_STRING(result_target_constant_string,"\"result\":");
  found = json_stream.readJsonAndFind(result_target_constant_string);
  Serial << "result_target_constant_string = " << result_target_constant_string << "\n";
  Serial << "good request found = " << found << "\n\n";

  delay(1000);

  // test readJsonAndFind with ConstantString *
  sendGoodRequest();

  ConstantString * result_target_constant_string_ptr = &result_target_constant_string;
  found = json_stream.readJsonAndFind(result_target_constant_string_ptr);
  Serial << "result_target_constant_string_ptr = " << *result_target_constant_string_ptr << "\n";
  Serial << "good request found = " << found << "\n\n";

  delay(1000);

  // test readJsonAndFind with ConstantString const *
  sendGoodRequest();

  ConstantString const * result_target_constant_string_const_ptr = &result_target_constant_string;
  found = json_stream.readJsonAndFind(result_target_constant_string_const_ptr);
  Serial << "result_target_constant_string_const_ptr = " << *result_target_constant_string_const_ptr << "\n";
  Serial << "good request found = " << found << "\n\n";

  delay(1000);

  // test readJsonAndFind with ConstantString * const
  sendGoodRequest();

  ConstantString * const result_target_constant_string_ptr_const = &result_target_constant_string;
  found = json_stream.readJsonAndFind(result_target_constant_string_ptr_const);
  Serial << "result_target_constant_string_ptr_const = " << *result_target_constant_string_ptr_const << "\n";
  Serial << "good request found = " << found << "\n\n";

  delay(1000);

  // test readJsonAndFind with ConstantString const * const
  sendGoodRequest();

  ConstantString const * const result_target_constant_string_const_ptr_const = &result_target_constant_string;
  found = json_stream.readJsonAndFind(result_target_constant_string_const_ptr_const);
  Serial << "result_target_constant_string_const_ptr_const = " << *result_target_constant_string_const_ptr_const << "\n";
  Serial << "good request found = " << found << "\n\n";

  delay(1000);

}
