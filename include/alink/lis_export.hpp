#pragma once

#include <string>

#include "alink/frame.hpp"
#include "alink/instrument_error.hpp"

namespace alink {

// LIS flag: 'S' sample, 'R' reagent, 'H' hardware, 'Q' quality hold, 'X' unknown.
char lis_flag(InstrumentError error);

// One LIS record for an ERR frame: E|<analyzer>|<sample>|<flag>|<text>
std::string lis_record(const ErrorFrame& frame);

}  // namespace alink
