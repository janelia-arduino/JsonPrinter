// ----------------------------------------------------------------------------
// JsonStream.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "JsonStream.h"


CONSTANT_STRING(error_constant_string,"\"error\"");
CONSTANT_STRING(success_constant_string,"\"success\"");

CONSTANT_STRING(true_constant_string,"true");
CONSTANT_STRING(false_constant_string,"false");
CONSTANT_STRING(null_constant_string,"null");

CONSTANT_STRING(long_constant_string,"\"long\"");
CONSTANT_STRING(double_constant_string,"\"double\"");
CONSTANT_STRING(bool_constant_string,"\"bool\"");
CONSTANT_STRING(string_constant_string,"\"string\"");
CONSTANT_STRING(object_constant_string,"\"object\"");
CONSTANT_STRING(array_constant_string,"\"array\"");

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

// encoder methods
void JsonStream::beginObject()
{
  if (!depth_tracker_.empty())
  {
    endArrayItem();
  }
  indent_level_++;
  depth_tracker_.push_back(JsonDepthTracker(true,true));
  *stream_ptr_ << "{";
}

void JsonStream::endObject()
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

void JsonStream::beginArray()
{
  indent_level_++;
  depth_tracker_.push_back(JsonDepthTracker(true,false));
  *stream_ptr_ << "[";
}

void JsonStream::endArray()
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

void JsonStream::setCompactPrint()
{
  pretty_print_ = false;
}

void JsonStream::setPrettyPrint()
{
  pretty_print_ = true;
}

template <>
void JsonStream::writeName<const char *>(const char *name)
{
  endItem();
  if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
  {
    *stream_ptr_ << "\"" << name << "\"" << ":";
  }
}

template <>
void JsonStream::writeName<char *>(char *name)
{
  endItem();
  if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
  {
    *stream_ptr_ << "\"" << name << "\"" << ":";
  }
}

template <>
void JsonStream::writeName<String>(String name)
{
  endItem();
  if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
  {
    *stream_ptr_ << "\"" << name << "\"" << ":";
  }
}

template <>
void JsonStream::writeName<ConstantString>(ConstantString name)
{
  endItem();
  if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
  {
    *stream_ptr_ << "\"" << name << "\"" << ":";
  }
}

template <>
void JsonStream::writeName<ConstantString const *>(ConstantString const *name_ptr)
{
  endItem();
  if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
  {
    *stream_ptr_ << "\"" << *name_ptr << "\"" << ":";
  }
}

template <>
void JsonStream::writeName<ConstantString *>(ConstantString *name_ptr)
{
  endItem();
  if (!depth_tracker_.empty() && depth_tracker_.back().inside_object_)
  {
    *stream_ptr_ << "\"" << *name_ptr << "\"" << ":";
  }
}

template <>
void JsonStream::write<char>(char value)
{
  endArrayItem();
  *stream_ptr_ << "\"" << value << "\"";
}

template <>
void JsonStream::write<const char*>(const char *value)
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

template <>
void JsonStream::write<char*>(char *value)
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

template <>
void JsonStream::write<String>(String value)
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

template <>
void JsonStream::write<String&>(String &value)
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

template <>
void JsonStream::write<ConstantString>(ConstantString value)
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

template <>
void JsonStream::write<ConstantString *>(ConstantString *value_ptr)
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

template <>
void JsonStream::write<ConstantString const *>(ConstantString const *value_ptr)
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

template <>
void JsonStream::write<unsigned char>(unsigned char value)
{
  endArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonStream::write<int>(int value)
{
  endArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonStream::write<unsigned int>(unsigned int value)
{
  endArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonStream::write<long>(long value)
{
  endArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonStream::write<unsigned long>(unsigned long value)
{
  endArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonStream::write<long long>(long long value)
{
  endArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonStream::write<unsigned long long>(unsigned long long value)
{
  endArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonStream::write<JsonStream::ResponseCodes>(JsonStream::ResponseCodes value)
{
  endArrayItem();
  if (!pretty_print_)
  {
    *stream_ptr_ <<  value;
  }
  else
  {
    switch (value)
    {
      case ERROR:
        *stream_ptr_ <<  error_constant_string;
        break;
      case SUCCESS:
        *stream_ptr_ <<  success_constant_string;
        break;
    }
  }
}

template <>
void JsonStream::write<JsonStream::JsonTypes>(JsonStream::JsonTypes value)
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
  }
}

template <>
void JsonStream::write<double>(double value)
{
  endArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  dtostrf(value,DOUBLE_DIGITS_DEFAULT,DOUBLE_DIGITS_DEFAULT,value_char_array);
  *stream_ptr_ <<  value_char_array;
}

template <>
void JsonStream::write<float>(float value)
{
  endArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  dtostrf((double)value,DOUBLE_DIGITS_DEFAULT,DOUBLE_DIGITS_DEFAULT,value_char_array);
  *stream_ptr_ <<  value_char_array;
}

template <>
void JsonStream::writeDouble<double>(double value, unsigned char prec)
{
  endArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  dtostrf(value,prec,prec,value_char_array);
  *stream_ptr_ <<  value_char_array;
}

template <>
void JsonStream::writeDouble<float>(float value, unsigned char prec)
{
  endArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  dtostrf((double)value,prec,prec,value_char_array);
  *stream_ptr_ <<  value_char_array;
}

template <>
void JsonStream::write<bool>(bool value)
{
  if (value)
  {
    *stream_ptr_ <<  true_constant_string;
  }
  else
  {
    *stream_ptr_ <<  false_constant_string;
  }
}

template <>
void JsonStream::write<ArduinoJson::JsonArray*>(ArduinoJson::JsonArray *array_ptr)
{
  endArrayItem();
  array_ptr->printTo(*stream_ptr_);
}

template <>
void JsonStream::write<ArduinoJson::JsonObject*>(ArduinoJson::JsonObject *object_ptr)
{
  endArrayItem();
  object_ptr->printTo(*stream_ptr_);
}

void JsonStream::writeNull()
{
  endArrayItem();
  *stream_ptr_ << null_constant_string;
}

template <>
void JsonStream::writeJson<const char*>(const char *value)
{
  endArrayItem();
  *stream_ptr_ << value;
}

template <>
void JsonStream::writeJson<char*>(char *value)
{
  endArrayItem();
  *stream_ptr_ << value;
}

template <>
void JsonStream::writeJson<String>(String value)
{
  endArrayItem();
  *stream_ptr_ << value;
}

template <>
void JsonStream::writeJson<String&>(String &value)
{
  endArrayItem();
  *stream_ptr_ << value;
}

template <>
void JsonStream::writeJson<ConstantString>(ConstantString value)
{
  endArrayItem();
  *stream_ptr_ << value;
}

template <>
void JsonStream::writeJson<ConstantString *>(ConstantString *value_ptr)
{
  endArrayItem();
  *stream_ptr_ << *value_ptr;
}

template <>
void JsonStream::writeJson<ConstantString const *>(ConstantString const *value_ptr)
{
  endArrayItem();
  *stream_ptr_ << *value_ptr;
}

template <>
void JsonStream::writeJson<ArduinoJson::JsonArray*>(ArduinoJson::JsonArray *array_ptr)
{
  endArrayItem();
  array_ptr->printTo(*stream_ptr_);
}

template <>
void JsonStream::writeJson<ArduinoJson::JsonObject*>(ArduinoJson::JsonObject *object_ptr)
{
  endArrayItem();
  object_ptr->printTo(*stream_ptr_);
}

void JsonStream::writeNewline()
{
  if (depth_tracker_.empty())
  {
    *stream_ptr_ << "\n";
  }
}

void JsonStream::writeChar(char c)
{
  if (!writing_)
  {
    endArrayItem();
    writing_ = true;
  }
  *stream_ptr_ << c;
}

void JsonStream::writeByte(byte b)
{
  if (!writing_)
  {
    endArrayItem();
    writing_ = true;
  }
  *stream_ptr_ << b;
}

// decoder methods
int JsonStream::available()
{
  return stream_ptr_->available();
}

int JsonStream::readJsonIntoBuffer(char buffer[], unsigned int buffer_size)
{
  return stream_ptr_->readBytesUntil(EOL,buffer,buffer_size);
}

// private methods
void JsonStream::indent()
{
  if (pretty_print_)
  {
    for (int i=0; i<(RESPONSE_INDENT*indent_level_); ++i)
    {
      *stream_ptr_ << " ";
    }
  }
}

void JsonStream::endItem()
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

void JsonStream::endArrayItem()
{
  if (!depth_tracker_.back().inside_object_)
  {
    endItem();
  }
}
