#include <cstdint>
#include <cstdio>

#include <algorithm>
#include <fstream>
#include <string>

void extract_chunks(const std::string &);

bool extract_mz(std::ifstream &, std::ostream *);
bool extract_bw(std::ifstream &, std::ostream *);
bool extract_mz_le(std::ifstream &, std::ostream *);

std::string without_extension(const std::string &);

constexpr uint32_t chunksize = 1024;

int main(int argc, char **argv) {
  for (auto i = 1; i < argc; i++) {
    extract_chunks(argv[i]);
  }
}

uint64_t current(std::ifstream &stream) {
  auto position = stream.tellg();
  stream.seekg(0);
  auto start = stream.tellg();
  auto result = position - start;
  stream.seekg(position);
  return result;
}

uint16_t magic(std::ifstream &stream) {
  auto position = stream.tellg();
  uint16_t magic;
  stream.read(reinterpret_cast<char *>(&magic), 2);
  stream.clear();
  stream.seekg(position);
  return magic;
}

void extract_chunks(const std::string &input) {
  auto base = without_extension(input);
  std::ifstream in(input, std::ios::binary);
  auto start = in.tellg();

  if (magic(in) != 0x5a4d) {
    fprintf(stderr, "%s: expecting 'MZ' at %08llx\n", input.c_str(), in.tellg() - start);
    return;
  }

  std::unique_ptr<char[]> trailer(new char[13]);

  snprintf(trailer.get(), 13, ".%08llx.mz", in.tellg() - start);
  std::ofstream mz(base + trailer.get(), std::ios::binary);
  if (!extract_mz(in, &mz)) {
    fprintf(stderr, "%s: not MZ\n", input.c_str());
    return;
  }

  if (magic(in) != 0x5742) {
    fprintf(stderr, "%s: expecting 'BW' at %08llx\n", input.c_str(), in.tellg() - start);
    return;
  }

  for (;;) {
    if (magic(in) != 0x5742) {
      break;
    }

    snprintf(trailer.get(), 13, ".%08llx.bw", in.tellg() - start);
    std::ofstream bw(base + trailer.get(), std::ios::binary);
    if (!extract_bw(in, &bw)) {
      fprintf(stderr, "%s: not BW\n", input.c_str());
      return;
    }
  }

  if (magic(in) != 0x5a4d) {
    fprintf(stderr, "%s: expecting 'MZ' at %08llx\n", input.c_str(), in.tellg() - start);
    return;
  }

  snprintf(trailer.get(), 13, ".%08llx.le", in.tellg() - start);
  std::ofstream mz_le(base + trailer.get(), std::ios::binary);
  if (!extract_mz_le(in, &mz_le)) {
    fprintf(stderr, "%s: not MZ/LE\n", input.c_str());
  }
}

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

  in.clear();
  in.seekg(start);

  std::unique_ptr<char[]> chunk(new char[chunksize]);
  do {
    in.read(chunk.get(), chunksize);
    out->write(chunk.get(), in.gcount());
  } while (in.gcount() > 0);

  return true;
}

std::string without_extension(const std::string &input) {
  auto dotpos = input.find_last_of('.');
  if (dotpos == std::string::npos || dotpos == 0) {
    // no '.' character in filename, or '.' is first character, copy it all
    return input;
  }

  auto slashpos = input.find_last_of('/');
  if (slashpos != std::string::npos && dotpos < slashpos) {
    // filename contains '/' in position after '.', copy it all
    return input;
  }

  if (slashpos != std::string::npos && (dotpos - slashpos) == 1) {
    // filename contains '/.' for last slash and dot positions, copy it all
    return input;
  }

  return input.substr(0, dotpos);
}
