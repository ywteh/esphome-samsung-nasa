#pragma once

#include <functional>

namespace esphome {
namespace samsung_nasa {

using log_lines_t = std::function<void(const char *, const char *)>;

enum class DecodeResultType {
  Fill = 1,
  Discard = 2,
  Processed = 3
};

struct DecodeResult {
  DecodeResultType type;
  uint16_t bytes;
};

enum class DataType : uint8_t {
  Undefined = 0,
  Read = 1,
  Write = 2,
  Request = 3,
  Notification = 4,
  Response = 5,
  Ack = 6,
  Nack = 7
};

enum class PacketType : uint8_t { StandBy = 0, Normal = 1, Gathering = 2, Install = 3, Download = 4 };

}  // namespace samsung_nasa
}  // namespace esphome