#pragma once
//
//    FILE: XMLWriter.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.0
//    DATE: 2013-11-06
// PURPOSE: Arduino library for creating XML 
//
// Released to the public domain
//

#include "Arduino.h"

#define XMLWRITER_VERSION "0.2.0"

// for comment()
#define NOMULTILINE false
#define MULTILINE   true

// for tagOpen(), tagEnd()
#define NEWLINE     true
#define NONEWLINE   false
#define NOINDENT    false    // for tagClose()

// for tagEnd()
#define SLASH       true
#define NOSLASH     false

// deepness of XML tree 5..10
// needed for stack of tagStack
#define XMLWRITER_MAXLEVEL 5      // adjust for deeper nested structures
#define XMLWRITER_MAXTAGSIZE 15   // adjust for longer fields - !! eats memory !!

// reduce footprint by commenting next line
#define XMLWRITER_ESCAPE_SUPPORT

// uncomment next line to reduce ~30bytes RAM in escape() function
// #define __PROGMEM__

class XMLWriter : Print
{
public:
  // default = Serial
  XMLWriter(Print* stream = &Serial);

  void reset();

  // standard XML header
  void header();

  // if multiline == true it does not indent to allow bigger text blocks
  // <!-- text -->
  void comment(const char* text, const bool multiLine = false);

  // <tag>
  void tagOpen(const char* tag, const bool newline = true);
  // <tag name="name">
  void tagOpen(const char* tag, const char* name, const bool newline = true);
  // </tag>
  void tagClose(const bool ind = true);

  // <tag
  void tagStart(const char* tag);
  // field="value"
  void tagField(const char* field, const char* value);
  //  />
  void tagEnd(const bool newline = true, const bool addSlash = true);

  // <tag>value</tag>
  void writeNode(const char* tag, const char* value);

  // typically 0,2,4; default == 2;
  void setIndentSize(const uint8_t size = 2);

  // for manual layout control
  void incrIndent()       { _indent += _indentStep; };
  void decrIndent()       { _indent -= _indentStep; };
  void indent();
  void raw(const char * str) { print(str); };

  void tagField(const char* field, const uint8_t  value, const uint8_t base = DEC);
  void tagField(const char* field, const uint16_t value, const uint8_t base = DEC);
  void tagField(const char* field, const uint32_t value, const uint8_t base = DEC);
  void tagField(const char* field, const int8_t   value, const uint8_t base = DEC);
  void tagField(const char* field, const int16_t  value, const uint8_t base = DEC);
  void tagField(const char* field, const int32_t  value, const uint8_t base = DEC);
  void tagField(const char* field, const bool     value);
  void tagField(const char* field, const double   value, const uint8_t decimals = 2);

  void writeNode(const char* tag, const uint8_t   value, const uint8_t base = DEC);
  void writeNode(const char* tag, const uint16_t  value, const uint8_t base = DEC);
  void writeNode(const char* tag, const uint32_t  value, const uint8_t base = DEC);
  void writeNode(const char* tag, const int8_t    value, const uint8_t base = DEC);
  void writeNode(const char* tag, const int16_t   value, const uint8_t base = DEC);
  void writeNode(const char* tag, const int32_t   value, const uint8_t base = DEC);
  void writeNode(const char* tag, const bool      value);
  void writeNode(const char* tag, const double    value, const uint8_t decimals = 2);

#ifdef XMLWRITER_ESCAPE_SUPPORT
  // expands the special xml chars
  void escape(const char* str);
#endif

private:
  // outputstream, Print Class
  Print* 			_stream;
  size_t write(uint8_t c);

  // for indentation
  uint8_t _indent;
  uint8_t _indentStep;

  // stack - used to remember the current tagname to create
  // automatic the right close tag.
  uint8_t _idx;
  char tagStack[XMLWRITER_MAXLEVEL][XMLWRITER_MAXTAGSIZE+1];
};

// -- END OF FILE --
