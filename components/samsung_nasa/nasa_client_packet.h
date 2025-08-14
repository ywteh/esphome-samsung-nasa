#pragma once

#include "vector"
#include "nasa.h"
#include "nasa_client_common.h"
#include "nasa_client_command.h"
#include "nasa_client_message.h"
#include "nasa_address.h"
#include "esphome/core/optional.h"

namespace esphome {
namespace samsung_nasa {

struct Packet {
  Address sa;
  Address da;
  Command command;
  std::vector<MessageSet> messages;

  static Packet create_partial(Address da, DataType dataType);
  DecodeResult decode(std::vector<uint8_t> &data);
  std::vector<uint8_t> encode();
  std::vector<std::string> to_string(optional<std::string> prefix);
  void log_multiline(optional<std::string> prefix, log_lines_t func);
 private:
  static int get_packet_number();
  static int packet_counter_;
};

struct PacketInfo {
  Packet packet;
  int retry_count;
  uint32_t last_sent_time;
};

}  // namespace samsung_nasa
}  // namespace esphome