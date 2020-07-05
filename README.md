# XMLWriter

Arduino Library to create simple XML (messages, files, print, ...)

## Description

The XMLWriter class supports generating XML files and send these over a stream
like Ethernet SD.File or Serial.

When instantiating an XMLWriter one can define the internal buffer size.
A bigger buffer will make the output faster, especially for Ethernet and SD.File.
The buffer size should be at least 2 bytes and max 250.
How much faster depends on the properties of the stream.
E.g. the baudrate and internal buffer of Serial, packet behaviour of Ethernet,
or paging of SD cards.
If performance is low one should do testruns with different sizes for the buffer 
and choose one that is appropriate.

Indicative sizes based upon the examples.
Run your tests to find your application optimum.

| STREAM   |    SIZE    |
|:------------|:----------|
| Ethernet | 20-30 |
| Serial   |  5 |
| SD File  |  10-16 |

**Important** When using buffering you should always include a call to **XML.flush()** 
at the end of the XML generation. This will flush the last bytes in the internal buffer.

## Interface

Constructor
- **XMLWriter(stream, bufsize);** Constructor defines the stream and the buffersize
to optimize performance vs memory usage.


Functions for manual layout control
- **setIndentSize(size = 2);**  preferred a multiple of 2;
- **incrIndent()**
- **decrIndent()**
- **indent();**
- **raw(str)** inject any string

General settings
- **setConfig(cfg)** used to show/strip comment, indent, newLine. 
use **setConfig(0);** to minimize the output
- **newLine(n)** add a number of newlines to the output, default = 1

----

Functions
- **reset()** resets internal state, to be called before new XML is written
- **Header()** injects standard XML header string, must be first line
- **comment(text, multiline)** \<!-- text --\>  
if multiline == true it does not indent to allow bigger text blocks
- **flush()** call flush() at the end of writing to empty the internal buffer. **!!**

Functions to create simple tags with named fields
- **tagOpen(tag, newLine)** \<tag\>
- **tagOpen(tag, name, newLine)** \<tag name="name"\>
- **tagCLose()** \</tag\>

Functions to make up tags with multiple fields:
- **tagStart(tag)**  \<tag 
- **tagField(field, string);**  field="string"
- **tagField(field, T value, base = DEC);** standard math types
- **tagEnd(newline = true, addSlash = true);**  /\>

Functions to make a node
- **writeNode(tag, value);** \<tag\>value\</tag\>
- **writeNode(tag, T value, base = DEC);** standard math types

Helper 
- **escape(str)** expands the xml chars: \"\'\<\>\&

Metrics (to optimize buffersize in combination with timing)
- **bufferIndex()** returns the size of the internal buffer
- **bytesWritten()** idem, since reset().

XMLWriter implements the print interface, so at any moment one can use print()
or println() to inject specific information. 
**tagField()** and **writeNode()** do not support 64 bit integer types and 
large values of double. 
Check my printHelpers library for conversion of these to string formats 
e.g. scientific or engineering notation.

## Configuration flags

| Flag | Value |
|----|----|
|XMLWRITER_NONE    | 0x00 |
|XMLWRITER_COMMENT | 0x01 |
|XMLWRITER_INDENT  | 0x02 |
|XMLWRITER_NEWLINE | 0x04 |

- **setConfig(XMLWRITER_NONE);** to minimize the output in bytes.
- **setConfig(XMLWRITER_NEWLINE);** to break an XML stream in lines.
- **setConfig(XMLWRITER_NEWLINE | XMLWRITER_INDENT);** to see XML structure.
- **setConfig(XMLWRITER_NEWLINE | XMLWRITER_INDENT | XMLWRITER_COMMENT);** to see XML structure + comments.



## Operation

See examples

