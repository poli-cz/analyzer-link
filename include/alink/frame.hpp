#pragma once

#include <string>
#include <string_view>
#include <variant>

#include "alink/instrument_error.hpp"

namespace alink {

// RES|<analyzer>|<sample>|<assay>|<value>|<unit>
struct ResultFrame {
  std::string analyzer;
  std::string sample_id;
  std::string assay;
  double value = 0.0;
  std::string unit;
};

// ERR|<analyzer>|<code>|<sample or ->
struct ErrorFrame {
  std::string analyzer;
  InstrumentError error = InstrumentError::BarcodeUnreadable;
  std::string sample_id;  // empty when the frame carries "-"
};

// HB|<analyzer>|<major>.<minor>
struct HeartbeatFrame {
  std::string analyzer;
  int major = 0;
  int minor = 0;
};

struct InvalidFrame {
  std::string reason;
};

using Frame = std::variant<ResultFrame, ErrorFrame, HeartbeatFrame, InvalidFrame>;

// Decodes one line received from the analyzer (without the line terminator).
Frame decode(std::string_view line);

// One human-readable line per frame, as printed by link-cli.
std::string describe(const Frame& frame);

}  // namespace alink
