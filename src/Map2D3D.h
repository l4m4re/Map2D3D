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
// Defines
//-----------------------------------------------------------------------------

// Requires Fix16 library: https://github.com/l4m4re/Arduino_fixpt
#define SUPPORT_INTEGER_ARITMETHIC 1  

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <stdint.h>
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
// http://www.cplusplus.com/forum/general/114406/
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 2D lookup table / fuel map. X axis xs must be sorted in ascending order.
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

#ifdef ARDUINO    // Initialization from array in PROGMEM
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

                    // find i, such that xs[i] <= x < xs[j] using bisection
                    while ( j - i > 1) {                      
                        k = (i+j) >> 1;  // k = (i+j)/2
                        
                        if ( x >= xs[k] )   i = k;
                        else                j = k;
                    }

                    return interpolate( x, xs[i], ys[i], xs[i+1], ys[i+1] );   
                  }

private:

    int16_t       xs[S];
    T             ys[S];

};


//-----------------------------------------------------------------------------
// 3D lookup table / fuel map. X and Y axis must be sorted in ascending order.
//-----------------------------------------------------------------------------

template<int R, int S, typename T>  // R,S: size (compile time), T: data type
class Table3D
{
public:

    int           sizeX()                { return R; }
    int           sizeY()                { return S; }


    void          setXs( const int16_t* xss ) 
                        { memcpy( xs, xss, R*sizeof(int16_t) ); }

    void          setYs( const int16_t* yss ) 
                        { memcpy( ys, yss, S*sizeof(int16_t) ); }

    void          setZs( const T* zss )
                        { memcpy( zs, zss, R*S*sizeof(T) ); }

    void          setZsFromFloat( const float* zss ) 
                  {
                      for( int i=0; i<R*S; i++ ) 
                        { zs[i] = static_cast<T>(zss[i]); }
                  }

#ifdef ARDUINO    // Initialize from array in PROGMEM
    void          setXs_P( const int16_t* xss ) 
                        { memcpy_P( xs, xss, R*sizeof(int16_t) ); }

    void          setYs_P( const int16_t* yss ) 
                        { memcpy_P( ys, yss, S*sizeof(int16_t) ); }

    void          setZs_P( const T* zss )
                        { memcpy_P( zs, zss, R*S*sizeof(T) ); }
                        
    void          setZsFromFloat_P( const float* zss )
                  {
                      for( int i=0; i<R*S; i++ ) 
                          { zs[i] = static_cast<T>( pgm_read_float_near(zss+i) ); }
                  }
#endif

    T             getValue(int16_t x, int16_t y)
                  {    
                    if (x < xs[0])      { x = xs[0];   } // minimum
                    if (x > xs[R-1])    { x = xs[R-1]; } // maximum
                    if (y < ys[0])      { x = ys[0];   } // minimum
                    if (y > ys[S-1])    { x = ys[S-1]; } // maximum

                    int i=0, j=R, m;

                    // find i, such that xs[i] <= x < xs[j] using bisection
                    while ( j - i > 1) {                      
                        m = (i+j) >> 1;  // m = (i+j)/2
                        
                        if ( x >= xs[m] )   i = m;
                        else                j = m;
                    }

                    int k=S; 
                    j=0;
                    
                    // find j, such that ys[j] <= y < ys[k] using bisection
                    while ( k - j > 1) {                      
                        m = (j+k) >> 1;  // m = (i+j)/2
                        
                        if ( y >= ys[m] )   j = m;
                        else                k = m;
                    }

                    return interpolate( x, y, 
                                        xs[i], xs[i+1], 
                                        ys[j], ys[j+1],
                                        zs[i][j], zs[i+1][j+1] );   
                  }

private:

    int16_t       xs[S];
    int16_t       ys[S];
    T             zs[R][S];

};


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

#endif // End multi-include protection
