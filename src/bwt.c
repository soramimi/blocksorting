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
      const uint idx = ( _offset + k ) % _in.size(); // size() O(1)
      return _in[idx];
    }

    friend bool operator< (const Sequence& a, const Sequence& b)
    {
      assert(a._in.size() == b._in.size() && "error on the size");

      uint i;
      for(i = 0; i < a._in.size(); i++)
        if(a[i] == b[i])
          continue;
        else
          return a[i] < b[i];
      assert(false && "ill-formed call, undefined behavior");
      return false;
    }

    friend std::ostream& operator<<(std::ostream& o, const Sequence& a)
    {
      for(uint i = 0; i < a._in.size(); i++)
        o << ( isprint(a[i] )? char(a[i]) : char(47) );
      return o;
    }
  };
}

namespace bwt {

  //
  // Encode
  //
  std::vector<octet>
  encode(const std::vector<octet>& dat)
  {
    std::vector<int> indices ( dat.size() );
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(indices.begin(), indices.end(), // O(nlogn)
              [&] (int lhs, int rhs) {
                Sequence a { dat, lhs };
                Sequence b { dat, rhs };
                return a < b;
              });

    const uint idx = std::distance(indices.begin(),
                                   std::find(indices.begin(), indices.end(), 0)); // O(n)

    std::vector<octet> out ( dat.size() + sizeof idx );

    // out index
    octet bytes[sizeof idx];
    std::copy(reinterpret_cast<const octet*>(&idx),
              reinterpret_cast<const octet*>(&idx) + sizeof idx,
              bytes);

    for(std::size_t i = 0; i < sizeof idx; i++)
      out[i] = bytes[i];

    // out sequence
    const uint last = dat.size() - 1;
    const uint dcl  = sizeof(idx);
    for(uint i = 0; i < dat.size(); i++) {
      Sequence curr { dat, indices[i] };
      out[i+dcl] = curr[last];
    }

    return out;
  }

  //
  // Decode
  //
  std::vector<octet>
  decode(std::vector<octet>& dat)
  {
    // read the index
    const uint* ptridx = reinterpret_cast<const uint*>(dat.data());
    const uint idx = *ptridx;
    dat.erase(dat.begin(), dat.begin() + sizeof idx);

    // build the output
    std::vector<int> indices ( dat.size() );
    std::iota(indices.begin(), indices.end(), 0);

    std::stable_sort(indices.begin(), indices.end(),
                     [&] ( int a, int b ) {
                       return dat[a] < dat[b];
                     });

    const auto lp = [&] (int i) { return dat[indices[i]]; };
    int p = idx;

    std::vector<octet> out(dat.size());
    for(uint i = 0; i < dat.size(); i++) {
      out[i] = lp(p);
      p = indices[p];
    }

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

  std::copy(out.begin(), out.end(),
            std::ostream_iterator<octet>(std::cout));

  return EXIT_SUCCESS;
}
