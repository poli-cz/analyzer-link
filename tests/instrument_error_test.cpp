#include "alink/instrument_error.hpp"

#include <gtest/gtest.h>

namespace alink {
namespace {

TEST(InstrumentErrorTest, MapsKnownCodes) {
  EXPECT_EQ(instrument_error_from_code(0), InstrumentError::BarcodeUnreadable);
  EXPECT_EQ(instrument_error_from_code(9), InstrumentError::ReagentEmpty);
  EXPECT_EQ(instrument_error_from_code(13), InstrumentError::WasteFull);
}

TEST(InstrumentErrorTest, RejectsUnknownCodes) {
  EXPECT_EQ(instrument_error_from_code(-1), std::nullopt);
  EXPECT_EQ(instrument_error_from_code(14), std::nullopt);
  EXPECT_EQ(instrument_error_from_code(255), std::nullopt);
}

TEST(InstrumentErrorTest, CodeRoundTrips) {
  for (int code = 0; code < kInstrumentErrorCount; ++code) {
    const auto error = instrument_error_from_code(code);
    ASSERT_TRUE(error.has_value()) << code;
    EXPECT_EQ(to_code(*error), code);
  }
}

TEST(InstrumentErrorTest, EveryErrorHasText) {
  for (int code = 0; code < kInstrumentErrorCount; ++code) {
    const auto text = to_text(*instrument_error_from_code(code));
    EXPECT_FALSE(text.empty()) << code;
    EXPECT_NE(text, "Unknown error") << code;
  }
}

}  // namespace
}  // namespace alink
