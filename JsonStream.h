// ----------------------------------------------------------------------------
// JsonStream.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _JSON_STREAM_H_
#define _JSON_STREAM_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Streaming.h"
#include "Array.h"
#include "ConstantVariable.h"
#include "ArduinoJson.h"


class JsonDepthTracker
{
public:
  JsonDepthTracker();
  JsonDepthTracker(bool first_item, bool inside_object);
  bool first_item_;
  bool inside_object_;
};

class JsonStream
{
public:
  enum ResponseCodes
    {
      ERROR=0,
      SUCCESS=1,
    };
  enum JsonTypes
    {
      LONG_TYPE,
      DOUBLE_TYPE,
      BOOL_TYPE,
      NULL_TYPE,
      STRING_TYPE,
      OBJECT_TYPE,
      ARRAY_TYPE,
    };
  static const uint8_t STRING_LENGTH_DOUBLE=36;
  static const uint8_t DOUBLE_DIGITS_DEFAULT=6;
  static const char EOL='\n';

  JsonStream(Stream &stream);
  void setStream(Stream &stream);

  // encoder methods
  void beginObject();
  void endObject();
  void beginArray();
  void endArray();
  void setCompactPrint();
  void setPrettyPrint();
  template<typename N>
  void writeName(N name);
  template<typename T>
  void write(T value);
  template<typename T>
  void writeDouble(T value, unsigned char prec=DOUBLE_DIGITS_DEFAULT);
  template<typename N, typename T>
  void write(N name, T value)
  {
    writeName(name);
    write(value);
  }
  template<typename N, typename T>
  void writeDouble(N name, T value, unsigned char prec=DOUBLE_DIGITS_DEFAULT)
  {
    writeName(name);
    writeDouble(value,prec);
  }
  void writeNull();
  template<typename N>
  void writeNull(N name)
  {
    writeName(name);
    writeNull();
  }
  template<typename T>
  void writeJson(T value);
  template<typename N, typename T>
  void writeJson(N name, T value)
  {
    writeName(name);
    writeJson(value);
  }
  void writeNewline();
  void writeChar(char c);
  void writeByte(byte b);

  // decoder methods
  int available();
  int readJsonIntoBuffer(char *buffer, int buffer_size);
private:
  static const uint8_t RESPONSE_DEPTH_MAX=8;
  static const uint8_t RESPONSE_INDENT=2;
  Stream *stream_ptr_;
  bool pretty_print_;
  byte indent_level_;
  bool writing_;
  Array<JsonDepthTracker,RESPONSE_DEPTH_MAX> depth_tracker_;
  void indent();
  void endItem();
  void endArrayItem();
};

#endif
