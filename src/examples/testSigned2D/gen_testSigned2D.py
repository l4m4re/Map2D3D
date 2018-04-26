#!/usr/bin/python


#sizes = [8,16]
sizes = [8]

memarraytypes = ["int8_t", "int16_t", "float"]
Xtypes        = ["int8_t", "int16_t", "Fix16" ]
Ytypes        = ["int8_t", "int16_t", "Fix16", "float" ]
#XYtypes       = ["int8_t", "int16_t", "Fix16" ]



header="""//-----------------------------------------------------------------------------
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

//#include <fix16.h>
#include <fix16.hpp>


using namespace std;

#define BAUDRATE    115200

// Stuff needed for printing
FILE                    serial_stdout;
static char             outstr[15];
#define mPrintFloat(f)  dtostrf(f, 9, 2, outstr); Serial.print(outstr);

"""

functions = """
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

"""

print header

globals = """
//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
"""

xs      = {}
ys      = {}


def get_arr_nm_idx_setter( dtype, axis, sz ):
    ar_idx = str(sz)

    if "x" in axis:
      setter = "setXs"
    else:  
      setter = "setYs"

    if dtype in ["Fix16", "float"]:
      dtype = "float"
      ar_idx += "f"
      setter += "FromFloat"

    if "8" in dtype:
      ar_idx += "_char"
    else:
      ar_idx += ""

    return axis + "_" + dtype + "_" + str(sz), ar_idx, setter


def mk_mem_array( arr, axis, sz ):
  for dtype in memarraytypes:

    name, idx, setter = get_arr_nm_idx_setter( dtype, axis, sz )  

    step  = 256/sz

    if "8" in dtype:
      multi = 1
    else:
      multi = 10

    if "int" in dtype: 
      ext = ""
    else:
      ext = ".0"

    if sz == 8:
      tabsize = 16
    else:
      tabsize = 8

    arr[idx]  = " { "

    rg = range(-128, 127, step )
    rg.append(127)
    for x in rg:
      s  = str(x*multi) + ext
      if x < 127:
        s += ","
        while len(s) < tabsize:
          s += " "
      if x == 127:
        s+= " };"
      arr[idx]  += s


def print_decl( arr, axis, sz ):
  for dtype in memarraytypes:

    name, idx, setter = get_arr_nm_idx_setter( dtype, axis, sz )  

    #decl = "const " + dtype + " xs_" + dtype + "_" + sub + "[]"  
    decl = "const " + dtype + " " + name + "[]"  
    while len(decl) < 30:
      decl += " "
    
    decl += "PROGMEM = " 

    print decl
    print arr[idx]

for size in sizes:
  mk_mem_array(xs, "xs",  size)  
  print_decl(xs, "xs", size)  
  mk_mem_array(ys, "ys",  size)  
  print_decl(ys, "ys", size)  

print functions

types = ["int8_t", "int16_t", "Fix16", "float" ]

'''    
Map2D<8, int16_t, byte>  testb;
testb.setXs_P(xs);
testb.setYs_P(ysb);

Map2D<8, uint16_t, byte>  testub;
testub.setXs_P(xs);
testub.setYs_P(ysb);
'''

for X in Xtypes:
  for Y in Ytypes:
    for size in sizes:
      table = "Map2D<" + str(size) + "," + X + "," + Y + ">"

      varname = "test_" + X + "_" + Y + "_" + str(size)
      xname, xidx, xsetter = get_arr_nm_idx_setter(  X, "xs", size )
      yname, yidx, ysetter = get_arr_nm_idx_setter(  Y, "ys", size )

      print
      print "    " + table, varname + ";"
      print "    " + varname + "." + xsetter + "_P(" + xname + ");"
      print "    " + varname + "." + ysetter + "_P(" + yname + ");"

print '''
  for( int idx=-1280; idx<=1270; idx+=10)
  {
'''

printfstr  = "%5d:"
printfargs = "idx"
mprints    = []
    
for Y in Ytypes:
  for X in Xtypes:
    for size in sizes:

      varname = "test_" + X + "_" + Y + "_" + str(size)
      xname, xidx, xsetter = get_arr_nm_idx_setter(  X, "xs", size )
      yname, yidx, ysetter = get_arr_nm_idx_setter(  Y, "ys", size )
    
      tempvartp = Y
      if tempvartp in ["Fix16", "float"]: 
        tempvartp = "float"

      fidx = ".f(idx);"
      if "8" in X:
        fidx = ".f(idx/10);"

      tempvarnm = "val_" + X + "_" + Y + "_" + str(size)

      print "    " + tempvartp , tempvarnm , " = " + varname + fidx

      if tempvartp in ["Fix16", "float"]:
         mprints.append( tempvarnm )
      else:
         printfstr  += " %4d"
         printfargs += ", " + tempvarnm

print
print '    printf("' + printfstr + '", ' + printfargs + ");"
print

for mprint in mprints:
  print "    mPrintFloat(" + mprint + ");"


'''
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
'''



footer = """

      Serial.println();
  }

   Serial.println( F("------------------------------------------") );
}


void loop()
{}

// Function that printf and related will use to print
int serial_putchar(char c, FILE* f) {
    if (c == '\\n') serial_putchar('\\r', f);
    return Serial.write(c) == 1? 0 : 1;
}

void initSerial()
{
    // Open serial port with a baud rate of BAUDRATE b/s
    Serial.begin(BAUDRATE);
    
    // Set up stdout
    fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &serial_stdout;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

"""

print footer


