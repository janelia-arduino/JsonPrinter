// ----------------------------------------------------------------------------
// JsonStream.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "JsonStream.h"


CONSTANT_STRING(true_constant_string,"true");
CONSTANT_STRING(false_constant_string,"false");
CONSTANT_STRING(null_constant_string,"null");

CONSTANT_STRING(long_constant_string,"\"long\"");
CONSTANT_STRING(double_constant_string,"\"double\"");
CONSTANT_STRING(bool_constant_string,"\"bool\"");
CONSTANT_STRING(string_constant_string,"\"string\"");
CONSTANT_STRING(object_constant_string,"\"object\"");
CONSTANT_STRING(array_constant_string,"\"array\"");
CONSTANT_STRING(value_constant_string,"\"value\"");

JsonDepthTracker::JsonDepthTracker()
{
  first_item_ = true;
  inside_object_ = true;
}

JsonDepthTracker::JsonDepthTracker(bool first_item, bool inside_object) :
  first_item_(first_item),
  inside_object_(inside_object)
{
}

JsonStream::JsonStream()
{
  stream_ptr_ = NULL;
  setCompactPrint();
  indent_level_ = 0;
  writing_ = false;
}

JsonStream::JsonStream(Stream &stream)
{
  setStream(stream);
  setCompactPrint();
  indent_level_ = 0;
  writing_ = false;
}

void JsonStream::setStream(Stream &stream)
{
  stream_ptr_ = &stream;
}

Stream& JsonStream::getStream()
{
  return *stream_ptr_;
}

// encoder methods
void JsonStream::beginObject()
{
  if (stream_ptr_ != NULL)
  {
    if (!depth_tracker_.empty())
    {
      endArrayItem();
    }
    indent_level_++;
    depth_tracker_.push_back(JsonDepthTracker(true,true));
    *stream_ptr_ << "{";
  }
}

void JsonStream::endObject()
{
  if (stream_ptr_ != NULL)
  {
    indent_level_--;
    if (pretty_print_ && (!depth_tracker_.back().first_item_))
    {
      *stream_ptr_ << "\n";
      indent();
    }
    depth_tracker_.pop_back();
    *stream_ptr_ << "}";
  }
}

void JsonStream::beginArray()
{
  if (stream_ptr_ != NULL)
  {
    indent_level_++;
    depth_tracker_.push_back(JsonDepthTracker(true,false));
    *stream_ptr_ << "[";
  }
}

void JsonStream::endArray()
{
  if (stream_ptr_ != NULL)
  {
    indent_level_--;
    if (pretty_print_ && (!depth_tracker_.back().first_item_))
    {
      *stream_ptr_ << "\n";
      indent();
    }
    depth_tracker_.pop_back();
    *stream_ptr_ << "]";
  }
}

void JsonStream::setCompactPrint()
{
  pretty_print_ = false;
}

void JsonStream::setPrettyPrint()
{
  pretty_print_ = true;
}

template <>
void JsonStream::writeKey<const char *>(const char *key)
{
  if (stream_ptr_ != NULL)
  {
    endItem();
    if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
    {
      *stream_ptr_ << "\"" << key << "\"" << ":";
    }
  }
}

template <>
void JsonStream::writeKey<char *>(char *key)
{
  if (stream_ptr_ != NULL)
  {
    endItem();
    if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
    {
      *stream_ptr_ << "\"" << key << "\"" << ":";
    }
  }
}

template <>
void JsonStream::writeKey<String>(String key)
{
  if (stream_ptr_ != NULL)
  {
    endItem();
    if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
    {
      *stream_ptr_ << "\"" << key << "\"" << ":";
    }
  }
}

template <>
void JsonStream::writeKey<ConstantString>(ConstantString key)
{
  if (stream_ptr_ != NULL)
  {
    endItem();
    if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
    {
      *stream_ptr_ << "\"" << key << "\"" << ":";
    }
  }
}

template <>
void JsonStream::writeKey<ConstantString const *>(ConstantString const *key_ptr)
{
  if (stream_ptr_ != NULL)
  {
    endItem();
    if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
    {
      *stream_ptr_ << "\"" << *key_ptr << "\"" << ":";
    }
  }
}

template <>
void JsonStream::writeKey<ConstantString *>(ConstantString *key_ptr)
{
  if (stream_ptr_ != NULL)
  {
    endItem();
    if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
    {
      *stream_ptr_ << "\"" << *key_ptr << "\"" << ":";
    }
  }
}

template <>
void JsonStream::write<char>(char value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ << "\"" << value << "\"";
  }
}

template <>
void JsonStream::write<const char *>(const char *value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    if (!depth_tracker_.empty())
    {
      *stream_ptr_ << "\"" << value << "\"";
    }
    else
    {
      *stream_ptr_ << value;
    }
  }
}

template <>
void JsonStream::write<char *>(char *value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    if (!depth_tracker_.empty())
    {
      *stream_ptr_ << "\"" << value << "\"";
    }
    else
    {
      *stream_ptr_ << value;
    }
  }
}

template <>
void JsonStream::write<String>(String value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    if (!depth_tracker_.empty())
    {
      *stream_ptr_ << "\"" << value << "\"";
    }
    else
    {
      *stream_ptr_ << value;
    }
  }
}

template <>
void JsonStream::write<String&>(String &value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    if (!depth_tracker_.empty())
    {
      *stream_ptr_ << "\"" << value << "\"";
    }
    else
    {
      *stream_ptr_ << value;
    }
  }
}

template <>
void JsonStream::write<ConstantString>(ConstantString value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    if (!depth_tracker_.empty())
    {
      *stream_ptr_ << "\"" << value << "\"";
    }
    else
    {
      *stream_ptr_ << value;
    }
  }
}

template <>
void JsonStream::write<ConstantString *>(ConstantString *value_ptr)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    if (!depth_tracker_.empty())
    {
      *stream_ptr_ << "\"" << *value_ptr << "\"";
    }
    else
    {
      *stream_ptr_ << *value_ptr;
    }
  }
}

template <>
void JsonStream::write<ConstantString const *>(ConstantString const *value_ptr)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    if (!depth_tracker_.empty())
    {
      *stream_ptr_ << "\"" << *value_ptr << "\"";
    }
    else
    {
      *stream_ptr_ << *value_ptr;
    }
  }
}

template <>
void JsonStream::write<unsigned char>(unsigned char value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ <<  _DEC(value);
  }
}

template <>
void JsonStream::write<int>(int value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ <<  _DEC(value);
  }
}

template <>
void JsonStream::write<unsigned int>(unsigned int value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ <<  _DEC(value);
  }
}

template <>
void JsonStream::write<long>(long value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ <<  _DEC(value);
  }
}

template <>
void JsonStream::write<unsigned long>(unsigned long value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ <<  _DEC(value);
  }
}

template <>
void JsonStream::write<long long>(long long value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ <<  _DEC(value);
  }
}

template <>
void JsonStream::write<unsigned long long>(unsigned long long value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ <<  _DEC(value);
  }
}

template <>
void JsonStream::write<JsonStream::JsonTypes>(JsonStream::JsonTypes value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    switch (value)
    {
      case LONG_TYPE:
        *stream_ptr_ <<  long_constant_string;
        break;
      case DOUBLE_TYPE:
        *stream_ptr_ <<  double_constant_string;
        break;
      case BOOL_TYPE:
        *stream_ptr_ <<  bool_constant_string;
        break;
      case NULL_TYPE:
        *stream_ptr_ <<  null_constant_string;
        break;
      case STRING_TYPE:
        *stream_ptr_ <<  string_constant_string;
        break;
      case OBJECT_TYPE:
        *stream_ptr_ <<  object_constant_string;
        break;
      case ARRAY_TYPE:
        *stream_ptr_ <<  array_constant_string;
        break;
      case VALUE_TYPE:
        *stream_ptr_ <<  value_constant_string;
        break;
    }
  }
}

template <>
void JsonStream::write<double>(double value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    char value_char_array[STRING_LENGTH_DOUBLE];
    dtostrf(value,DOUBLE_DIGITS_DEFAULT,DOUBLE_DIGITS_DEFAULT,value_char_array);
    *stream_ptr_ <<  value_char_array;
  }
}

template <>
void JsonStream::write<float>(float value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    char value_char_array[STRING_LENGTH_DOUBLE];
    dtostrf((double)value,DOUBLE_DIGITS_DEFAULT,DOUBLE_DIGITS_DEFAULT,value_char_array);
    *stream_ptr_ <<  value_char_array;
  }
}

template <>
void JsonStream::writeDouble<double>(double value, unsigned char prec)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    char value_char_array[STRING_LENGTH_DOUBLE];
    dtostrf(value,prec,prec,value_char_array);
    *stream_ptr_ <<  value_char_array;
  }
}

template <>
void JsonStream::writeDouble<float>(float value, unsigned char prec)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    char value_char_array[STRING_LENGTH_DOUBLE];
    dtostrf((double)value,prec,prec,value_char_array);
    *stream_ptr_ <<  value_char_array;
  }
}

template <>
void JsonStream::write<bool>(bool value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    if (value)
    {
      *stream_ptr_ <<  true_constant_string;
    }
    else
    {
      *stream_ptr_ <<  false_constant_string;
    }
  }
}

template <>
void JsonStream::write<ArduinoJson::JsonArray *>(ArduinoJson::JsonArray *array_ptr)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    array_ptr->printTo(*stream_ptr_);
  }
}

template <>
void JsonStream::write<ArduinoJson::JsonObject *>(ArduinoJson::JsonObject *object_ptr)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    object_ptr->printTo(*stream_ptr_);
  }
}

void JsonStream::writeNull()
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ << null_constant_string;
  }
}

template <>
void JsonStream::writeJson<const char *>(const char *value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ << value;
  }
}

template <>
void JsonStream::writeJson<char *>(char *value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ << value;
  }
}

template <>
void JsonStream::writeJson<String>(String value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ << value;
  }
}

template <>
void JsonStream::writeJson<String&>(String &value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ << value;
  }
}

template <>
void JsonStream::writeJson<ConstantString>(ConstantString value)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ << value;
  }
}

template <>
void JsonStream::writeJson<ConstantString *>(ConstantString *value_ptr)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ << *value_ptr;
  }
}

template <>
void JsonStream::writeJson<ConstantString const *>(ConstantString const *value_ptr)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    *stream_ptr_ << *value_ptr;
  }
}

template <>
void JsonStream::writeJson<ArduinoJson::JsonArray *>(ArduinoJson::JsonArray *array_ptr)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    array_ptr->printTo(*stream_ptr_);
  }
}

template <>
void JsonStream::writeJson<ArduinoJson::JsonObject *>(ArduinoJson::JsonObject *object_ptr)
{
  if (stream_ptr_ != NULL)
  {
    endArrayItem();
    object_ptr->printTo(*stream_ptr_);
  }
}

void JsonStream::writeNewline()
{
  if (stream_ptr_ != NULL)
  {
    if (depth_tracker_.empty())
    {
      *stream_ptr_ << "\n";
    }
  }
}

void JsonStream::writeChar(char c)
{
  if (stream_ptr_ != NULL)
  {
    if (!writing_)
    {
      endArrayItem();
      writing_ = true;
    }
    *stream_ptr_ << c;
  }
}

void JsonStream::writeByte(byte b)
{
  if (stream_ptr_ != NULL)
  {
    if (!writing_)
    {
      endArrayItem();
      writing_ = true;
    }
    *stream_ptr_ << b;
  }
}

// decoder methods
int JsonStream::available()
{
  if (stream_ptr_ != NULL)
  {
    return stream_ptr_->available();
  }
  else
  {
    return 0;
  }
}

int JsonStream::readJsonIntoBuffer(char buffer[], unsigned int buffer_size)
{
  if (stream_ptr_ != NULL)
  {
    unsigned int bytes_read = stream_ptr_->readBytesUntil(EOL,buffer,buffer_size);
    if (bytes_read < buffer_size)
    {
      // terminate string
      buffer[bytes_read] = 0;
    }
    else
    {
      // set buffer to empty string
      buffer[0] = 0;
      // clear stream of remaining characters
      char EOL_STR[2];
      EOL_STR[0] = EOL;
      EOL_STR[1] = 0;
      stream_ptr_->find(EOL_STR);
      return -1;
    }
    return bytes_read;
  }
  else
  {
    return 0;
  }
}

char JsonStream::readChar()
{
  if (stream_ptr_ != NULL)
  {
    return stream_ptr_->read();
  }
  else
  {
    return 0;
  }
}

// private methods
void JsonStream::indent()
{
  if (stream_ptr_ != NULL)
  {
    if (pretty_print_)
    {
      for (int i=0; i<(RESPONSE_INDENT*indent_level_); ++i)
      {
        *stream_ptr_ << " ";
      }
    }
  }
}

void JsonStream::endItem()
{
  if (stream_ptr_ != NULL)
  {
    if (!depth_tracker_.empty())
    {
      if (!depth_tracker_.back().first_item_)
      {
        *stream_ptr_ << ",";
      }
      else
      {
        depth_tracker_.back().first_item_ = false;
      }
      if (pretty_print_)
      {
        *stream_ptr_ << "\n";
      }
      indent();
    }
    writing_ = false;
  }
}

void JsonStream::endArrayItem()
{
  if (!depth_tracker_.back().inside_object_)
  {
    endItem();
  }
}
