#include "nasa.h"
#include "nasa_client_packet.h"

namespace esphome {
namespace samsung_nasa {

void Command::decode(std::vector<uint8_t> &data, unsigned int index) {
  this->packetInformation = ((int) data[index] & 128) >> 7 == 1;
  this->protocolVersion = (uint8_t) (((int) data[index] & 96) >> 5);
  this->retryCount = (uint8_t) (((int) data[index] & 24) >> 3);
  this->packetType = (PacketType) (((int) data[index + 1] & 240) >> 4);
  this->dataType = (DataType) ((int) data[index + 1] & 15);
  this->packetNumber = data[index + 2];
}

void Command::encode(std::vector<uint8_t> &data) {
  data.push_back(
      (uint8_t) ((((int) packetInformation ? 1 : 0) << 7) + ((int) protocolVersion << 5) + ((int) retryCount << 3)));
  data.push_back((uint8_t) (((int) packetType << 4) + (int) dataType));
  data.push_back(packetNumber);
}

std::string Command::to_string() {
  std::string str;
  str += "{";
  str += "PacketInformation: " + std::to_string(packetInformation) + ";";
  str += "ProtocolVersion: " + std::to_string(protocolVersion) + ";";
  str += "RetryCount: " + std::to_string(retryCount) + ";";
  str += "PacketType: " + std::to_string((int) packetType) + ";";
  str += "DataType: " + std::to_string((int) dataType) + ";";
  str += "PacketNumber: " + std::to_string(packetNumber);
  str += "}";
  return str;
}

}  // namespace samsung_nasa
}  // namespace esphome