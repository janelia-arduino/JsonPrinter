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
#include "Vector.h"
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

  JsonStream();
  JsonStream(Stream &stream);
  void setStream(Stream &stream);
  Stream& getStream();

  // encoder methods
  void beginObject();
  void endObject();
  void beginArray();
  void endArray();
  void setCompactPrint();
  void setPrettyPrint();
  template<typename K>
  void writeKey(K key);
  template<typename T>
  void write(T value);
  template<typename T>
  void writeDouble(T value, unsigned char prec=DOUBLE_DIGITS_DEFAULT);
  template<typename K, typename T>
  void write(K key, T value)
  {
    writeKey(key);
    write(value);
  }
  template<typename K, typename T>
  void writeDouble(K key, T value, unsigned char prec=DOUBLE_DIGITS_DEFAULT)
  {
    writeKey(key);
    writeDouble(value,prec);
  }
  void writeNull();
  template<typename K>
  void writeNull(K key)
  {
    writeKey(key);
    writeNull();
  }
  template<typename T>
  void writeJson(T value);
  template<typename K, typename T>
  void writeJson(K key, T value)
  {
    writeKey(key);
    writeJson(value);
  }
  void writeNewline();
  void writeChar(char c);
  void writeByte(byte b);
  template <typename T, size_t N>
  void write(T (&values)[N])
  {
    if (stream_ptr_ != NULL)
    {
      endArrayItem();
      beginArray();
      for (int i=0;i<N;++i)
      {
        write(values[i]);
      }
      endArray();
    }
  }
  template <size_t N>
  void write(const char (&values)[N])
  {
    write<const char *>(values);
  }
  template <size_t N>
  void write(char (&values)[N])
  {
    write<char *>(values);
  }
  template <typename K, typename T, size_t N>
  void write(K key, T (&values)[N])
  {
    writeKey(key);
    write(values);
  }
  template <typename T, size_t N>
  void write(Array<T,N> values)
  {
    if (stream_ptr_ != NULL)
    {
      endArrayItem();
      beginArray();
      for (int i=0;i<N;++i)
      {
        write(values[i]);
      }
      endArray();
    }
  }
  template <typename T>
  void write(Vector<T> values)
  {
    if (stream_ptr_ != NULL)
    {
      endArrayItem();
      beginArray();
      for (int i=0;i<values.size();++i)
      {
        write(values[i]);
      }
      endArray();
    }
  }

  // decoder methods
  int available();
  int readJsonIntoBuffer(char buffer[], unsigned int buffer_size);
  char readChar();
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
