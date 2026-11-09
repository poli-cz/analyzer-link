// Reads analyzer frames from standard input, one per line, and prints what they mean.
//
//   link-cli < capture.frames

#include <iostream>
#include <string>

#include "alink/frame.hpp"

int main() {
  std::string line;
  while (std::getline(std::cin, line)) {
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    if (line.empty() || line.front() == '#') {
      continue;
    }
    std::cout << alink::describe(alink::decode(line)) << '\n';
  }
  return 0;
}
