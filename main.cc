#include "utils.h"
#include <iostream>

using std::cout;
using std::endl;

void usage(const char* name) {
  cout << "Usage: " << name << " [b1] [b2]" << endl;
  cout << "b1 and b2 are numbers in hex (base 16) that must be the same length"
   << endl;
  cout << "Output is the xor of the two numbers" << endl;
}

int main(int argc, const char * argv[]) {
  if (argc != 3) {
    usage(argv[0]);
    return 1;
  }

  auto b1 = hex2bytes(argv[1]);
  auto b2 = hex2bytes(argv[2]);

  if (b1.size() != b2.size()) {
    usage(argv[0]);
    return 2;
  }

  xor_blocks(b1, b2, b1);

  cout << bytes2hex(b1) << endl;
  return 0;
}
