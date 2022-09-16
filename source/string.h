#ifndef __STRING_H__
#define __STRING_H__

#include <cstdint>
#include <cstddef>

#include <string>

constexpr uint16_t operator ""_magic2(const char *value, std::size_t len) {
  if (len == 0) { return 0; }

  if (len == 1) { return value[0]; }

  return value[1] << 8 | value[0];
}

std::string without_extension(const std::string &);

#endif
