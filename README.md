# Map2D3D

Arduino library for 2d and 3D fuel maps c.q. lookup tables as used in
automotive engine control units.

This library is written for Arduino, but there is very little specific Arduino
code in there, so it should be easy to adapt for other platforms.

Support for integer and byte tables (small footprint and much faster than
floats on a platform without FPU) requires libfixmath.

See:

[https:en.wikipedia.org/wiki/Libfixmath](https:en.wikipedia.org/wiki/Libfixmath)

[https:github.com/PetteriAimonen/libfixmath](https:github.com/PetteriAimonen/libfixmath)

And my repository for an Arduino version:

[https:github.com/l4m4re/Arduino_fixpt](https:github.com/l4m4re/Arduino_fixpt)


Description:
------------

Maps or lookup tables (LUTs) are used to approximate a multidimensional
function y = f(X), with y a single dimensional value and X an n dimensional
vector X = (x1, x2, ... xn). Usually, this technique is used to either
speed up te computation of computationally expensive functions (such as
sin, cos or tan) or to approximate functions with unkown characteristics.

For the one dimensional case, two arrays are used to store a number of
known values for y = f(x), as follows::

  ys[i] = f( xs[i] )
    
For the two dimensional case, three arrays are used to store a number of
known values for y = f(X) = f(x1,x2), as follows::

  ys[i][j] = f( x1s[i], x2s[j] )

In order to approximate f(X) at any point within the range of stored
values, we search the xs array(s) to find the nearest known X-es and
lineary interpolate the known result stored in the ys array.    

In the case of control systems for automotive applications, the
characteristics of an actual engine cannot be easily computed analytically,
but it is possible to measure the behavior of an engine and use that to
compute a table of a number of known function values for a number of known
input values, which can then be used to approximate the function.

An example of such a function is the Volumetric Efficiency (VE) of an
engine as a function of RPM and the pressure in the inlet manifold:

[https:en.wikipedia.org/wiki/Volumetric_efficiency](https:en.wikipedia.org/wiki/Volumetric_efficiency)

"Volumetric efficiency in an internal combustion engine design refers to
the efficiency with which the engine can move the charge of fuel and air
into and out of the cylinders. It also denotes the ratio of air volume
drawn into the cylinder to the cylinder's swept volume."

In this case, the 2 dimensional function to approximate would be: 

   ve = f( rpm, p )  

While this is a 2 dimensional function with respect to the number of input
parameters, it defines a relation between 3 parameters (including the
output) and thus it is common to refer to such a mapping as a 3D map, often
called a "fuel map" in the automotive world. Such maps are extensively used
in engine control systems and largely determine the behavior of the control
system and therefore play a most important role in engine tuning. For a
short introduction in engine tuning and how maps/tables are vizualised and
edited, see:

  (https:speeduino.com/wiki/index.php/Tuning)  



