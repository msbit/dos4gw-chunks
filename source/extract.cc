#include <cstdint>

#include <algorithm>
#include <fstream>
#include <memory>

constexpr uint32_t chunksize = 1024;

bool extract_mz(std::ifstream &in, std::ostream *out) {
  auto start = in.tellg();

  uint16_t magic;
  in.read(reinterpret_cast<char *>(&magic), 2);
  if (magic != 0x5a4d) {
    return false;
  }

  uint16_t cblp;
  uint16_t cp;
  in.read(reinterpret_cast<char *>(&cblp), 2);
  in.read(reinterpret_cast<char *>(&cp), 2);

  uint32_t size = cblp == 0 ? cp * 512 : ((cp - 1) * 512) + cblp;

  in.clear();
  in.seekg(start);

  std::unique_ptr<char[]> chunk(new char[chunksize]);
  while (size > 0) {
    in.read(chunk.get(), std::min(chunksize, size));
    out->write(chunk.get(), in.gcount());
    size -= in.gcount();
  }

  return true;
}

bool extract_bw(std::ifstream &in, std::ostream *out) {
  auto start = in.tellg();

  uint16_t magic;
  in.read(reinterpret_cast<char *>(&magic), 2);
  if (magic != 0x5742) {
    return false;
  }

  in.seekg(30, std::ios::cur);
  uint32_t size;
  in.read(reinterpret_cast<char *>(&size), 4);

  in.clear();
  in.seekg(start);

  std::unique_ptr<char[]> chunk(new char[chunksize]);
  while (size > 0) {
    in.read(chunk.get(), std::min(chunksize, size));
    out->write(chunk.get(), in.gcount());
    size -= in.gcount();
  }

  return true;
}

bool extract_mz_le(std::ifstream &in, std::ostream *out) {
  auto start = in.tellg();

  uint16_t magic;
  in.read(reinterpret_cast<char *>(&magic), 2);
  if (magic != 0x5a4d) {
    return false;
  }

  in.seekg(0x3c - (in.tellg() - start), std::ios_base::cur);
  uint32_t lfanew;
  in.read(reinterpret_cast<char *>(&lfanew), 4);

  in.seekg(lfanew - (in.tellg() - start), std::ios_base::cur);
  in.read(reinterpret_cast<char *>(&magic), 2);
  if (magic != 0x454c) {
    return false;
  }

  in.clear();
  in.seekg(start);

  std::unique_ptr<char[]> chunk(new char[chunksize]);
  do {
    in.read(chunk.get(), chunksize);
    out->write(chunk.get(), in.gcount());
  } while (in.gcount() > 0);

  return true;
}
