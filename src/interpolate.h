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
// 
// (Bi)Linear interpolation in 2D and 3D 
//
// Based on:
//
// http://bit.ly/LUT_c_linear_interpolation
// http://bit.ly/Interpolate2D
// http://bit.ly/biliniar_barycentric_interpolation
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <stdint.h>

#ifdef SUPPORT_INTEGER_ARITMETHIC
# include <fix16.hpp>
#endif

//-----------------------------------------------------------------------------
// 2D linear interpolation
//-----------------------------------------------------------------------------

template <class T>
inline T interpolate( int16_t x,           // target point
                      int16_t x1, T y1,    // point left 
                      int16_t x2, T y2 )   // point right
{
  int16_t   dx  = x2-x1;
  T         dy  = y2-y1;
  
//  return y1 + (x-x1) * (dy/dx); // avoid ambigious operator overloading:

  T       delta = x-x1;
  T       dydx  = dy/dx;

  return y1 + delta * dydx;
}


//-----------------------------------------------------------------------------
// 3D bilinear interpolation
//-----------------------------------------------------------------------------

template <class T>
inline T interpolate( int16_t x,  int16_t y, 
                      int16_t x1, int16_t x2, 
                      int16_t y1, int16_t y2, 
                      T z1, T z2 )
{
  int16_t   dx  = x2-x1;
  int16_t   dy  = y2-y1;
  T         dz  = z2-z1;
  
//  return z1 + (x-x1)*(dz/dx) + (y-y1)*(dz/dy); // avoid ambig oper overload:

  T         deltax = x-x1;
  T         deltay = y-y1;
  T         dzdx  = dz/dx;
  T         dzdy  = dz/dy;

  return z1 + deltax * dzdx + deltay * dzdy;
}

#ifdef SUPPORT_INTEGER_ARITMETHIC

//-----------------------------------------------------------------------------
// 2D specialization for integers by casting to Fix16 
//-----------------------------------------------------------------------------

template<>
inline int8_t interpolate( int16_t x, 
                           int16_t x1, int8_t y1, 
                           int16_t x2, int8_t y2 )
{
  return static_cast<int16_t>( interpolate( x, 
                                           x1, static_cast<Fix16>(y1),  
                                           x2, static_cast<Fix16>(y2)  ));
}

template<>
inline int16_t interpolate( int16_t x, 
                            int16_t x1, int16_t y1, 
                            int16_t x2, int16_t y2 )
{
  return static_cast<int16_t>( interpolate( x, 
                                           x1, static_cast<Fix16>(y1),  
                                           x2, static_cast<Fix16>(y2)  ));
}


//-----------------------------------------------------------------------------
// 3D specialization for integers by casting to Fix16 
//-----------------------------------------------------------------------------

template<>
inline int8_t interpolate( int16_t x , int16_t y,
                           int16_t x1, int16_t x2, 
                           int16_t y1, int16_t y2, 
                           int8_t  z1, int8_t  z2 )
{
  return static_cast<int16_t>( interpolate(x, y, x1, x2, y1, y2,
                                static_cast<Fix16>(z1), static_cast<Fix16>(z2) ));
}

template<>
inline int16_t interpolate( int16_t x , int16_t y,
                            int16_t x1, int16_t x2, 
                            int16_t y1, int16_t y2, 
                            int16_t z1, int16_t z2 )
{
  return static_cast<int16_t>( interpolate(x, y, x1, x2, y1, y2,
                                static_cast<Fix16>(z1), static_cast<Fix16>(z2) ));
}

#endif // SUPPORT_INTEGER_ARITMETHIC

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

#endif // End multi-include protection
