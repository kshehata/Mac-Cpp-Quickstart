#include "utils.h"

void xor_blocks(const byte_array& b1, const byte_array& b2, byte_array& out) {
    // assume b1 and b2 are the same length. Up to caller to ensure that
  if (out.size() != b1.size()) {
    out.resize(b1.size());
  }
  for (int i = 0; i < b1.size(); ++i) {
    out[i] = b1[i] ^ b2[i];
  }
}
