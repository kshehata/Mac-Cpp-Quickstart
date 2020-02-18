#ifndef EXAMPLE_UTILS_H
#define EXAMPLE_UTILS_H

#include <string>
#include <vector>

using std::string;

typedef std::vector<unsigned char> byte_array;

byte_array hex2bytes(const string& in);

string bytes2hex(const byte_array& in);

void xor_blocks(const byte_array& b1, const byte_array& b2, byte_array& out);

#endif  // EXAMPLE_UTILS_H
