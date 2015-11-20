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

JsonStream::JsonStream(GenericSerialBase &serial)
{
  setSerial(serial);
  setCompactPrint();
  indent_level_ = 0;
  writing_ = false;
}

void JsonStream::setSerial(GenericSerialBase &serial)
{
  generic_serial_ptr_ = &serial;
}

void JsonStream::beginObject()
{
  if (!jdt_array_.empty())
  {
    endArrayItem();
  }
  indent_level_++;
  jdt_array_.push_back(JsonDepthTracker(true,true));
  generic_serial_ptr_->getStream() << "{";
}

void JsonStream::endObject()
{
  indent_level_--;
  if (pretty_print_ && (!jdt_array_.back().first_item_))
  {
    generic_serial_ptr_->getStream() << "\n";
    indent();
  }
  jdt_array_.pop_back();
  generic_serial_ptr_->getStream() << "}";
}

void JsonStream::beginArray()
{
  indent_level_++;
  jdt_array_.push_back(JsonDepthTracker(true,false));
  generic_serial_ptr_->getStream() << "[";
}

void JsonStream::endArray()
{
  indent_level_--;
  if (pretty_print_ && (!jdt_array_.back().first_item_))
  {
    generic_serial_ptr_->getStream() << "\n";
    indent();
  }
  jdt_array_.pop_back();
  generic_serial_ptr_->getStream() << "]";
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
void JsonStream::addKey<const char *>(const char *key)
{
  endItem();
  generic_serial_ptr_->getStream() << "\"" << key << "\"" << ":";
}

template <>
void JsonStream::addKey<char *>(char *key)
{
  endItem();
  generic_serial_ptr_->getStream() << "\"" << key << "\"" << ":";
}

template <>
void JsonStream::addKey<String>(String key)
{
  endItem();
  generic_serial_ptr_->getStream() << "\"" << key << "\"" << ":";
}

template <>
void JsonStream::addKey<ConstantString>(ConstantString key)
{
  endItem();
  generic_serial_ptr_->getStream() << "\"" << key << "\"" << ":";
}

template <>
void JsonStream::addKey<ConstantString const *>(ConstantString const *key_ptr)
{
  endItem();
  generic_serial_ptr_->getStream() << "\"" << *key_ptr << "\"" << ":";
}

template <>
void JsonStream::addKey<ConstantString *>(ConstantString *key_ptr)
{
  endItem();
  generic_serial_ptr_->getStream() << "\"" << *key_ptr << "\"" << ":";
}

template <>
void JsonStream::add<char>(char value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonStream::add<const char*>(const char *value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonStream::add<char*>(char *value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonStream::add<String>(String value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonStream::add<String&>(String &value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonStream::add<ConstantString>(ConstantString value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonStream::add<ConstantString *>(ConstantString *value_ptr)
{
  endArrayItem();
  generic_serial_ptr_->getStream() << "\"" << *value_ptr << "\"";
}

template <>
void JsonStream::add<ConstantString const *>(ConstantString const *value_ptr)
{
  endArrayItem();
  generic_serial_ptr_->getStream() << "\"" << *value_ptr << "\"";
}

template <>
void JsonStream::add<unsigned char>(unsigned char value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonStream::add<int>(int value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonStream::add<unsigned int>(unsigned int value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonStream::add<long>(long value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonStream::add<unsigned long>(unsigned long value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonStream::add<long long>(long long value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonStream::add<unsigned long long>(unsigned long long value)
{
  endArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonStream::add<JsonStream::ResponseCodes>(JsonStream::ResponseCodes value)
{
  endArrayItem();
  if (!pretty_print_)
  {
    generic_serial_ptr_->getStream() <<  value;
  }
  else
  {
    switch (value)
    {
      case ERROR:
        generic_serial_ptr_->getStream() <<  error_constant_string;
        break;
      case SUCCESS:
        generic_serial_ptr_->getStream() <<  success_constant_string;
        break;
    }
  }
}

template <>
void JsonStream::add<JsonStream::JsonTypes>(JsonStream::JsonTypes value)
{
  endArrayItem();
  switch (value)
  {
    case LONG_TYPE:
      generic_serial_ptr_->getStream() <<  long_constant_string;
      break;
    case DOUBLE_TYPE:
      generic_serial_ptr_->getStream() <<  double_constant_string;
      break;
    case BOOL_TYPE:
      generic_serial_ptr_->getStream() <<  bool_constant_string;
      break;
    case NULL_TYPE:
      generic_serial_ptr_->getStream() <<  null_constant_string;
      break;
    case STRING_TYPE:
      generic_serial_ptr_->getStream() <<  string_constant_string;
      break;
    case OBJECT_TYPE:
      generic_serial_ptr_->getStream() <<  object_constant_string;
      break;
    case ARRAY_TYPE:
      generic_serial_ptr_->getStream() <<  array_constant_string;
      break;
  }
}

template <>
void JsonStream::add<double>(double value)
{
  endArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  dtostrf(value,DOUBLE_DIGITS_DEFAULT,DOUBLE_DIGITS_DEFAULT,value_char_array);
  generic_serial_ptr_->getStream() <<  value_char_array;
}

template <>
void JsonStream::add<float>(float value)
{
  endArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  dtostrf((double)value,DOUBLE_DIGITS_DEFAULT,DOUBLE_DIGITS_DEFAULT,value_char_array);
  generic_serial_ptr_->getStream() <<  value_char_array;
}

template <>
void JsonStream::addDouble<double>(double value, unsigned char prec)
{
  endArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  dtostrf(value,prec,prec,value_char_array);
  generic_serial_ptr_->getStream() <<  value_char_array;
}

template <>
void JsonStream::addDouble<float>(float value, unsigned char prec)
{
  endArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  dtostrf((double)value,prec,prec,value_char_array);
  generic_serial_ptr_->getStream() <<  value_char_array;
}

template <>
void JsonStream::add<bool>(bool value)
{
  if (value)
  {
    generic_serial_ptr_->getStream() <<  true_constant_string;
  }
  else
  {
    generic_serial_ptr_->getStream() <<  false_constant_string;
  }
}

template <>
void JsonStream::add<ArduinoJson::JsonArray*>(ArduinoJson::JsonArray *array_ptr)
{
  endArrayItem();
  array_ptr->printTo(generic_serial_ptr_->getStream());
}

template <>
void JsonStream::add<ArduinoJson::JsonObject*>(ArduinoJson::JsonObject *object_ptr)
{
  endArrayItem();
  object_ptr->printTo(generic_serial_ptr_->getStream());
}

void JsonStream::addNull()
{
  endArrayItem();
  generic_serial_ptr_->getStream() << null_constant_string;
}

void JsonStream::linefeed()
{
  generic_serial_ptr_->getStream() << "\n";
}

void JsonStream::writeChar(char c)
{
  if (!writing_)
  {
    endArrayItem();
    writing_ = true;
  }
  generic_serial_ptr_->getStream() << c;
}

void JsonStream::writeByte(byte b)
{
  if (!writing_)
  {
    endArrayItem();
    writing_ = true;
  }
  generic_serial_ptr_->getStream() << b;
}

void JsonStream::indent()
{
  if (pretty_print_)
  {
    for (int i=0; i<(RESPONSE_INDENT*indent_level_); ++i)
    {
      generic_serial_ptr_->getStream() << " ";
    }
  }
}

void JsonStream::endItem()
{
  if (!jdt_array_.back().first_item_)
  {
    generic_serial_ptr_->getStream() << ",";
  }
  else
  {
    jdt_array_.back().first_item_ = false;
  }
  if (pretty_print_)
  {
    generic_serial_ptr_->getStream() << "\n";
  }
  indent();
  writing_ = false;
}

void JsonStream::endArrayItem()
{
  if (!jdt_array_.back().inside_object_)
  {
    endItem();
  }
}
