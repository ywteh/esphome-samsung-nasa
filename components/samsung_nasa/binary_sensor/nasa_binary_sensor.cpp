#include "nasa_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace samsung_nasa {

void NASA_BinarySensor::on_receive(long value) {
  this->publish_state(value != 0);
}

void NASA_BinarySensor::set_parent(NASA_Controller *controller) { this->controller_ = controller; }

}  // namespace samsung_nasa
}  // namespace esphome