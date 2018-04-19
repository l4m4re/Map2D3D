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
#include "interpolate.h"

//-----------------------------------------------------------------------------
// Debug stuff
//-----------------------------------------------------------------------------

#define DEBUG            1

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
//
// http://bit.ly/LUT_c_linear_interpolation
// http://www.cplusplus.com/forum/general/114406/
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 2D 2D lookup table / fuel map. X axis xs must be sorted in ascending order.
//-----------------------------------------------------------------------------

template<int S, typename T>  // S: determine size at compile time, T: data type
class Table2D
{
public:

    int           size()                { return S; }

    void          setXs( const int16_t* xss ) 
                        { memcpy( xs, xss, S*sizeof(int16_t) ); }

    void          setYs( const T* yss )
                        { memcpy( ys, yss, S*sizeof(T) ); }

    void          setYsFromFloat( const float* yss ) 
                  {
                      for( int i=0; i<S; i++ ) { ys[i] = static_cast<T>(yss[i]); }
                  }

#ifdef ARDUINO    // Initialize from array in PROGMEM
    void          setXs_P( const int16_t* xss ) 
                        { memcpy_P( xs, xss, S*sizeof(int16_t) ); }

    void          setYs_P( const T* yss )
                        { memcpy_P( ys, yss, S*sizeof(T) ); }
                        
    void          setYsFromFloat_P( const float* yss )
                  {
                      for( int i=0; i<S; i++ ) 
                          { ys[i] = static_cast<T>( pgm_read_float_near(yss+i) ); }
                  }
#endif

    T             getValue(int16_t x)
                  {    
                    if (x < xs[0])      { return ys[0];   } // minimum
                    if (x > xs[S-1])    { return ys[S-1]; } // maximum

                    int i=0, j=S, k;

                    /* find i, such that xs[i] <= x < xs[j] */
                    while ( j - i > 1) {                      
                        k = (i+j) >> 1;  // k = (i+j)/2
                        
                        if ( x >= xs[k] )   i = k;
                        else                j = k;
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
