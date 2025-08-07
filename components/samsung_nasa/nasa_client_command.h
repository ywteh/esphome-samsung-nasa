#pragma once

#include <vector>
#include <string>
#include "nasa.h"
#include "nasa_client_common.h"

namespace esphome {
namespace samsung_nasa {

struct Command {
  bool packetInformation = true;
  uint8_t protocolVersion = 2;
  uint8_t retryCount = 0;
  PacketType packetType = PacketType::StandBy;
  DataType dataType = DataType::Undefined;
  uint8_t packetNumber = 0;
  uint8_t size = 3;
  void decode(std::vector<uint8_t> &data, unsigned int index);
  void encode(std::vector<uint8_t> &data);
  std::string to_string();
};

}  // namespace samsung_nasa
}  // namespace esphome