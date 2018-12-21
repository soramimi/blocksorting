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
#include <numeric>
#include <array>

#include "parse.h"

namespace {
  using octet = uint8_t;

  struct Sequence {
    const std::vector<octet>& _in;
    int                       _offset;

    Sequence() = delete;
    Sequence(const std::vector<octet>& in, int offset)
      : _in(in), _offset(offset) { }

    octet operator[](uint k) const
    {
      const uint idx = ( _offset + k ) % _in.size(); // size() est O(1)
      return _in[idx];
    }

    friend bool operator< (const Sequence& a, const Sequence& b)
    {
      assert(a._in.size() == b._in.size() && "error on the size");

      uint i;
      for(i = 0; i < a._in.size(); i++)
        if(a[i] == b[i])
          continue;

      return a[i] < b[i];
    }

    friend std::ostream& operator<<(std::ostream& o, const Sequence& a)
    {
      //o << "<< debut debug affichage \n[" << std::endl;
      for(uint i = 0; i < a._in.size(); i++)
        o << ( isprint(a[i] )? char(a[i]) : char(47) );
      //o << "\n]\n<< fin debug affichage" << std::endl;

      return o;
    }
  };
}

namespace bwt {

  std::vector<octet>
  encode(std::vector<octet>& dat)
  {
    std::vector<int> indices ( dat.size() );
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(indices.begin(), indices.end(),
              [&] (int lhs, int rhs) {
                Sequence a { dat, lhs };
                Sequence b { dat, rhs };
                return a < b;
              });

    uint idx = std::distance(indices.begin(),
                            std::find(indices.begin(), indices.end(), 0));

    std::clog << Sequence(dat, indices[idx]) << std::endl;
    for(uint i = 0; i < indices.size(); ++i) {
      int k = indices[i];
      std::clog << (idx==i?"> ":"") << k << " : "
                << Sequence(dat, k) << std::endl;
    }

    std::vector<octet> out ( dat.size() );

    const uint last = dat.size() - 1;
    for(uint i = 0; i < dat.size(); i++) {
      Sequence curr { dat, indices[i] };
      out[i] = curr[last];
    }

    return out;
  }

  std::vector<octet>
  decode(std::vector<octet>& dat)
  {
    auto out = dat;
    return out;
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

  // out the result to std::cout
  std::cerr << "<< end of the process" << std::endl;
  std::clog << Sequence{ out, 0 } << std::endl;

  // std::copy(out.begin(), out.end(),
  //           std::ostream_iterator<octet>(std::cout));

  return EXIT_SUCCESS;
}
