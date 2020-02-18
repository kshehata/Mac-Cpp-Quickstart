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