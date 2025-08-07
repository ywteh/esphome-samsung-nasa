#pragma once

#include "../nasa_base.h"
#include "../nasa_controller.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace samsung_nasa {

class NASA_Sensor : public sensor::Sensor, public NASA_Base {
  using NASA_Base::NASA_Base;

 public:
  void on_receive(long value) override;
  void set_parent(NASA_Controller *controller);

 protected:
  NASA_Controller *controller_{nullptr};
};

}  // namespace samsung_nasa
}  // namespace esphome