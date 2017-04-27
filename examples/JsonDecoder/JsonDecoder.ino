#include "Arduino.h"
#include "Streaming.h"
#include "Array.h"
#include "Vector.h"
#include "ConstantVariable.h"
#include "ArduinoJson.h"
#include "JsonStream.h"


const long BAUDRATE = 115200;
const long STRING_LENGTH_RESPONSE = 4000;

JsonStream json_stream(Serial1);

char response[STRING_LENGTH_RESPONSE];

void setup()
{
  Serial.begin(BAUDRATE);
  Serial1.begin(BAUDRATE);

  json_stream.setCompactPrint();
}

void loop()
{
  json_stream.beginArray();
  json_stream.write("?");
  json_stream.endArray();
  json_stream.writeNewline();

  long bytes_read = json_stream.readJsonIntoBuffer(response);
  Serial << "bytes_read = " << bytes_read << "\n";
  Serial << "response = " << response << "\n\n";

  delay(2000);
}
