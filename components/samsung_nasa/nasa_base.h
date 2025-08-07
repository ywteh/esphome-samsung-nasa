#pragma once

#include <string>
#include "nasa.h"

namespace esphome {
namespace samsung_nasa {

enum class ControllerMode : uint8_t;
class NASA_Device;

class NASA_Base {
 public:
  NASA_Base(const std::string label, const uint16_t message, const ControllerMode nasa_mode, NASA_Device *device)
      : label_{label}, message_{message}, nasa_mode_{nasa_mode}, device_{device} {};
  auto get_message() { return this->message_; }
  auto get_label() { return this->label_; }
  auto get_mode() { return this->nasa_mode_; }
  auto get_device() { return this->device_; }
  // Must implement in each component type
  virtual void on_receive(long value) = 0;
  // Not all components write (e.g., sensor)
  virtual void write(long value) {};

 protected:
  const std::string label_;
  const uint16_t message_;
  const ControllerMode nasa_mode_;
  NASA_Device *device_{nullptr};
};

}  // namespace samsung_nasa
}  // namespace esphome