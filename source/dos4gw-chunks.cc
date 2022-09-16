#include <cstdio>

#include <fstream>
#include <memory>
#include <string>

#include "extract.h"
#include "stream.h"
#include "string.h"

void extract_chunks(const std::string &);

int main(int argc, char **argv) {
  for (auto i = 1; i < argc; i++) {
    extract_chunks(argv[i]);
  }
}

void extract_chunks(const std::string &input) {
  auto base = without_extension(input);
  std::ifstream in(input, std::ios::binary);
  auto start = in.tellg();

  if (magic(in) != 0x5a4d) {
    fprintf(stderr, "%s: expecting 'MZ' at %08llx\n", input.c_str(),
            in.tellg() - start);
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
    fprintf(stderr, "%s: expecting 'BW' at %08llx\n", input.c_str(),
            in.tellg() - start);
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
    fprintf(stderr, "%s: expecting 'MZ' at %08llx\n", input.c_str(),
            in.tellg() - start);
    return;
  }

  snprintf(trailer.get(), 13, ".%08llx.le", in.tellg() - start);
  std::ofstream mz_le(base + trailer.get(), std::ios::binary);
  if (!extract_mz_le(in, &mz_le)) {
    fprintf(stderr, "%s: not MZ/LE\n", input.c_str());
  }
}
