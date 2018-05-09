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

#include "Arduino.h"
#include "Map2D3D.h"
#include "toString.h"

//#include <fix16.h>
#include <fix16.hpp>


using namespace std;

#define BAUDRATE    115200

// Stuff needed for printing
#define mPrintFloat(f)  Serial.print(toString(f,9,2));


const int8_t xs_int8_t_8[]    PROGMEM = 
 { -128,           -96,            -64,            -32,            0,              32,             64,             96,             127 };
const int16_t xs_int16_t_8[]  PROGMEM = 
 { -1280,          -960,           -640,           -320,           0,              320,            640,            960,            1270 };
const float xs_float_8[]      PROGMEM = 
 { -1280.0,        -960.0,         -640.0,         -320.0,         0.0,            320.0,          640.0,          960.0,          1270.0 };
const int8_t ys_int8_t_8[]    PROGMEM = 
 { -128,           -96,            -64,            -32,            0,              32,             64,             96,             127 };
const int16_t ys_int16_t_8[]  PROGMEM = 
 { -1280,          -960,           -640,           -320,           0,              320,            640,            960,            1270 };
const float ys_float_8[]      PROGMEM = 
 { -1280.0,        -960.0,         -640.0,         -320.0,         0.0,            320.0,          640.0,          960.0,          1270.0 };

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

                                
void setup()
{
    initSerial();


    

    Serial.println();
    Serial.println( F("------------------------------") );
    Serial.println( F("      2D Maps test") );
    Serial.println( F("------------------------------") );
    Serial.println( F(" Signed: int and float types.") );
    Serial.println( F("------------------------------") );



    Map2D<8,int8_t,int8_t> test_int8_t_int8_t_8;
    test_int8_t_int8_t_8.setXs_P(xs_int8_t_8);
    test_int8_t_int8_t_8.setYs_P(ys_int8_t_8);

    Map2D<8,int8_t,int16_t> test_int8_t_int16_t_8;
    test_int8_t_int16_t_8.setXs_P(xs_int8_t_8);
    test_int8_t_int16_t_8.setYs_P(ys_int16_t_8);

    Map2D<8,int8_t,Fix16> test_int8_t_Fix16_8;
    test_int8_t_Fix16_8.setXs_P(xs_int8_t_8);
    test_int8_t_Fix16_8.setYsFromFloat_P(ys_float_8);

    Map2D<8,int8_t,float> test_int8_t_float_8;
    test_int8_t_float_8.setXs_P(xs_int8_t_8);
    test_int8_t_float_8.setYsFromFloat_P(ys_float_8);

    Map2D<8,int16_t,int8_t> test_int16_t_int8_t_8;
    test_int16_t_int8_t_8.setXs_P(xs_int16_t_8);
    test_int16_t_int8_t_8.setYs_P(ys_int8_t_8);

    Map2D<8,int16_t,int16_t> test_int16_t_int16_t_8;
    test_int16_t_int16_t_8.setXs_P(xs_int16_t_8);
    test_int16_t_int16_t_8.setYs_P(ys_int16_t_8);

    Map2D<8,int16_t,Fix16> test_int16_t_Fix16_8;
    test_int16_t_Fix16_8.setXs_P(xs_int16_t_8);
    test_int16_t_Fix16_8.setYsFromFloat_P(ys_float_8);

    Map2D<8,int16_t,float> test_int16_t_float_8;
    test_int16_t_float_8.setXs_P(xs_int16_t_8);
    test_int16_t_float_8.setYsFromFloat_P(ys_float_8);

    Map2D<8,Fix16,int8_t> test_Fix16_int8_t_8;
    test_Fix16_int8_t_8.setXsFromFloat_P(xs_float_8);
    test_Fix16_int8_t_8.setYs_P(ys_int8_t_8);

    Map2D<8,Fix16,int16_t> test_Fix16_int16_t_8;
    test_Fix16_int16_t_8.setXsFromFloat_P(xs_float_8);
    test_Fix16_int16_t_8.setYs_P(ys_int16_t_8);

    Map2D<8,Fix16,Fix16> test_Fix16_Fix16_8;
    test_Fix16_Fix16_8.setXsFromFloat_P(xs_float_8);
    test_Fix16_Fix16_8.setYsFromFloat_P(ys_float_8);

    Map2D<8,Fix16,float> test_Fix16_float_8;
    test_Fix16_float_8.setXsFromFloat_P(xs_float_8);
    test_Fix16_float_8.setYsFromFloat_P(ys_float_8);

  for( int idx=-1280; idx<=1270; idx+=10)
  {

    int8_t val_int8_t_int8_t_8  = test_int8_t_int8_t_8.f(idx/10);
    int8_t val_int16_t_int8_t_8  = test_int16_t_int8_t_8.f(idx);
    int8_t val_Fix16_int8_t_8  = test_Fix16_int8_t_8.f(idx);
    int16_t val_int8_t_int16_t_8  = test_int8_t_int16_t_8.f(idx/10);
    int16_t val_int16_t_int16_t_8  = test_int16_t_int16_t_8.f(idx);
    int16_t val_Fix16_int16_t_8  = test_Fix16_int16_t_8.f(idx);
    float val_int8_t_Fix16_8  = test_int8_t_Fix16_8.f(idx/10);
    float val_int16_t_Fix16_8  = test_int16_t_Fix16_8.f(idx);
    float val_Fix16_Fix16_8  = test_Fix16_Fix16_8.f(idx);
    float val_int8_t_float_8  = test_int8_t_float_8.f(idx/10);
    float val_int16_t_float_8  = test_int16_t_float_8.f(idx);
    float val_Fix16_float_8  = test_Fix16_float_8.f(idx);

    printf("%5d: %4d %4d %4d %4d %4d %4d", idx, val_int8_t_int8_t_8, val_int16_t_int8_t_8, val_Fix16_int8_t_8, val_int8_t_int16_t_8, val_int16_t_int16_t_8, val_Fix16_int16_t_8);

    mPrintFloat(val_int8_t_Fix16_8);
    mPrintFloat(val_int16_t_Fix16_8);
    mPrintFloat(val_Fix16_Fix16_8);
    mPrintFloat(val_int8_t_float_8);
    mPrintFloat(val_int16_t_float_8);
    mPrintFloat(val_Fix16_float_8);


      Serial.println();
  }

   Serial.println( F("------------------------------------------") );
}


void loop()
{}

// Function that printf and related will use to print
int serial_putchar(char c, FILE* f) {
    if (c == '\n') serial_putchar('\r', f);
    return Serial.write(c) == 1? 0 : 1;
}

void initSerial()
{
    // Open serial port with a baud rate of BAUDRATE b/s
    Serial.begin(BAUDRATE);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------


