// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _JSON_PRINTER_CONSTANTS_H_
#define _JSON_PRINTER_CONSTANTS_H_


namespace JsonPrinter
{
namespace constants
{
enum {RESPONSE_DEPTH_MAX=8};

enum ResponseCodes
  {
    ERROR=0,
    SUCCESS=1,
  };

enum{STRING_LENGTH_DOUBLE=36};

extern const int double_digits;

extern const int response_indent;
}
}
#endif
