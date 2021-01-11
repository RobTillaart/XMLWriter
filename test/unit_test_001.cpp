//
//    FILE: unit_test_001.cpp
//  AUTHOR: Rob Tillaart
//    DATE: 2021-01-09
// PURPOSE: unit tests for the XMLWriter library
//          https://github.com/RobTillaart/XMLWriter
//          https://github.com/Arduino-CI/arduino_ci/blob/master/REFERENCE.md
//

// supported assertions
// ----------------------------
// assertEqual(expected, actual);               // a == b
// assertNotEqual(unwanted, actual);            // a != b
// assertComparativeEquivalent(expected, actual);    // abs(a - b) == 0 or (!(a > b) && !(a < b))
// assertComparativeNotEquivalent(unwanted, actual); // abs(a - b) > 0  or ((a > b) || (a < b))
// assertLess(upperBound, actual);              // a < b
// assertMore(lowerBound, actual);              // a > b
// assertLessOrEqual(upperBound, actual);       // a <= b
// assertMoreOrEqual(lowerBound, actual);       // a >= b
// assertTrue(actual);
// assertFalse(actual);
// assertNull(actual);

// // special cases for floats
// assertEqualFloat(expected, actual, epsilon);    // fabs(a - b) <= epsilon
// assertNotEqualFloat(unwanted, actual, epsilon); // fabs(a - b) >= epsilon
// assertInfinity(actual);                         // isinf(a)
// assertNotInfinity(actual);                      // !isinf(a)
// assertNAN(arg);                                 // isnan(a)
// assertNotNAN(arg);                              // !isnan(a)

#include <ArduinoUnitTests.h>

#include "Arduino.h"
#include "XMLWriter.h"


unittest_setup()
{
}

unittest_teardown()
{
}


unittest(test_constructor)
{
  fprintf(stderr, "VERSION: %s\n", XMLWRITER_VERSION);

  XMLWriter XML(&Serial);
  assertEqual(0, XML.bytesWritten());
  assertEqual(2, XML.getIndentSize());

  // OK it is a debug function but should work
  assertEqual(0, XML.bufferIndex());
}


unittest(test_header_flush)
{
  fprintf(stderr, "VERSION: %s\n", XMLWRITER_VERSION);

  XMLWriter XML(&Serial);

  XML.header();
  assertEqual(0, XML.bytesWritten());
  assertEqual(0, XML.bufferIndex());
  
  XML.flush();
  assertEqual(0, XML.bytesWritten());
  assertEqual(0, XML.bufferIndex());
  
  XML.reset();
  assertEqual(0, XML.bytesWritten());
  assertEqual(0, XML.bufferIndex());
}



unittest(test_indent)
{
  XMLWriter XML(&Serial);
  assertEqual(0, XML.bytesWritten());
  
  for (uint8_t indent = 0; indent < 10; indent += 2)
  {
    XML.setIndentSize(indent);
    assertEqual(indent, XML.getIndentSize());
  }
  assertEqual(0, XML.bytesWritten());
  
  XML.setIndentSize(2);
  for (int i = 0; i < 5; i++)
  {
    indent = XML.getIndentSize();
    XML.incrIndent();
    assertEqual(indent + 2, XML.getIndentSize());
  }
  assertEqual(0, XML.bytesWritten());

  for (int i = 0; i < 5; i++)
  {
    indent = XML.getIndentSize();
    XML.decrIndent();
    assertEqual(indent - 2, XML.getIndentSize());
  }
  
  XML.indent();
  assertEqual(2, XML.bytesWritten());
}


unittest_main()

// --------
