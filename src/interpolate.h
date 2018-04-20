//-----------------------------------------------------------------------------
// (Bi)Linear interpolation in 2D and 3D 
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
// Based on:
//
// Interpolation in Two or More Dimensions
// http://www.aip.de/groups/soe/local/numres/bookcpdf/c3-6.pdf
//
// How to build a lookup table in C (SDCC compiler) with linear interpolation
// http://bit.ly/LUT_c_linear_interpolation
//
// Linear interpolation: calculate correction based on 2D table
// http://bit.ly/Interpolate2D
//
// 2D Array Interpolation
// http://bit.ly/biliniar_barycentric_interpolation
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Multi-include protection
//-----------------------------------------------------------------------------

#ifndef _INTERPOL_H
#define _INTERPOL_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <stdint.h>

#ifdef SUPPORT_INTEGER_ARITMETHIC
# include <fix16.hpp>
#endif

//-----------------------------------------------------------------------------
// 1D linear interpolation
//-----------------------------------------------------------------------------

template <class T>
inline T interpolate( int16_t x, int16_t x_1, int16_t x_2, T y_1, T y_2 ) 
{
  T   one   = 1.0f; // avoid ambigious operator overload

  // cast integer indexes to T
  T   _x    = x;
  T   _x_1  = x_1;
  T   _x_2  = x_2;
  
  T   dx    = (_x - _x_1) / (_x_2 - _x_1);  //  0 <= dx <= 1

  return (one-dx)*y_1 + dx*y_2;
}


//-----------------------------------------------------------------------------
// 2D bilinear interpolation
//-----------------------------------------------------------------------------

template <class T>
inline T interpolate( int16_t x1,  int16_t x2, 
                      int16_t x_1, int16_t x_2, int16_t x_3, int16_t x_4,
                            T y_1,       T y_2,       T y_3,       T y_4 )
{
  T   one   = 1.0f; // avoid ambigious operator overload

  // cast integer indexes to T
  T   _x1   = x1;
  T   _x2   = x2;
  T   _x_1  = x_1;
  T   _x_2  = x_2;
  T   _x_3  = x_3;
  T   _x_4  = x_4;
  
  T   dx1   = (_x1 - _x_1) / (_x_2 - _x_1);  // 0 <= dx1 <= 1
  T   dx2   = (_x2 - _x_3) / (_x_4 - _x_3);  // 0 <= dx2 <= 1

  return (one-dx1)*(one-dx2)*y_1 + dx1*(one-dx2)*y_2 + 
                     dx1*dx2*y_3 + (one-dx1)*dx2*y_4;
}

#ifdef SUPPORT_INTEGER_ARITMETHIC

//-----------------------------------------------------------------------------
// 1D specialization for integers by casting to Fix16 
//-----------------------------------------------------------------------------

template<>
inline int8_t interpolate( int16_t x,  int16_t x_1, int16_t x_2, 
                           int8_t  y_1, int8_t y_2 )
{
  return static_cast<int16_t>( interpolate( x, x_1, x_2, 
                                           static_cast<Fix16>(y_1),  
                                           static_cast<Fix16>(y_2)  ));
}

template<>
inline uint8_t interpolate( int16_t x,   int16_t x_1, int16_t x_2, 
                            uint8_t y_1, uint8_t y_2 )
{
  return static_cast<int16_t>( interpolate( x, x_1, x_2, 
                                           static_cast<Fix16>(y_1),  
                                           static_cast<Fix16>(y_2)  ));
}

template<>
inline int16_t interpolate( int16_t x,   int16_t x_1, int16_t x_2, 
                            int16_t y_1, int16_t y_2 )
{
  return static_cast<int16_t>( interpolate( x, x_1, x_2, 
                                           static_cast<Fix16>(y_1),  
                                           static_cast<Fix16>(y_2)  ));
}


//-----------------------------------------------------------------------------
// 2D specialization for integers by casting to Fix16 
//-----------------------------------------------------------------------------

template<>
inline int8_t interpolate( int16_t x1,  int16_t x2,   
                           int16_t x_1, int16_t x_2, int16_t x_3, int16_t x_4,
                            int8_t y_1,  int8_t y_2,  int8_t y_3,  int8_t y_4 )
{
  return static_cast<int16_t>( interpolate(x1, x2, x_1, x_2, x_3, x_4,
                                static_cast<Fix16>(y_1), static_cast<Fix16>(y_2), 
                                static_cast<Fix16>(y_3), static_cast<Fix16>(y_4) ));
}

template<>
inline uint8_t interpolate( int16_t x1,  int16_t x2,   
                           int16_t x_1, int16_t x_2, int16_t x_3, int16_t x_4,
                           uint8_t y_1, uint8_t y_2, uint8_t y_3, uint8_t y_4 )
{
  return static_cast<int16_t>( interpolate(x1, x2, x_1, x_2, x_3, x_4,
                                static_cast<Fix16>(y_1), static_cast<Fix16>(y_2), 
                                static_cast<Fix16>(y_3), static_cast<Fix16>(y_4) ));
}

template<>
inline int16_t interpolate( int16_t x1,  int16_t x2,   
                            int16_t x_1, int16_t x_2, int16_t x_3, int16_t x_4,
                            int16_t y_1, int16_t y_2, int16_t y_3, int16_t y_4 )
{
  return static_cast<int16_t>( interpolate(x1, x2, x_1, x_2, x_3, x_4,
                                static_cast<Fix16>(y_1), static_cast<Fix16>(y_2), 
                                static_cast<Fix16>(y_3), static_cast<Fix16>(y_4) ));
}

#endif // SUPPORT_INTEGER_ARITMETHIC

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

#endif // End multi-include protection
