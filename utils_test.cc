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

class HexConversion : public TestWithParam<pair<string, byte_array>> {
protected:
  ~HexConversion() override {}
  void SetUp() override {}
  void TearDown() override {}
};

TEST_P(HexConversion, Hex2Bytes) {
  const auto& in = GetParam().first;
  const auto& exp = GetParam().second;
  auto result = hex2bytes(in);
  EXPECT_EQ(result, exp);
}

TEST_P(HexConversion, Bytes2Hex) {
  const byte_array& in = GetParam().second;
  const auto& exp = GetParam().first;
  auto result = bytes2hex(in);
  EXPECT_EQ(result, exp);
}

INSTANTIATE_TEST_SUITE_P(HexConversionExamples,
                         HexConversion,
                         testing::Values(
                          make_pair("", byte_array{}),
                          make_pair("00", byte_array{0x00}),
                          make_pair("deadbeef", byte_array{0xde, 0xad, 0xbe, 0xef}),
                          make_pair(
                            "aa55aa55deadbeef0011223344556677aa55aa55deadbeef0011223344556677",
                            byte_array{0xaa, 0x55, 0xaa, 0x55, 0xde, 0xad,
                              0xbe, 0xef, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                              0x66, 0x77, 0xaa, 0x55, 0xaa, 0x55, 0xde, 0xad,
                              0xbe, 0xef, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                              0x66, 0x77})
                          ));



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
                          make_tuple(byte_array{}, byte_array{}, byte_array{}),
                          make_tuple(byte_array{0x00}, byte_array{0x00}, byte_array{0x00}),
                          make_tuple(byte_array{0xde, 0xad, 0xbe, 0xef},
                            byte_array{0xaa, 0x55, 0xaa, 0x55},
                            byte_array{0x74, 0xf8, 0x14, 0xba})
                          ));

} // namespace testing