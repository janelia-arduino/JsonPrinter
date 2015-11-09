// ----------------------------------------------------------------------------
// JsonPrinter.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "JsonPrinter.h"


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

JsonPrinter::JsonPrinter(GenericSerialBase &serial)
{
  setSerial(serial);
  setCompactPrint();
  indent_level_ = 0;
}

void JsonPrinter::setSerial(GenericSerialBase &serial)
{
  generic_serial_ptr_ = &serial;
}

void JsonPrinter::startObject()
{
  if (!jdt_array_.empty())
  {
    stopArrayItem();
  }
  indent_level_++;
  jdt_array_.push_back(JsonDepthTracker(true,true));
  generic_serial_ptr_->getStream() << "{";
}

void JsonPrinter::stopObject()
{
  indent_level_--;
  if (pretty_print_ && (!jdt_array_.back().first_item_))
  {
    generic_serial_ptr_->getStream() << endl;
    indent();
  }
  jdt_array_.pop_back();
  generic_serial_ptr_->getStream() << "}";
}

void JsonPrinter::startArray()
{
  indent_level_++;
  jdt_array_.push_back(JsonDepthTracker(true,false));
  generic_serial_ptr_->getStream() << "[";
}

void JsonPrinter::stopArray()
{
  indent_level_--;
  if (pretty_print_ && (!jdt_array_.back().first_item_))
  {
    generic_serial_ptr_->getStream() << endl;
    indent();
  }
  jdt_array_.pop_back();
  generic_serial_ptr_->getStream() << "]";
}

void JsonPrinter::setCompactPrint()
{
  pretty_print_ = false;
}

void JsonPrinter::setPrettyPrint()
{
  pretty_print_ = true;
}

template <>
void JsonPrinter::addKey<const char *>(const char *key)
{
  stopItem();
  generic_serial_ptr_->getStream() << "\"" << key << "\"" << ":";
}

template <>
void JsonPrinter::addKey<char *>(char *key)
{
  stopItem();
  generic_serial_ptr_->getStream() << "\"" << key << "\"" << ":";
}

template <>
void JsonPrinter::addKey<String>(String key)
{
  stopItem();
  generic_serial_ptr_->getStream() << "\"" << key << "\"" << ":";
}

template <>
void JsonPrinter::addKey<ConstantString>(ConstantString key)
{
  stopItem();
  generic_serial_ptr_->getStream() << "\"" << key << "\"" << ":";
}

template <>
void JsonPrinter::add<char>(char value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<const char*>(const char *value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<char*>(char *value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<String>(String value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<String&>(String &value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<ConstantString>(ConstantString value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<unsigned char>(unsigned char value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonPrinter::add<int>(int value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonPrinter::add<unsigned int>(unsigned int value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonPrinter::add<long>(long value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonPrinter::add<unsigned long>(unsigned long value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonPrinter::add<long long>(long long value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonPrinter::add<unsigned long long>(unsigned long long value)
{
  stopArrayItem();
  generic_serial_ptr_->getStream() <<  _DEC(value);
}

template <>
void JsonPrinter::add<JsonPrinter::ResponseCodes>(JsonPrinter::ResponseCodes value)
{
  stopArrayItem();
  if (!pretty_print_)
  {
    generic_serial_ptr_->getStream() <<  value;
  }
  else
  {
    switch (value)
    {
      case ERROR:
        generic_serial_ptr_->getStream() <<  "\"error\"";
        break;
      case SUCCESS:
        generic_serial_ptr_->getStream() <<  "\"success\"";
        break;
    }
  }
}

template <>
void JsonPrinter::add<double>(double value)
{
  stopArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  // dtostre(value,value_char_array,DOUBLE_DIGITS,0);
  dtostrf(value,DOUBLE_DIGITS,DOUBLE_DIGITS,value_char_array);
  generic_serial_ptr_->getStream() <<  value_char_array;
}

template <>
void JsonPrinter::add<float>(float value)
{
  stopArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  // dtostre((double)value,value_char_array,DOUBLE_DIGITS,0);
  dtostrf((double)value,DOUBLE_DIGITS,DOUBLE_DIGITS,value_char_array);
  generic_serial_ptr_->getStream() <<  value_char_array;
}

template <>
void JsonPrinter::add<bool>(bool value)
{
  if (value)
  {
    generic_serial_ptr_->getStream() <<  "true";
  }
  else
  {
    generic_serial_ptr_->getStream() <<  "false";
  }
}

void JsonPrinter::addNull()
{
  stopArrayItem();
  generic_serial_ptr_->getStream() << "null";
}

void JsonPrinter::indent()
{
  if (pretty_print_)
  {
    for (int i=0; i<(RESPONSE_INDENT*indent_level_); ++i)
    {
      generic_serial_ptr_->getStream() << " ";
    }
  }
}

void JsonPrinter::stopItem()
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
    generic_serial_ptr_->getStream() << endl;
  }
  indent();
}

void JsonPrinter::stopArrayItem()
{
  if (!jdt_array_.back().inside_object_)
  {
    stopItem();
  }
}
