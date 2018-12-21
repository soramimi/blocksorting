#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <climits>
#include <cassert>

#include <iostream>
#include <vector>
#include <array>

#include "parse.h"

namespace {
  using octet = uint8_t;
}

int main( int argc, char *argv[] )
{
  auto input = parse<octet>(argc, argv);

  for(auto o: input)
    std::clog << o;

  return EXIT_SUCCESS;
}
