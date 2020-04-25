//
//    FILE: XMLWriter.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.0
//    DATE: 2013-11-06
// PURPOSE: Arduino library for creating XML 
//
// HISTORY:
// 0.1.00  2013-11-06 initial version
// 0.1.01  2013-11-07 rework interfaces
// 0.1.02  2013-11-07 +setIndentSize(), corrected history, +escape support
// 0.1.03  2015-03-07 refactored - footprint + interface
// 0.1.04  2015-05-21 refactored - reduce RAM -> used F() macro etc.
// 0.1.05  2015-05-23 added XMLWRITER_MAXTAGSIZE 15 (to support KML coordinates tag)
// 0.1.6   2016-03-16 added incrIndent(), decrIndent(), indent(), raw();
// 0.1.7   2017-07-26 added const where possible
// 0.1.8   2017-12-09 fix casting issue #83 (long -> int32_t);
// 0.1.9   2017-12-09 add PROGMEM support for escape() strings
// 0.2.0   2020-04-24 refactor, added examples, #pragma, Print as base class
//

#include <XMLWriter.h>


XMLWriter::XMLWriter(Print* stream)
{
  _stream = stream;
  reset();
}

void XMLWriter::reset()
{
  _indent = 0;
  _indentStep = 2;
  _idx = 0;
}

void XMLWriter::header()
{
  println(F("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));
}

void XMLWriter::comment(const char* text, const bool multiLine)
{
  println();
  if (!multiLine) indent();
  print(F("<!-- "));
  if (multiLine) println();
  print(text);
  if (multiLine) println();
  println(F("  -->"));
}

void XMLWriter::tagOpen(const char* tag, const bool newline)
{
  tagOpen(tag, "", newline);
}

void XMLWriter::tagOpen(const char* tag, const char* name, const bool newline)
{
  if (_idx > XMLWRITER_MAXLEVEL)
  {
    comment("XMLWRITER_MAXLEVEL exceeded.");
    comment(tag);
    comment(name);
	return;
  }
  strncpy(tagStack[_idx++], tag, XMLWRITER_MAXTAGSIZE);
  tagStart(tag);
  if (name[0] != 0) tagField("name", name);
  tagEnd(newline, NOSLASH);
  _indent += _indentStep;
}

void XMLWriter::tagClose(const bool ind)
{
  _indent -= _indentStep;
  if (ind) indent();
  print(F("</"));
  print(tagStack[--_idx]);
  println('>');
}

void XMLWriter::tagStart(const char *tag)
{
  indent();
  print('<');
  print(tag);
}

void XMLWriter::tagField(const char *field, const char* str)
{
  print(' ');
  print(field);
  print(F("=\""));
#ifdef XMLWRITER_ESCAPE_SUPPORT
  escape(str);
#else
  print(str);
#endif
  print('"');
}

void XMLWriter::tagEnd(const bool newline, const bool addSlash)
{
  if (addSlash) print('/');
  print('>');
  if (newline) println();
}

void XMLWriter::writeNode(const char* tag, const char* str)
{
  tagOpen(tag, "", NONEWLINE);
#ifdef XMLWRITER_ESCAPE_SUPPORT
  escape(str);
#else
  print(str);
#endif
  tagClose(NOINDENT);
}

void XMLWriter::setIndentSize(const uint8_t size)
{
  _indentStep = size;
}

void XMLWriter::tagField(const char *field, const uint8_t value, const uint8_t base)
{
  tagField(field, (uint32_t) value, base);
}

void XMLWriter::tagField(const char *field, const uint16_t value, const uint8_t base)
{
  tagField(field, (uint32_t) value, base);
}

void XMLWriter::tagField(const char *field, const uint32_t value, const uint8_t base)
{
  print(' ');
  print(field);
  print(F("=\""));
  print(value, base);
  print('"');
}

void XMLWriter::tagField(const char *field, const int8_t value, const uint8_t base)
{
  tagField(field, (int32_t) value, base);
}

void XMLWriter::tagField(const char *field, const int16_t value, const uint8_t base)
{
  tagField(field, (int32_t) value, base);
}

void XMLWriter::tagField(const char *field, const int32_t value, const uint8_t base)
{
  print(' ');
  print(field);
  print(F("=\""));
  print(value, base);
  print('"');
}

void XMLWriter::tagField(const char *field, const bool value)
{
  print(' ');
  print(field);
  print(F("=\""));
  print(value ? F("true") : F("false"));
  print('"');
}

void XMLWriter::tagField(const char *field, const double value, const uint8_t decimals)
{
  print(' ');
  print(field);
  print(F("=\""));
  print(value, decimals);
  print('"');
}

void XMLWriter::writeNode(const char* tag, const uint8_t value, const uint8_t base)
{
  writeNode(tag, (uint32_t) value, base);
}

void XMLWriter::writeNode(const char* tag, const uint16_t value, const uint8_t base)
{
  writeNode(tag, (uint32_t) value, base);
}

void XMLWriter::writeNode(const char* tag, const uint32_t value, const uint8_t base)
{
  tagOpen(tag, "", NONEWLINE);
  print(value, base);  // todo: leading zero's
  tagClose(NOINDENT);
}

void XMLWriter::writeNode(const char* tag, const int8_t value, const uint8_t base)
{
  writeNode(tag, (int32_t) value, base);
}

void XMLWriter::writeNode(const char* tag, const int16_t value, const uint8_t base)
{
  writeNode(tag, (int32_t) value, base);
}

void XMLWriter::writeNode(const char* tag, const int32_t value, const uint8_t base)
{
  tagOpen(tag, "", NONEWLINE);
  print(value, base);
  tagClose(NOINDENT);
}

void XMLWriter::writeNode(const char* tag, const bool value)
{
  tagOpen(tag, "", NONEWLINE);
  print(value ? F("true") : F("false"));
  tagClose(NOINDENT);
}

void XMLWriter::writeNode(const char* tag, const double value, const uint8_t decimals)
{
  tagOpen(tag, "", NONEWLINE);
  print(value, decimals);
  tagClose(NOINDENT);
}

void XMLWriter::indent()
{
  for (uint8_t i = _indent; i > 0; i--) print(' ');
}

size_t XMLWriter::write(uint8_t c)
{
  return _stream->write(c);
};
  
////////////////////////////////////////////////////////////////////
//
// ESCAPE
//

#ifdef XMLWRITER_ESCAPE_SUPPORT
char c[6] = "\"\'<>&";

#ifdef __PROGMEM__
PROGMEM const char quote[] = "&quot;";
PROGMEM const char apostrophe[] = "&apos;";
PROGMEM const char lessthen[] = "&lt;";
PROGMEM const char greaterthen[] = "&gt;";
PROGMEM const char ampersand[] = "&amp;";

PROGMEM const char* const expanded[] =
{
  quote, apostrophe, lessthen, greaterthen, ampersand
};

#else
// NOTE: & and ; are handled in code.
char expanded[][5] = { "quot", "apos","lt","gt","amp"};

#endif

void XMLWriter::escape(const char* str)
{
  char* p = (char *)str;
  while(*p != 0)
  {
    char* q = strchr(c, *p);
    if (q == NULL) print(*p);
#ifdef __PROGMEM__
    else 
    {
      char buffer[8];
      strcpy_P(buffer, (char*)pgm_read_word(&(expanded[q - c])));
      print(buffer);
    }
#else
    else
	{
		print('&');
		print(expanded[q - c]); // uint8_t idx = q-c;
		print(';');
	}
#endif
    p++;
  }
}
#endif


// -- END OF FILE --
