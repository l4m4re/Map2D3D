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
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
// Multi-include protection
//-----------------------------------------------------------------------------

#ifndef _LOOKUP_TABLE_2D3D
#define _LOOKUP_TABLE_2D3D

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <Arduino.h>

//-----------------------------------------------------------------------------
// Debug stuff
//-----------------------------------------------------------------------------

//#define DEBUG            1

#if DEBUG == 1
    #define dprint(expression) Serial.print(F("# ")); Serial.print( F(#expression) ); Serial.print( F(": ") ); Serial.println( expression )
    #define dshow(expression) Serial.println( expression )
#else
    #define dprint(expression) 
    #define dshow(expression)
#endif

//-----------------------------------------------------------------------------
// 
// Based on:
// https://stackoverflow.com/questions/7091294/how-to-build-a-lookup-table-in-c-sdcc-compiler-with-linear-int16_terpolation
// http://www.cplusplus.com/forum/general/114406/
// 
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Linear interpolation with specialization for integer arithmetic 
//-----------------------------------------------------------------------------

template <class T>
T interpolate( int16_t x, int16_t x1, int16_t x2, T y1, T y2 )
{
    int16_t   dx  = x2-x1;
    T         dy  = y2-y1;
    
//  return y1 + (x-x1) * (dy/dx); // avoid ambigious operator overloading:

    T       delta = x-x1;
    T       dydx  = dy/dx;

    return y1 + delta * dydx;
}


// For integer type lookup tables, we incorporate fractions by
// casting into an int32 and shift by 6 bits.

template<>
uint8_t interpolate( int16_t   x, int16_t  x1, int16_t x2, 
                      uint8_t  y1, uint8_t y2 )
{
    int32_t   _y1 = (static_cast<int32_t>(y1))<<6;
    int32_t   _y2 = (static_cast<int32_t>(y2))<<6;

    int16_t  dx = x2-x1;
    int32_t   dy = _y2-_y1;

    dprint(F("uint8_t"));
    
    return y1 + (( (x-x1) * (dy/dx) )>>6);
}


template<>
int8_t interpolate( int16_t x, int16_t x1, int16_t x2, 
                    int8_t  y1, int8_t y2 )
{
    int32_t   _y1 = (static_cast<int32_t>(y1))<<6;
    int32_t   _y2 = (static_cast<int32_t>(y2))<<6;

    int16_t  dx = x2-x1;
    int32_t   dy = _y2-_y1;

    dprint(F("int8_t"));

    return y1 + (( (x-x1) * (dy/dx) )>>6);
}


template<>
uint16_t interpolate( int16_t x, int16_t x1, int16_t x2, 
                          uint16_t y1, uint16_t y2 )
{
    int32_t   _y1 = (static_cast<int32_t>(y1))<<6;
    int32_t   _y2 = (static_cast<int32_t>(y2))<<6;

    int16_t  dx = x2-x1;
    int32_t   dy = _y2-_y1;

    dprint(F("uint16_t"));

    return y1 + (( (x-x1) * (dy/dx) )>>6);
}


template<>
int16_t interpolate( int16_t  x, int16_t x1, int16_t x2, 
                          int16_t y1, int16_t y2 )
{
    int32_t   _y1 = (static_cast<int32_t>(y1))<<6;
    int32_t   _y2 = (static_cast<int32_t>(y2))<<6;

    int16_t  dx = x2-x1;
    int32_t   dy = _y2-_y1;

    dprint(F("int16_t"));

    return y1 + (( (x-x1) * (dy/dx) )>>6);
}



template<int S, typename T>  // S: determine size at compile time, T: data type
class Table2D
{
public:

    int           size()                { return S; }

    void          setXs( const int16_t* xss ) 
                        { memcpy( xs, xss, S*sizeof(int16_t) ); }

    void          setYs( const T* yss )
                        { memcpy( ys, yss, S*sizeof(T) ); }

#ifdef ARDUINO    // Initialize from array in PROGMEM
    void          setXs_P( const int16_t* xss ) 
                        { memcpy_P( xs, xss, S*sizeof(int16_t) ); }

    void          setYs_P( const T* yss )
                        { 
                          dprint( S );  
                          dprint( sizeof(T) );  
                          dprint( S*sizeof(T) );  
                          memcpy_P( ys, yss, S*sizeof(T) ); 
                        }
#endif

    T             getValue(int16_t x)
                  {    
                    int i;
 
                    if (x < xs[0])      { return ys[0];   } // minimum
                    if (x > xs[S-1])    { return ys[S-1]; } // maximum

                    /* find i, such that xs[i] <= x < xs[i+1] */
                    for (i = 0; i < S-1; i++) {
                        if (xs[i+1] > x) {
                            break;
                        }
                    }

                    return interpolate( x, xs[i], xs[i+1], ys[i], ys[i+1] );   
                  }

private:

    int16_t       xs[S];
    T             ys[S];

};




//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

#endif // End multi-include protection
