//
//  Copyright © 2020 Kareem Shehata. All rights reserved.
//
//  Utility functions for blocks of numbers, useful in crypto functions.
//  Mainly serves as an example of how to write unit-tested code and how to use
//  a library like Crypto++.

#ifndef EXAMPLE_UTILS_H
#define EXAMPLE_UTILS_H

#include <string>
#include <vector>

using std::string;

// Convenience type for representing an array of bytes
typedef std::vector<unsigned char> byte_array;

// Convert an ASCII-formatted string with a number in hex to a byte array.
// Uses Crypto++ under the hood for decoding.
byte_array hex2bytes(const string& in);

// Format an array of byte values into a human-readable string in hex.
// Uses Crypto++ under the hood for encoding.
string bytes2hex(const byte_array& in);

// Calculate the XOR of two blocks b1 and b2 and store in out.
// It is assumed that b1 and b2 are both the same length.
// If out is not the same length as b1, it will be resized.
// Output can be in the same array as b1 or b2 if desired or separate.
void xor_blocks(const byte_array& b1, const byte_array& b2, byte_array& out);

#endif  // EXAMPLE_UTILS_H
