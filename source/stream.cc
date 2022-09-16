#include <cstdint>

#include <fstream>

uint16_t magic(std::ifstream &stream) {
  auto position = stream.tellg();
  uint16_t magic;
  stream.read(reinterpret_cast<char *>(&magic), 2);
  stream.clear();
  stream.seekg(position);
  return magic;
}
