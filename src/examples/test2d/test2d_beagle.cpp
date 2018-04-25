//-----------------------------------------------------------------------------
// Test 2D maps
//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 Arend Lammertink
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, version 3.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "Map2D3D.h"

//#include <fix16.h>
#include <fix16.hpp>

// Beagle
#include <stdio.h>
#include <string.h>
#define byte uint8_t
#define F( str )  str
#define setXs_P   setXs
#define setYs_P   setYs

//static char             outstr[15];
//#define mPrintFloat(f)  dtostrf(f, 8, 2, outstr); printf("%s\n", outstr);

using namespace std;

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------

/* NOTE: xs MUST be sorted */
const int16_t   xs[]  = { 300,     700,  800,  900,  1500, 1800,   2100, 2500 };

// byte == unsigned char.
const byte     ysb[]  = {   0,      30,   55,   89,    99,   145,   255,   10 };

// signed. All tables contain (about) the same values. 
const int8_t   ys8[]  = {-127,   -50,   127,    0,    13,   -33,   -36,   10 };
const float   ysfl[]  = {-127.3, -49.9, 127.0,  0.0,  13.3, -33.0, -35.8, 10.0 };

// In order to store Fix16 in memory, we need to store it's int32 representationr: 
const int32_t ysf16[]  = { (int32_t)0xFF80B333,  (int32_t)0xFFCE1999, (int32_t)0x7F0000,  
                                0,  (int32_t)0xD4CCD, (int32_t)0xFFDF0000, (int32_t)0xFFDC3333, 
                                (int32_t)0xA0000 };

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

                                
int main()
{
    

    printf( "%s\n","------------------------------" );
    printf( "%s\n","      2D Maps test" );
    printf( "%s\n","------------------------------" );
    printf( "%s\n","Unsigned: only char/byte type." );
    printf( "%s\n","------------------------------" );


    Map2D<8, int16_t, byte>  testb;
    testb.setXs(xs);
    testb.setYs(ysb);


    Map2D<8, uint16_t, byte>  testub;
    testub.setXs_P((uint16_t*)xs);
    testub.setYs_P(ysb);


    for( int idx=250; idx<2550; idx+=50)
    {
      byte val = testb.f(idx);
      byte valus = testub.f(idx);
      printf( "%d", idx);
      printf( "%s", F(": ") );
      printf( "%d", static_cast<int>(val) );
      printf( "%s", F(", ") );
      printf( "%d\n", static_cast<int>(valus) );
    }

/*
    printf( "%s\n",( F("------------------------------------------") );
    printf( "%s\n",( F("               Signed") );
    printf( "%s\n",( F("------------------------------------------") );
    printf( "%s\n",( F("rpm  char  fix16   fix16   float   double ") );
    printf( "%s\n",( F("------------------------------------------") );

    Map2D<8, int16_t, int8_t>  testInt8;
    testInt8.setXs_P(xs);
    testInt8.setYs_P(ys8);

    Map2D<8, int16_t, Fix16>  testFix16;
    testFix16.setXs_P(xs);
    testFix16.setYs_P( (const Fix16*)ysf16 );

    Map2D<8, int16_t, Fix16>  testFix16FromFloat;
    testFix16FromFloat.setXs_P(xs);
    testFix16FromFloat.setYsFromFloat_P(ysfl);

    Map2D<8, int16_t, float> testFloat;
    testFloat.setXs_P(xs);
    testFloat.setYsFromFloat_P(ysfl);

    Map2D<8, int16_t, double> testDouble;
    testDouble.setXs_P(xs);
    testDouble.setYsFromFloat_P(ysfl);

    for( int idx=250; idx<2550; idx+=50)
    {
      int8_t val8   = testInt8.f(idx); 
      float val16ff = testFix16FromFloat.f(idx);
      float val16   = testFix16.f(idx);
      float valf    = testFloat.f(idx);
      float vald    = testDouble.f(idx);

      printf("%4d: %4d", idx, val8); //integers can be printed with printf
      mPrintFloat(val16);
      mPrintFloat(val16ff);
      mPrintFloat(valf);      
      mPrintFloat(vald);      
      printf( "%s\n",();
    }

    printf( "%s\n",( F("------------------------------------------") );
    printf( "%s\n",( F("               Sizes") );
    printf( "%s\n",( F("------------------------------------------") );

#define sprint(expression) printf( "%s",F("Size of ")); printf( "%s", F(#expression) );  \
            printf( "%s", F("\t with size: ") ); printf( "%s", expression.sizeX() ); \
            printf( "%s", F(": ") ); printf( "%s\n",( sizeof(expression) )

   sprint( testInt8 );
   sprint( testFix16 );
   sprint( testFix16FromFloat );
   sprint( testFloat );
   sprint( testDouble );

   printf( "%s\n",( F("------------------------------------------") );

*/
}



//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------



