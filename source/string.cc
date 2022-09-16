#include <string>

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
