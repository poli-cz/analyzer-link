#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

namespace alink {

// Errors the analyzer reports in ERR frames.
enum class InstrumentError : std::uint8_t {
  BarcodeUnreadable,
  ClotDetected,
  CuvetteMissing,
  DoorOpen,
  IncubatorTemperature,
  LampFailure,
  LidOpen,
  PressureLow,
  ProbeCrash,
  ReagentEmpty,
  SampleShort,
  TemperatureDrift,
  WashFailure,
  WasteFull,
};

inline constexpr int kInstrumentErrorCount = 14;

// Maps the numeric code from an ERR frame; std::nullopt for an unknown code.
std::optional<InstrumentError> instrument_error_from_code(int code);

int to_code(InstrumentError error);

// Short English text shown to the operator.
std::string_view to_text(InstrumentError error);

}  // namespace alink
