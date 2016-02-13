#include <cstdlib>
#include <iostream>

#include "et.h"

int main( int argc, char **argv ){
  
  et::vector<char> B(20);
  et::vector<double> C(20);
  for( int i=0; i<B.size(); ++i ){
    B[i] = i;
    C[i] = drand48()*0.01;
  }
  
  auto res = 1.05 + 0.5f*C + B*int(2) - 2;
  std::cout << res << std::endl;
  
  return 0;
}