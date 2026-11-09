#include "alink/lis_export.hpp"

#include <string>

namespace alink {

char lis_flag(InstrumentError error) {
  switch (error) {
    case InstrumentError::BarcodeUnreadable:
    case InstrumentError::ClotDetected:
    case InstrumentError::SampleShort:
      return 'S';
    case InstrumentError::ReagentEmpty:
    case InstrumentError::LidOpen:
      return 'R';
    case InstrumentError::CuvetteMissing:
    case InstrumentError::DoorOpen:
    case InstrumentError::LampFailure:
    case InstrumentError::PressureLow:
    case InstrumentError::ProbeCrash:
    case InstrumentError::WashFailure:
    case InstrumentError::WasteFull:
      return 'H';
    case InstrumentError::IncubatorTemperature:
    case InstrumentError::TemperatureDrift:
      return 'Q';
  }
  return 'X';
}

std::string lis_record(const ErrorFrame& frame) {
  std::string record = "E|";
  record += frame.analyzer;
  record += '|';
  record += frame.sample_id.empty() ? "-" : frame.sample_id;
  record += '|';
  record += lis_flag(frame.error);
  record += '|';
  record += to_text(frame.error);
  return record;
}

}  // namespace alink
