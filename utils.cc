//
//  Copyright © 2020 Kareem Shehata. All rights reserved.
//
//  Implementation of crypto-related utility functions using Crypto++.

#include "utils.h"

#include <cryptopp/hex.h>

byte_array hex2bytes(const string& in) {
  byte_array result;

  CryptoPP::StringSource ss(in, true,
    new CryptoPP::HexDecoder(
      new CryptoPP::StringSinkTemplate<byte_array>(result)));

  return result;
}

string bytes2hex(const byte_array& in) {
  string result;

  CryptoPP::StringSource ss(in.data(), in.size(), true,
    new CryptoPP::HexEncoder(
      new CryptoPP::StringSink(result), false));

  return result;
}

void xor_blocks(const byte_array& b1, const byte_array& b2, byte_array& out) {
    // assume b1 and b2 are the same length. Up to caller to ensure that
  if (out.size() != b1.size()) {
    out.resize(b1.size());
  }
  for (int i = 0; i < b1.size(); ++i) {
    out[i] = b1[i] ^ b2[i];
  }
}
