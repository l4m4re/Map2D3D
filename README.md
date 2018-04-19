# Map2D3D

Arduino library for 2d and 3D fuel maps c.q. lookup tables as used in
automotive engine control units.

This library is written for Arduino, but there is very little specific Arduino
code in there, so it should be easy to adapt for other platforms.

Support for integer and byte tables (small footprint and much faster than
floats on a platform without FPU) requires libfixmath.

See:

https://en.wikipedia.org/wiki/Libfixmath

https://github.com/PetteriAimonen/libfixmath

And my repository for an Arduino version:

https://github.com/l4m4re/Arduino_fixpt

