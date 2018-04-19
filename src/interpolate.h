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

#ifndef _INTERPOL_H
#define _INTERPOL_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <Arduino.h>

//-----------------------------------------------------------------------------
// 2D linear interpolation with specialization for integer arithmetic
//-----------------------------------------------------------------------------

template <class T>
inline T interpolate( int16_t x, int16_t x1, int16_t x2, T y1, T y2 )
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
inline uint8_t interpolate( int16_t x, int16_t  x1, int16_t x2, 
                           uint8_t y1, uint8_t y2 )
{
    int32_t   _y1 = (static_cast<int32_t>(y1))<<6;
    int32_t   _y2 = (static_cast<int32_t>(y2))<<6;

    int16_t  dx = x2-x1;
    int32_t   dy = _y2-_y1;

    return y1 + (( (x-x1) * (dy/dx) )>>6);
}


template<>
inline int8_t interpolate( int16_t x, int16_t x1, int16_t x2, 
                           int8_t y1, int8_t y2 )
{
    int32_t   _y1 = (static_cast<int32_t>(y1))<<6;
    int32_t   _y2 = (static_cast<int32_t>(y2))<<6;

    int16_t  dx = x2-x1;
    int32_t   dy = _y2-_y1;

    return y1 + (( (x-x1) * (dy/dx) )>>6);
}


template<>
inline uint16_t interpolate( int16_t x, int16_t x1, int16_t x2, 
                           uint16_t y1, uint16_t y2 )
{
    int32_t   _y1 = (static_cast<int32_t>(y1))<<6;
    int32_t   _y2 = (static_cast<int32_t>(y2))<<6;

    int16_t  dx = x2-x1;
    int32_t   dy = _y2-_y1;

    return y1 + (( (x-x1) * (dy/dx) )>>6);
}


template<>
inline int16_t interpolate( int16_t  x, int16_t x1, int16_t x2, 
                            int16_t y1, int16_t y2 )
{
    int32_t   _y1 = (static_cast<int32_t>(y1))<<6;
    int32_t   _y2 = (static_cast<int32_t>(y2))<<6;

    int16_t  dx = x2-x1;
    int32_t   dy = _y2-_y1;

    return y1 + (( (x-x1) * (dy/dx) )>>6);
}



//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

#endif // End multi-include protection
