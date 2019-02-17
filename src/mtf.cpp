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

namespace mtf {
  inline
  void move_to_front(std::vector<octet>& in, int idx)
  {
    const octet tmp = in[idx];
    for(int i = idx - 1; i >= 0; i--)
      in[i+1] = in[i];
    in[0] = tmp;
  }

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
      move_to_front(alph, idx);
      out.emplace_back(idx);
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
      move_to_front(alph, oct);
    }

    return out; // rvo
  }
}

int main( int argc, char *argv[] )
{
  Parameter<octet> input = parse<octet>(argc, argv);

  std::vector<octet> out;

  if (input.decode)
    out = mtf::decode(input.dat);
  else
    out = mtf::encode(input.dat);

  std::copy(out.begin(), out.end(),
            std::ostream_iterator<octet>(std::cout));

  return EXIT_SUCCESS;
}
