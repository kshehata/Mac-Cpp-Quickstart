//
//  Copyright © 2020 Kareem Shehata. All rights reserved.
//
//  Demonstration of using utility function to calculate xor of two numbers
//  given by the user via command line.

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

  // parse both input strings
  auto b1 = hex2bytes(argv[1]);
  auto b2 = hex2bytes(argv[2]);

  // check that input lengths are usable
  if (b1.size() != b2.size()) {
    usage(argv[0]);
    return 2;
  }

  // calculate the xor
  xor_blocks(b1, b2, b1);

  // output the string to the user on stdout
  cout << bytes2hex(b1) << endl;
  return 0;
}
