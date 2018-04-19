
#include "Map2D3D.h"

//#include <fix16.h>
#include <fix16.hpp>

#define BAUDRATE    115200

/* http://www.gammon.com.au/progmem 
 *  
 *  AVR Libc, and therefore Arduino provides several standard string and memory 
 *  manipulation functions in versions that address program memory rather than RAM. 
 *  The AVR Libc convention is to append "_P" to the name of the standard function, 
 *  thus we have strcpy_P() and memcpy_P() which are functions used in the above 
 *  examples to copy strings or memory from PROGMEM to RAM. The "source" argument 
 *  is assumed to be in PROGMEM and the destination argument in RAM. Otherwise 
 *  they function like the standard library functions strcpy() and memcpy(). 
 *  Some useful functions of this kind are:

    strcpy_P  (dest, PROGMEM src)
    strcmp_P  (s1,   PROGMEM s2)
    strncmp_P (s1,   PROGMEM s2,  len)
    memcpy_P  (dest, PROGMEM src, len)
    sprintf_P (dest, PROGMEM format, item, ... )
 *  
*/

/* NOTE: xs MUST be sorted */
const int16_t  xs[] PROGMEM = { 300,     700,  800,  900,  1500, 1800,   2100, 2500 };
const uint8_t  ys[] PROGMEM = {  10,      89,  126,    0,   225,  230,    220,   10 };
const int8_t  yss[] PROGMEM = {-127,     -50,  127,    0,    10,   -30,   -50,   10 };

#define USEPROGMEM
// In order to use program memory for Fix16's directly, we need to cast to/from int32_t 
#ifdef USEPROGMEM    
const int32_t ysf[] PROGMEM = {0xFF80B333, 0xFFCE1999, 0x7F0000,  0,  0xD4CCD, 0xFFDF0000, 0xFFDC3333, 0xA0000 };
const float  ysfl[] PROGMEM = {-127.3, -49.9, 127.0, 0.0,  13.3, -33.0, -35.8, 10.0 };
#else
const Fix16   ysf[]         = {-127.3, -49.9, 127,   0,  13.3,   -33, -35.8,   10 };
const float  ysfl[]         = {-127.3, -49.9, 127.0, 0.0,  13.3, -33.0, -35.8, 10.0 };
#endif


// Some test data from Miata Brain ECU
// https://sourceforge.net/projects/miatabrain/
const int RPMSteps[16] PROGMEM = 
   {   256, 512, 1024, 1536, 2048, 2560, 3072, 3584, 4086, 4598, 5120, 5632, 6144, 6656, 7168, 7680};

const float tuningMap[256] PROGMEM = 
   {   2.0, 10.0, 10.0, 28.0, 30.4, 30.4, 30.4, 36.9, 37.6, 40.9, 39.6, 31.3, 32.2, 30.0, 30.0, 30.0,
       2.0, 10.0, 10.0, 28.0, 30.4, 30.4, 30.4, 36.9, 37.6, 40.9, 39.6, 31.3, 32.2, 30.0, 30.0, 30.0,
       2.0, 10.0, 10.0, 28.0, 30.4, 30.4, 30.4, 36.9, 37.6, 40.9, 39.6, 31.3, 32.2, 30.0, 30.0, 30.0,
       2.0, 10.0, 13.1, 24.2, 29.6, 29.8, 30.7, 36.0, 37.6, 39.8, 38.7, 31.3, 32.2, 30.0, 30.0, 30.0,
       2.0, 14.0, 15.3, 22.0, 26.2, 27.3, 27.8, 31.6, 33.1, 34.4, 34.4, 32.4, 32.2, 30.0, 30.0, 30.0,
       2.0, 12.2, 13.3, 19.8, 23.8, 24.9, 25.3, 28.9, 30.9, 32.0, 32.4, 30.2, 31.6, 30.0, 30.0, 30.0,
       2.0,  8.7, 12.0, 18.7, 20.9, 23.3, 24.0, 27.6, 29.1, 30.2, 31.3, 28.4, 29.1, 27.6, 27.8, 27.8,
       2.0,  5.1,  7.6, 16.9, 20.2, 21.8, 22.7, 26.0, 27.8, 29.6, 29.1, 26.2, 27.1, 26.4, 26.7, 26.7,
       2.0,  3.3,  5.6, 15.6, 20.0, 21.3, 22.0, 24.0, 25.6, 27.6, 26.9, 25.1, 26.4, 25.3, 25.3, 25.3,
       2.0,  1.6,  2.7, 12.2, 19.1, 20.9, 21.3, 23.3, 24.2, 25.8, 26.0, 24.0, 25.3, 24.7, 24.9, 24.9,
       2.0, -1.3,  1.1,  8.9, 14.7, 20.2, 20.4, 22.4, 22.2, 23.1, 25.1, 23.3, 24.7, 23.8, 24.2, 24.2,
       2.0, -2.0, -2.0,  4.9,  9.1, 11.8, 17.6, 19.1, 19.1, 22.7, 24.7, 22.7, 23.8, 24.0, 22.9, 22.9,
       2.0, -2.0, -2.0,  4.9,  9.1, 11.8, 14.4, 19.1, 19.1, 19.6, 23.6, 21.3, 22.9, 22.9, 22.9, 22.9,
       2.0, -2.0, -2.0,  4.9,  9.1, 11.8, 14.4, 19.1, 19.1, 19.6, 22.4, 21.3, 22.9, 22.9, 22.9, 22.9,
       2.0, -2.0, -2.0,  4.9,  9.1, 11.8, 14.4, 19.1, 19.1, 19.6, 22.4, 21.3, 22.9, 22.9, 22.9, 22.9,
       2.0, -2.0, -2.0,  4.9,  9.1, 11.8, 14.4, 19.1, 19.1, 19.6, 22.4, 21.3, 22.9, 22.9, 22.9, 22.9};



void initSerial()
{
    // Open serial port with a baud rate of BAUDRATE b/s
    Serial.begin(BAUDRATE);
}


void setup()
{
    initSerial();

    Serial.println();
    Serial.println( F("Hi there") );

    Table2D<8, uint8_t>  test;
    test.setXs_P(xs);
    test.setYs_P(ys);

    for( int idx=250; idx<2550; idx+=50)
    {
      uint8_t val = test.getValue(idx);
      Serial.print(idx);
      Serial.print( F(": ") );
      Serial.println( (int)val );
    }

    Table2D<8, int8_t>  test2;
    test2.setXs_P(xs);
    test2.setYs_P(yss);

    for( int idx=250; idx<2550; idx+=50)
    {
      int8_t val = test2.getValue(idx);
      Serial.print(idx);
      Serial.print( F(": ") );
      Serial.println( (int)val );
    }


    Table2D<8, Fix16>  testFix16;
    testFix16.setXs_P(xs);
    
#ifdef USEPROGMEM    
    testFix16.setYs_P( (const Fix16*)ysf );
#else
    testFix16.setYs(ysf);
#endif

    for( int idx=0; idx<8; idx++)
    {
      Serial.print(idx);
      Serial.print( F(": ") );
      Serial.print( (float)ysf[idx] );
      Serial.print( F(": ") );
#ifdef USEPROGMEM       
      Serial.println();
#else
      Serial.println( ysf[idx].value, HEX );
#endif
    }


    for( int idx=250; idx<2550; idx+=50)
    {
      Fix16 val = testFix16.getValue(idx);
      Serial.print(idx);
      Serial.print( F(": ") );
      Serial.println( (float)val );
    }


//  Test Fix16 from float and compare with testFix16

    Table2D<8, Fix16>  testFix16FromFloat;
    testFix16FromFloat.setXs_P(xs);

#ifdef USEPROGMEM
    testFix16FromFloat.setYsFromFloat_P(ysfl);
#else   
    testFix16FromFloat.setYsFromFloat(ysfl);
#endif

    for( int idx=250; idx<2550; idx+=50)
    {
      Fix16 val = testFix16FromFloat.getValue(idx);
      Fix16 val2 = testFix16.getValue(idx);
      Serial.print(idx);
      Serial.print( F(": ") );
      Serial.print( (float)val );
      Serial.print( F(", ") );
      Serial.println( (float)val2 );
    }

 
}


void loop()
{
   

}

