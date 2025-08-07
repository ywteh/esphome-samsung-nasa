#include "nasa_select.h"

namespace esphome {
namespace samsung_nasa {

void NASA_Select::control(const std::string &value) {
  if (!this->has_option(value))
    return;
  this->publish_state(value);
  auto index = this->index_of(value).value();
  this->write(this->lambda_to_(index));
};

void NASA_Select::on_receive(long value) {
  auto index = this->lambda_from_(value);
  if (!this->has_index(index))
    return;
  this->publish_state(this->at(index).value());
}

void NASA_Select::write(long value) {
  this->controller_->write(this->get_device()->get_address(), this->get_message(), value);
}

void NASA_Select::set_lambdas(lambda_from lamda_from, lambda_to lambda_to) {
  this->lambda_from_ = lamda_from;
  this->lambda_to_ = lambda_to;
}

void NASA_Select::set_parent(NASA_Controller *controller) { this->controller_ = controller; };

}  // namespace samsung_nasa
}  // namespace esphome