//-----------------------------------------------------------------------------
// Decorator for Arduino's Stream class, which is a/o implemented by Serial
//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 Arend Lammertink
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, version 3.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
//
//
// Description:
//
// The Serial class in Arduino is derived from the Stream class, which on it's
// turn is derived from the Print class. In these, there is no support for
// reading/writing integers or floats in binary form.
//
// In order to add this functionality, we use the Decorator pattern:
//
//  https://cpppatterns.com/patterns/decorator.html 
//
// The ExtendedSerial class is derived from the Stream class and thus
// implements all methods which are present in the Stream class, while it adds
// a number of new methods to send and receive values in binary format as well
// as support for printing Fix16 fixed point numbers. Also, all the methods
// from the HardwareSerial class are implemented.
//
// It can be instantiated using an existing ExtendedSerial instance, such as
// Serial, Serial1, etc:
//
//   ExtentedSerial mySerial(Serial);
//
// It can then be used just like any other Serial, like:
//   
//   mySerial.begin(9600);
//   mySerial.println("Hello, world!");
//
// In additon to the standard methods, one can also send and receive ints,
// floats and Fix16s in binary format. Both the send and receive methods
// return the number of bytes transferred. For example:
//
//   float f=1.23;
//   mySerial.send(f);
//
//   float g;
//   if( !mySerial.receive(g) ) { mySerial.println("No data available"); }
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Multi-include protection
//-----------------------------------------------------------------------------

#ifndef _EXTENDED_SERIAL_H
#define _EXTENDED_SERIAL_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <HardwareSerial.h>
#include <fix16.hpp>

//-----------------------------------------------------------------------------
// ExtendedSerial class
//-----------------------------------------------------------------------------

class ExtendedSerial : public Stream
{

  public:
                    ExtendedSerial(HardwareSerial& s) : _s(s) {}

    // Methods delegated to existing Serial instance
#ifdef AVR
    void            begin(unsigned long baud)         { begin(baud, SERIAL_8N1);  }
    void            begin(unsigned long b, uint8_t m) { _s.begin(b,m);            }
#else
    void            begin(unsigned long baud)         { _s.begin(baud);           }
#endif
    void            end()                             { _s.end();                 }
    virtual int     available(void)                   { return _s.available();    }
    virtual int     peek(void)                        { return _s.peek();         }
    virtual int     read(void)                        { return _s.read();         }
    virtual void    flush(void)                       { return _s.flush();        }
    virtual size_t  write(uint8_t n)                  { return _s.write(n);       }
    virtual size_t  write(const uint8_t* b,size_t sz) { return _s.write(b,sz);    }
    inline  size_t  write(unsigned long n)            { return write((uint8_t)n); }
    inline  size_t  write(long n)                     { return write((uint8_t)n); }
    inline  size_t  write(unsigned int n)             { return write((uint8_t)n); }
    inline  size_t  write(int n)                      { return write((uint8_t)n); }
#ifdef AVR
    virtual int     availableForWrite(void)           { return _s.availableForWrite(); }
#endif
    operator bool()                                   { return true;                   }

    // Extra methods
    size_t          print(  Fix16 f, int d = 2)       { return print(  (double)f,d); }
    size_t          println(Fix16 f, int d = 2)       { return println((double)f,d); }

    size_t          receive(uint8_t* buf, size_t sz)
                    {
                      if( available() < sz )            return 0;

                      for( int i=0; i<sz; i++)        { *buf++ = (uint8_t)read();    }

                      return sz;
                    }

    size_t          send(const uint8_t& x)  { return write(x);                                   }
    size_t          receive(uint8_t& x)     { return receive((uint8_t*)&x, sizeof(uint8_t));     }

    size_t          send(const int8_t& x)   { return write((uint8_t)x);                          }
    size_t          receive(int8_t& x)      { return receive((uint8_t*)&x, sizeof(int8_t));      }

    size_t          send(const int16_t& x)  { return write((const uint8_t*)&x, sizeof(int16_t)); }
    size_t          receive(int16_t& x)     { return receive((uint8_t*)&x,     sizeof(int16_t)); }

    size_t          send(const uint16_t& x) { return write((const uint8_t*)&x, sizeof(uint16_t));}
    size_t          receive(uint16_t& x)    { return receive((uint8_t*)&x,     sizeof(uint16_t));}

    size_t          send(const int32_t& x)  { return write((const uint8_t*)&x, sizeof(int32_t)); }
    size_t          receive(int32_t& x)     { return receive((uint8_t*)&x,     sizeof(int32_t)); }

    size_t          send(const uint32_t& x) { return write((const uint8_t*)&x, sizeof(uint32_t));}
    size_t          receive(uint32_t& x)    { return receive((uint8_t*)&x,     sizeof(uint32_t));}

    size_t          send(const float& x)    { return write((const uint8_t*)&x, sizeof(float));   }
    size_t          receive(float& x)       { return receive((uint8_t*)&x,     sizeof(float));   }

    size_t          send(const double& x)   { return write((const uint8_t*)&x, sizeof(double));  }
    size_t          receive(double& x)      { return receive((uint8_t*)&x,     sizeof(double));  }

    size_t          send(const Fix16& x)    { return write((const uint8_t*)&x, sizeof(Fix16));   }
    size_t          receive(Fix16& x)       { return receive((uint8_t*)&x,     sizeof(Fix16));   }

    using           Print::write;   // pull in the other write methods from Print.
    using           Print::print;   // pull in existing print methods from Print.
    using           Print::println; // pull in existing println methods from Print.

  private:

    HardwareSerial& _s;

                    // Must be instantiated with a HardwareSerial&.
                    ExtendedSerial();
                    ExtendedSerial(ExtendedSerial&);
                    ExtendedSerial(int);
                    ExtendedSerial(const char*);
};




//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

#endif // End multi-include protection
