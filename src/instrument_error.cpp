#include "alink/instrument_error.hpp"

namespace alink {

std::optional<InstrumentError> instrument_error_from_code(int code) {
  if (code < 0 || code >= kInstrumentErrorCount) {
    return std::nullopt;
  }
  return static_cast<InstrumentError>(code);
}

int to_code(InstrumentError error) { return static_cast<int>(error); }

std::string_view to_text(InstrumentError error) {
  switch (error) {
    case InstrumentError::BarcodeUnreadable:
      return "Barcode unreadable";
    case InstrumentError::ClotDetected:
      return "Clot detected";
    case InstrumentError::CuvetteMissing:
      return "Cuvette missing";
    case InstrumentError::DoorOpen:
      return "Door open";
    case InstrumentError::IncubatorTemperature:
      return "Incubator temperature out of range";
    case InstrumentError::LampFailure:
      return "Photometer lamp failure";
    case InstrumentError::LidOpen:
      return "Reagent lid open";
    case InstrumentError::PressureLow:
      return "Vacuum pressure low";
    case InstrumentError::ProbeCrash:
      return "Sample probe crash";
    case InstrumentError::ReagentEmpty:
      return "Reagent empty";
    case InstrumentError::SampleShort:
      return "Sample volume too low";
    case InstrumentError::TemperatureDrift:
      return "Reaction temperature drift";
    case InstrumentError::WashFailure:
      return "Wash station failure";
    case InstrumentError::WasteFull:
      return "Waste container full";
  }
  return "Unknown error";
}

}  // namespace alink
