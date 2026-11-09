#include "alink/frame.hpp"

#include <gtest/gtest.h>

#include <string>
#include <variant>

#include "alink/protocol.hpp"

namespace alink {
namespace {

TEST(FrameTest, DecodesResult) {
  const Frame frame = decode("RES|AN-07|S-1001|GLU|5.4|mmol/L");
  const auto* res = std::get_if<ResultFrame>(&frame);
  ASSERT_NE(res, nullptr);
  EXPECT_EQ(res->sample_id, "S-1001");
  EXPECT_EQ(res->assay, "GLU");
  EXPECT_DOUBLE_EQ(res->value, 5.4);
  EXPECT_EQ(describe(frame), "result AN-07 S-1001 GLU 5.4 mmol/L");
}

TEST(FrameTest, DecodesErrorWithoutSample) {
  const Frame frame = decode("ERR|AN-07|3|-");
  const auto* err = std::get_if<ErrorFrame>(&frame);
  ASSERT_NE(err, nullptr);
  EXPECT_EQ(err->error, InstrumentError::DoorOpen);
  EXPECT_TRUE(err->sample_id.empty());
}

TEST(FrameTest, RejectsBrokenFrames) {
  EXPECT_EQ(describe(decode("RES|AN-07|S-1|GLU|abc|mmol/L")), "invalid: RES value is not a number");
  EXPECT_EQ(describe(decode("ERR|AN-07|x|-")), "invalid: ERR code is not a number");
  EXPECT_EQ(describe(decode("ERR|AN-07|99|-")), "invalid: unknown error code 99");
  EXPECT_EQ(describe(decode("HB|AN-07|2")), "invalid: HB version must be <major>.<minor>");
  EXPECT_EQ(describe(decode("PING|AN-07")), "invalid: unknown frame type 'PING'");
}

TEST(FrameTest, HeartbeatWithCurrentVersionIsOk) {
  const std::string version = std::to_string(kProtocolMajor) + "." + std::to_string(kProtocolMinor);
  EXPECT_EQ(describe(decode("HB|AN-07|" + version)),
            "heartbeat AN-07 protocol " + version + " ok");
}

TEST(FrameTest, HeartbeatWithNewerMinorIsReported) {
  const std::string current =
      std::to_string(kProtocolMajor) + "." + std::to_string(kProtocolMinor);
  const std::string newer =
      std::to_string(kProtocolMajor) + "." + std::to_string(kProtocolMinor + 1);
  EXPECT_EQ(describe(decode("HB|AN-07|" + newer)),
            "heartbeat AN-07 protocol " + newer + " newer than supported " + current);
}

TEST(FrameTest, HeartbeatWithOtherMajorIsUnsupported) {
  const std::string version = std::to_string(kProtocolMajor + 1) + ".0";
  EXPECT_EQ(describe(decode("HB|AN-07|" + version)),
            "heartbeat AN-07 protocol " + version + " unsupported major version");
}

}  // namespace
}  // namespace alink
