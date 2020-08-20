#include <Streaming.h>
#include <JsonStream.h>
#include <ArduinoJson.h>

namespace constants
{
enum{STRING_LENGTH_REQUEST=257};
const long BAUD = 115200;
}

char request[constants::STRING_LENGTH_REQUEST];
JsonStream json_stream(Serial);


void setup()
{
  Serial.begin(constants::BAUD);
}

void loop() {
  if (json_stream.available() > 0)
  {
    Serial << "available = " << json_stream.available() << "\n";
    long bytes_read = json_stream.readJsonIntoBuffer(request);
    Serial << "bytes_read = " << bytes_read << "\n";
    if (bytes_read > 0)
    {
      Serial << "request = " << request << "\n";
    }
  }
}
