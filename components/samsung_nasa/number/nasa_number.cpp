#include "nasa_number.h"

namespace esphome {
namespace samsung_nasa {

void NASA_Number::control(float value) {
  this->publish_state(value);
  this->write(this->lambda_to_(value));
};

void NASA_Number::on_receive(long value) {
  auto new_value = this->lambda_from_(value);
  this->publish_state(new_value);
}

void NASA_Number::write(long value) {
  this->controller_->write(this->get_address(), this->get_message(), value);
}

void NASA_Number::set_lambdas(lambda_from lamda_from, lambda_to lambda_to) {
  this->lambda_from_ = lamda_from;
  this->lambda_to_ = lambda_to;
}

void NASA_Number::set_parent(NASA_Controller *controller) { this->controller_ = controller; };

}  // namespace samsung_nasa
}  // namespace esphome