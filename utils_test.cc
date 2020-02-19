//
//  Copyright © 2020 Kareem Shehata. All rights reserved.
//
//  Unit tests of crypto-related utility functions using Crypto++.

#include "utils.h"

#include <gtest/gtest.h>
#include <utility>

using std::pair;
using std::make_pair;
using std::string;
using std::tuple;
using ::testing::TestWithParam;
using ::testing::Values;

namespace testing {

// Unit tests for conversions between hex-formatted strings and byte arrays
class HexConversion : public TestWithParam<pair<string, byte_array>> {
protected:
  ~HexConversion() override {}

  void SetUp() override {
    hex_string_ = GetParam().first;
    byte_values_ = GetParam().second;
  }

  void TearDown() override {}

  string hex_string_;
  byte_array byte_values_;
};

// Unit tests for converting hex strings to bytes
TEST_P(HexConversion, Hex2Bytes) {
  auto result = hex2bytes(hex_string_);
  EXPECT_EQ(result, byte_values_);
}

// Reverse direction: encoding bytes to hex strings
TEST_P(HexConversion, Bytes2Hex) {
  auto result = bytes2hex(byte_values_);
  EXPECT_EQ(result, hex_string_);
}

INSTANTIATE_TEST_SUITE_P(HexConversionExamples,
                         HexConversion,
                         testing::Values(
                          // empty string should just produce empty array
                          make_pair("", byte_array{}),

                          // simplest case
                          make_pair("00", byte_array{0x00}),

                          // normal case
                          make_pair("deadbeef", byte_array{0xde, 0xad, 0xbe, 0xef}),

                          // long case
                          make_pair(
                            "aa55aa55deadbeef0011223344556677aa55aa55deadbeef0011223344556677",
                            byte_array{0xaa, 0x55, 0xaa, 0x55, 0xde, 0xad,
                              0xbe, 0xef, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                              0x66, 0x77, 0xaa, 0x55, 0xaa, 0x55, 0xde, 0xad,
                              0xbe, 0xef, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                              0x66, 0x77})
                          ));


// Unit tests for utility function xor_blocks
class XorBlocks : public TestWithParam<tuple<byte_array, byte_array, byte_array>> {
protected:
  ~XorBlocks() override {}
  void SetUp() override {}
  void TearDown() override {}
};

TEST_P(XorBlocks, Examples) {
  const auto& b1 = get<0>(GetParam());
  const auto& b2 = get<1>(GetParam());
  const auto& exp = get<2>(GetParam());
  byte_array result;
  result.resize(b1.size());
  xor_blocks(b1, b2, result);
  EXPECT_EQ(result, exp);
}

INSTANTIATE_TEST_SUITE_P(XorBlocksExamples,
                         XorBlocks,
                         testing::Values(
                          // make sure empty arrays don't do anything crazy
                          make_tuple(byte_array{}, byte_array{}, byte_array{}),

                          // zero case
                          make_tuple(byte_array{0x00}, byte_array{0x00},
                            byte_array{0x00}),

                          // basic usage
                          make_tuple(byte_array{0xde, 0xad, 0xbe, 0xef},
                            byte_array{0xaa, 0x55, 0xaa, 0x55},
                            byte_array{0x74, 0xf8, 0x14, 0xba})
                          ));

} // namespace testing