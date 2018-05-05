/*
 ToString.cpp - Base class that provides print() and println()
 Copyright (c) 2008 David A. Mellis.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 
 Modified 23 November 2006 by David A. Mellis
 Modified 03 August 2015 by Chuck Todd
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fix16.hpp>

#include "toString.h"

// progmem on Arduino
#ifndef F
#define F(c) c
#endif


static const char* NumberToString(unsigned long n, uint8_t base=DEC);
static const char* NumberToString(long n, uint8_t base=DEC);
static const char* FloatToString(double number, uint8_t digits);


// Public Methods //////////////////////////////////////////////////////////////


const char* toString(char b, int base)
{
  return NumberToString((long) b, base);
}

const char* toString(unsigned char b, int base)
{
  return NumberToString((unsigned long) b, base);
}

const char* toString(int n, int base)
{
  return NumberToString((long) n, base);
}

const char* toString(unsigned int n, int base)
{
  return NumberToString((unsigned long) n, base);
}

const char* toString(long n, int base)
{
  return NumberToString(n, base);
}

const char* toString(unsigned long n, int base)
{
  return NumberToString(n, base);
}

const char* toString(double n, int digits)
{
  return FloatToString(n, digits);
}

const char* toString(Fix16 n, int digits)
{
  return FloatToString(static_cast<double>(n), digits);
}


// Private Methods /////////////////////////////////////////////////////////////

static const char* NumberToString(unsigned long n, uint8_t base)
{
  static char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return str;
}

static const char* NumberToString(long n, uint8_t base)
{
  static char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  bool isneg = false;

  if ((base == 10) && (n < 0)){
    isneg = true;
    n = -n;
  }

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  if( isneg ) *--str = '-';

  return str;
}

static const char* FloatToString(double number, uint8_t digits) 
{
  static char buf[20];
  char *str = &buf[0];
  
  if (isnan(number)) { strcpy(buf, F("nan")); return buf; }
  if (isinf(number)) { strcpy(buf, F("inf")); return buf; }
  if (number > 4294967040.0) { strcpy(buf, F("ovf")); return buf; }
  if (number <-4294967040.0) { strcpy(buf, F("ovf")); return buf; }

  if (digits > 8 ) digits = 8;
  
  // Handle negative numbers
  if (number < 0.0)
  {
     *str++ = '-';
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  strcpy(str, NumberToString(int_part));
  while( *str != 0 ) str++;

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    *str++= '.'; 
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);

    strcpy(str, NumberToString((unsigned long)remainder));
    while( !*str ) str++;

    remainder -= toPrint; 
  }

  return buf;
}
