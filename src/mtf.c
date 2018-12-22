#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <climits>
#include <cassert>

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <utility>
#include <numeric>
#include <array>

#include "parse.h"

namespace bwt {
  //
  // Encode
  //
  std::vector<octet>
  encode(const std::vector<octet>& dat)
  {
    std::vector<octet> alph (256);
    std::iota(alph.begin(), alph.end(), 0);

    std::vector<octet> out;
    out.reserve(dat.size());

    for(octet oct: dat) {
      int idx = std::distance(alph.begin(),
                              std::find(alph.begin(), alph.end(), oct)); // O(n)
      out.emplace_back(idx);
      std::iter_swap(alph.begin(), alph.begin() + idx);
    }

    return out; // rvo
  }

  //
  // Decode
  //
  std::vector<octet>
  decode(std::vector<octet>& dat)
  {
    std::vector<octet> alph (256);
    std::iota(alph.begin(), alph.end(), 0);

    std::vector<octet> out;
    out.reserve(dat.size());

    for(octet oct: dat) {
      out.emplace_back(alph[oct]);
      std::iter_swap(alph.begin(), alph.begin() + oct);
    }

    return out; // rvo
  }
}

int main( int argc, char *argv[] )
{
  Parameter<octet> input = parse<octet>(argc, argv);

  std::vector<octet> out;

  if (input.decode)
    out = bwt::decode(input.dat);
  else
    out = bwt::encode(input.dat);

  std::copy(out.begin(), out.end(),
            std::ostream_iterator<octet>(std::cout));

  return EXIT_SUCCESS;
}
