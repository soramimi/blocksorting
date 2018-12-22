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

namespace rle {
  //
  // Encode
  //
  std::vector<octet>
  encode(const std::vector<octet>& dat)
  {
    std::vector<octet> out;
    out.reserve(dat.size());

    uint i = 0;
    while(i < dat.size())
    {
      octet val = dat[i];
      if(val != 0) {
        out.emplace_back(val);
        i++;
      }
      else
      {
        octet nbzeros = 0;
        while(val == 0 && i < dat.size() && nbzeros < 255)
        {
          i++;
          nbzeros++;
          val = dat[i];
        }

        out.emplace_back(0);
        out.emplace_back(nbzeros);
        // std::clog << "\n=+=\t["
        //           << int(nbzeros)
        //           << "]"
        //           << std::endl;
      }
    }

    return out; // rvo
  }

  //
  // Decode
  //
  std::vector<octet>
  decode(std::vector<octet>& dat)
  {
    // std::clog << __PRETTY_FUNCTION__ << std::endl;
    std::vector<octet> out;
    out.reserve(dat.size());

    uint i = 0;
    while(i < dat.size()) {
      if(dat[i] == 0) {
        auto nbzeros = int(dat[++i]);
        // std::clog << "\n=+=\t["
        //           << int(nbzeros)
        //           << "]"
        //           << std::endl;

        for(int k = 0; k < nbzeros; ++k)
          out.emplace_back(0);
      }
      else
        out.emplace_back(dat[i]);

      ++i;
    }
    return out; // rvo
  }
}

int main( int argc, char *argv[] )
{
  Parameter<octet> input = parse<octet>(argc, argv);

  std::vector<octet> out;

  if (input.decode)
    out = rle::decode(input.dat);
  else
    out = rle::encode(input.dat);

  std::copy(out.begin(), out.end(),
            std::ostream_iterator<octet>(std::cout));

  return EXIT_SUCCESS;
}
