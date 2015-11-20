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
  void beginObject();
  void endObject();
  void beginArray();
  void endArray();
  void setCompactPrint();
  void setPrettyPrint();
  template<typename K>
  void addKey(K key);
  template<typename T>
  void add(T value);
  template<typename T>
  void addDouble(T value, unsigned char prec=DOUBLE_DIGITS_DEFAULT);
  template<typename K, typename T>
  void add(K key, T value)
  {
    addKey(key);
    add(value);
  }
  template<typename K, typename T>
  void addDouble(K key, T value, unsigned char prec=DOUBLE_DIGITS_DEFAULT)
  {
    addKey(key);
    addDouble(value,prec);
  }
  void addNull();
  template<typename K>
  void addNull(K key)
  {
    addKey(key);
    addNull();
  }
  void newline();
  void writeChar(char c);
  void writeByte(byte b);
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
