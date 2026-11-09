#include "alink/frame.hpp"

#include <charconv>
#include <cstddef>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

#include "alink/lis_export.hpp"
#include "alink/protocol.hpp"

namespace alink {
namespace {

std::vector<std::string_view> split(std::string_view line, char separator) {
  std::vector<std::string_view> fields;
  std::size_t start = 0;
  while (true) {
    const std::size_t end = line.find(separator, start);
    if (end == std::string_view::npos) {
      fields.push_back(line.substr(start));
      return fields;
    }
    fields.push_back(line.substr(start, end - start));
    start = end + 1;
  }
}

bool parse_int(std::string_view text, int& out) {
  const char* first = text.data();
  const char* last = text.data() + text.size();
  const auto [ptr, ec] = std::from_chars(first, last, out);
  return ec == std::errc{} && ptr == last && !text.empty();
}

bool parse_double(std::string_view text, double& out) {
  std::istringstream in{std::string(text)};
  in.imbue(std::locale::classic());
  in >> out;
  return !text.empty() && !in.fail() && in.eof();
}

Frame invalid(std::string reason) { return InvalidFrame{std::move(reason)}; }

Frame decode_result(const std::vector<std::string_view>& f) {
  if (f.size() != 6) {
    return invalid("RES needs 6 fields");
  }
  ResultFrame frame{std::string(f[1]), std::string(f[2]), std::string(f[3]), 0.0,
                    std::string(f[5])};
  if (!parse_double(f[4], frame.value)) {
    return invalid("RES value is not a number");
  }
  return frame;
}

Frame decode_error(const std::vector<std::string_view>& f) {
  if (f.size() != 4) {
    return invalid("ERR needs 4 fields");
  }
  int code = 0;
  if (!parse_int(f[2], code)) {
    return invalid("ERR code is not a number");
  }
  const auto error = instrument_error_from_code(code);
  if (!error) {
    return invalid("unknown error code " + std::string(f[2]));
  }
  return ErrorFrame{std::string(f[1]), *error, f[3] == "-" ? std::string{} : std::string(f[3])};
}

Frame decode_heartbeat(const std::vector<std::string_view>& f) {
  if (f.size() != 3) {
    return invalid("HB needs 3 fields");
  }
  const auto version = split(f[2], '.');
  HeartbeatFrame frame{std::string(f[1]), 0, 0};
  if (version.size() != 2 || !parse_int(version[0], frame.major) ||
      !parse_int(version[1], frame.minor)) {
    return invalid("HB version must be <major>.<minor>");
  }
  return frame;
}

std::string version_text(int major, int minor) {
  return std::to_string(major) + "." + std::to_string(minor);
}

}  // namespace

Frame decode(std::string_view line) {
  const auto fields = split(line, '|');
  const std::string_view type = fields[0];
  if (type == "RES") {
    return decode_result(fields);
  }
  if (type == "ERR") {
    return decode_error(fields);
  }
  if (type == "HB") {
    return decode_heartbeat(fields);
  }
  return invalid("unknown frame type '" + std::string(type) + "'");
}

std::string describe(const Frame& frame) {
  if (const auto* res = std::get_if<ResultFrame>(&frame)) {
    std::ostringstream out;
    out.imbue(std::locale::classic());
    out << "result " << res->analyzer << ' ' << res->sample_id << ' ' << res->assay << ' '
        << std::setprecision(6) << res->value << ' ' << res->unit;
    return out.str();
  }
  if (const auto* err = std::get_if<ErrorFrame>(&frame)) {
    return "error " + err->analyzer + " code=" + std::to_string(to_code(err->error)) + " \"" +
           std::string(to_text(err->error)) + "\" lis=" + lis_record(*err);
  }
  if (const auto* hb = std::get_if<HeartbeatFrame>(&frame)) {
    const std::string text = "heartbeat " + hb->analyzer + " protocol " + version_text(hb->major, hb->minor);
    if (hb->major != kProtocolMajor) {
      return text + " unsupported major version";
    }
    if (hb->minor > kProtocolMinor) {
      return text + " newer than supported " + version_text(kProtocolMajor, kProtocolMinor);
    }
    return text + " ok";
  }
  return "invalid: " + std::get<InvalidFrame>(frame).reason;
}

}  // namespace alink
