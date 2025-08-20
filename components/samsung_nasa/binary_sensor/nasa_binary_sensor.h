#pragma once

#include "../nasa_base.h"
#include "../nasa_controller.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace samsung_nasa {

class NASA_BinarySensor : public binary_sensor::BinarySensor, public NASA_Base {
 public:
  inline NASA_BinarySensor(const std::string label, const uint16_t message, const ControllerMode nasa_mode,
                     const NASA_Device *device)
      : NASA_Base(label, message, nasa_mode, device) {};
  void on_receive(long value) override;
  void set_parent(NASA_Controller *controller);

 protected:
  NASA_Controller *controller_{nullptr};
};

}  // namespace samsung_nasa
}  // namespace esphome