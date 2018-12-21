#pragma once

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <climits>
#include <cassert>
#include <vector>

template<typename T>
struct Parameter {
  std::vector<T> dat;
  bool decode;
};

// T doit etre une representation d'un octet
template<typename T>
Parameter<T> parse( int argc, char *argv[] )
{
  const std::size_t BUFFER_SIZE = 1024; // pour remplir le vector

  if(argc != 2) {
    std::cerr << "erreur d'options : doit etre -d ou -e" << std::endl;
    exit(EXIT_FAILURE);
  }

  const bool decode = strcmp( argv[1], "-d" ) == 0;
  const bool encode = strcmp( argv[1], "-e" ) == 0;

  if( not (decode xor encode) ) {
    std::cerr << "erreur d'options : doit etre -d ou -e" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<T> stream;

  try
  {
    std::freopen(nullptr, "rb", stdin);

    if(std::ferror(stdin))
      throw std::runtime_error(std::strerror(errno));

    std::size_t len;
    std::array<T, BUFFER_SIZE> buf;

    while((len = std::fread(buf.data(), sizeof(buf[0]), buf.size(), stdin)) > 0)
    {
      if(std::ferror(stdin) && !std::feof(stdin))
        throw std::runtime_error(std::strerror(errno));

      stream.insert(stream.end(), buf.data(), buf.data() + len);
    }
  }
  catch(std::exception const& e)
  {
    std::cerr << e.what() << '\n';
    exit(EXIT_FAILURE);
  }

  return { std::move(stream), decode };
}
