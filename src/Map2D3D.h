//-----------------------------------------------------------------------------
// 2D and 3D Maps c.q. lookup tables with (bi)linear interpolation
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
// Description:
//
// Maps or lookup tables (LUTs) are used to approximate a multidimensional
// function y = f(X), with y a single dimensional value and X an n dimensional
// vector X = (x1, x2, ... xn).
//
// For the one dimensional case, two arrays are used to store a number of
// known values for y = f(x), as follows::
//
//   ys[i] = f( xs[i] )
//     
// For the two dimensional case, three arrays are used to store a number of
// known values for y = f(X) = f(x1,x2), as follows::
//
//   ys[i][j] = f( x1s[i], x2s[j] )
//
// In order to approximate f(X) at any point within the range of stored
// values, we search the xs array(s) to find the nearest known X-es and
// lineary interpolate the known result stored in the ys array.    
//
//-----------------------------------------------------------------------------
// 
// Based on:
//
// Set array size at compile time thru constructor
// http://www.cplusplus.com/forum/general/114406/
//
// How to Search an Ordered Table:
// http://www.aip.de/groups/soe/local/numres/bookcpdf/c3-4.pdf
//
// Putting constant data into program memory (PROGMEM)
// http://www.gammon.com.au/progmem
//
//-----------------------------------------------------------------------------

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
    #define dprint(expression) Serial.print(F("# ")); Serial.print( F(#expression) ); \
                               Serial.print( F(": ") ); Serial.println( expression )
    #define dshow(expression) Serial.println( expression )
#else
    #define dprint(expression) 
    #define dshow(expression)
#endif


//-----------------------------------------------------------------------------
// 2D lookup table / fuel map. X axis (xs) must be sorted in ascending order.
//-----------------------------------------------------------------------------

template<int S, typename X, typename Y> // S: size, X,Y: data types
class Table2D
{
public:

    int           sizeX()               { return S; }

    void          setXs( const X* xss ) 
                        { memcpy( xs, xss, S*sizeof(X) ); }

    void          setYs( const Y* yss )
                        { memcpy( ys, yss, S*sizeof(Y) ); }

    Y             f( X x )              // approximate f(x)
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

                    return interpolate( x, xs[i], xs[i+1], ys[i], ys[i+1]); 

                  }

//protected:

    X             xs[S];
    Y             ys[S];

};


//-----------------------------------------------------------------------------
// 2D lookup table / fuel map with int16 for the x axis.
//-----------------------------------------------------------------------------

template<int S, typename Y>  // S: determine size at compile time, T: data type
class iTable2D: public Table2D<S, int16_t, Y>
{

public:

    void          setYsFromFloat( const float* yss ) 
                  {
                      for( int i=0; i<S; i++ ) { this->ys[i] = static_cast<Y>(yss[i]); }
                  }

#ifdef ARDUINO    // Initialization from array in PROGMEM

    void          setXs_P( const int16_t* xss ) 
                        { memcpy_P( this->xs, xss, S*sizeof(int16_t) ); }

    void          setYs_P( const Y* yss )
                        { memcpy_P( this->ys, yss, S*sizeof(Y) ); }
                        
    void          setYsFromFloat_P( const float* yss )
                  {
                      for( int i=0; i<S; i++ ) 
                          { this->ys[i] = static_cast<Y>( pgm_read_float_near(yss+i) ); }
                  }
#endif
};



//-----------------------------------------------------------------------------
// 3D lookup table / fuel map. X axis must be sorted in ascending order.
//-----------------------------------------------------------------------------

template<int R, int S, typename X, typename Y> // R,S: size, Y,Y: data type
class Table3D
{
public:

    int           sizeX1()                { return R; }
    int           sizeX2()                { return S; }


    void          setX1s( const X* x1ss ) 
                        { memcpy( x1s, x1ss, R*sizeof(X) ); }

    void          setX2s( const X* x2ss ) 
                        { memcpy( x2s, x2ss, S*sizeof(X) ); }

    void          setYs( const Y* yss )
                        { memcpy( ys, yss, R*S*sizeof(Y) ); }

    Y             f( X x1, X x2 )
                  {    
                    if (x1 < x1s[0])      { x1 = x1s[0];   } // minimum
                    if (x1 > x1s[R-1])    { x1 = x1s[R-1]; } // maximum
                    if (x2 < x2s[0])      { x2 = x2s[0];   } // minimum
                    if (x2 > x2s[S-1])    { x2 = x2s[S-1]; } // maximum

                    int i=0, j=R, m;

                    // find i, such that x1s[i] <= x1 < x1s[j] using bisection
                    while ( j - i > 1) {                      
                        m = (i+j) >> 1;     // m = (i+j)/2
                        
                        if ( x1 >= x1s[m] ) i = m;
                        else                j = m;
                    }

                    int k=S; 
                    j=0;
                    
                    // find j, such that x2s[j] <= x2 < x2s[k] using bisection
                    while ( k - j > 1) {                      
                        m = (j+k) >> 1;     // m = (j+k)/2
                        
                        if ( x2 >= x2s[m] ) j = m;
                        else                k = m;
                    }

                    return interpolate( x1,       x2, 
                                        x1s[i],   x1s[i+1],   x2s[j],       x2s[j+1],
                                        ys[i][j], ys[i+1][j], ys[i+1][j+1], ys[i][j+1]);   
                  }

private:

    X             x1s[S];
    X             x2s[S];
    Y             ys[R][S];

};


//-----------------------------------------------------------------------------
// 3D lookup table / fuel map with int16 for the x axis.
//-----------------------------------------------------------------------------

template<int R, int S, typename Y>  // R,S: size, Y: data type
class iTable3D: public Table3D<R, S, int16_t, Y>
{

public:

    void          setYsFromFloat( const float* yss ) 
                  {
                      for( int i=0; i<R*S; i++ ) 
                        { this->ys[i] = static_cast<Y>(yss[i]); }
                  }

#ifdef ARDUINO    // Initialize from array in PROGMEM

    void          setX1s_P( const int16_t* x1ss ) 
                        { memcpy_P( this->x1s, x1ss, R*sizeof(int16_t) ); }

    void          setX2s_P( const int16_t* x2ss ) 
                        { memcpy_P( this->x2s, x2ss, S*sizeof(int16_t) ); }

    void          setYs_P( const Y* yss )
                        { memcpy_P( this->ys, yss, R*S*sizeof(Y) ); }
                        
    void          setYsFromFloat_P( const float* yss )
                  {
                      for( int i=0; i<R*S; i++ ) 
                          { this->ys[i] = static_cast<Y>( pgm_read_float_near(yss+i) ); }
                  }
#endif

};

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

#endif // End multi-include protection
