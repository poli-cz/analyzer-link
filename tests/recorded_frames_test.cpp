// Replays frames recorded on real analyzers and compares the decoded output line by line.

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "alink/frame.hpp"

namespace alink {
namespace {

namespace fs = std::filesystem;

std::vector<std::string> read_lines(const fs::path& path, bool skip_comments) {
  std::ifstream in(path);
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(in, line)) {
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    if (line.empty() || (skip_comments && line.front() == '#')) {
      continue;
    }
    lines.push_back(line);
  }
  return lines;
}

TEST(RecordedFramesTest, EveryCaptureDecodesAsRecorded) {
  int captures = 0;
  for (const auto& entry : fs::directory_iterator(ALINK_RECORDED_DIR)) {
    if (entry.path().extension() != ".frames") {
      continue;
    }
    ++captures;
    fs::path expected_path = entry.path();
    expected_path.replace_extension(".expected");
    const auto frames = read_lines(entry.path(), true);
    const auto expected = read_lines(expected_path, false);
    ASSERT_EQ(frames.size(), expected.size()) << entry.path();
    for (std::size_t i = 0; i < frames.size(); ++i) {
      EXPECT_EQ(describe(decode(frames[i])), expected[i])
          << entry.path().filename() << " line " << i + 1 << ": " << frames[i];
    }
  }
  EXPECT_GT(captures, 0);
}

}  // namespace
}  // namespace alink
