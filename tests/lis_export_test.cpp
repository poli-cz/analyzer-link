#include "alink/lis_export.hpp"

#include <gtest/gtest.h>

namespace alink {
namespace {

TEST(LisExportTest, FlagsByCategory) {
  EXPECT_EQ(lis_flag(InstrumentError::ClotDetected), 'S');
  EXPECT_EQ(lis_flag(InstrumentError::ReagentEmpty), 'R');
  EXPECT_EQ(lis_flag(InstrumentError::ProbeCrash), 'H');
  EXPECT_EQ(lis_flag(InstrumentError::TemperatureDrift), 'Q');
}

TEST(LisExportTest, RecordWithSample) {
  const ErrorFrame frame{"AN-07", InstrumentError::SampleShort, "S-1006"};
  EXPECT_EQ(lis_record(frame), "E|AN-07|S-1006|S|Sample volume too low");
}

TEST(LisExportTest, RecordWithoutSample) {
  const ErrorFrame frame{"AN-12", InstrumentError::WasteFull, ""};
  EXPECT_EQ(lis_record(frame), "E|AN-12|-|H|Waste container full");
}

}  // namespace
}  // namespace alink
