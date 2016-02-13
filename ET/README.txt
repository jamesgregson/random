========================================================
et: vector expression templates in C++
(c) James Gregson 2016
james.gregson@gmail.com

License
======================================================== 
Use for whatever you want, at your own risk. Bug reports and feature requests are welcome.

Description 
======================================================== 
The file et.h implements a basic vector expression template library in C++ that uses basic template metaprogramming and SFINAE to implement a mathematical vector class. It is meant as a single-file example to (some of) these techniques that is heavily commented, rather than as a full-fledged linear algebra library. It supports the following operations:

vector = op( vector, vector )
  op: +, -, *, /

vector = op( vector, scalar )
  op: +, -, *, /

vector = op( scalar, vector )
  op: +, -, *

Also included are some rudimentary type promotion rules on signed integers and float/double real values, allowing expressions of mixed type to be promoted.

The library also makes use of SFINAE to define operations only on numeric types.