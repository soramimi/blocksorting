#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <climits>

#include <iostream>
#include <vector>
#include <array>

namespace {
  constexpr std::size_t BUFFER_SIZE = 1024; // pour remplir le vector
  using octet = uint8_t;
  // using octet = std::byte;
}

int main( int argc, char *argv[] )
{
  if(argc != 2) return EXIT_FAILURE;

  // bool decode = strcmp( argv[1], "-d" ) == 0;

  std::vector<octet> input;

  try
  {
    std::freopen(nullptr, "rb", stdin);

    if(std::ferror(stdin))
      throw std::runtime_error(std::strerror(errno));

    std::size_t len;
    std::array<octet, BUFFER_SIZE> buf;

    while((len = std::fread(buf.data(), sizeof(buf[0]), buf.size(), stdin)) > 0)
    {
      if(std::ferror(stdin) && !std::feof(stdin))
        throw std::runtime_error(std::strerror(errno));

      input.insert(input.end(), buf.data(), buf.data() + len);
    }
  }
  catch(std::exception const& e)
  {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  for(auto o: input)
    std::clog << o;

  std::clog << " :: " << input.size() << std::endl;

  return EXIT_SUCCESS;
}
