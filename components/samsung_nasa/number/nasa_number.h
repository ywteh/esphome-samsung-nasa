#pragma once

#include "../nasa_base.h"
#include "../nasa_controller.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace samsung_nasa {

class NASA_Number : public number::Number, public NASA_Base {
  using NASA_Base::NASA_Base;
  using lambda_from = std::function<float(float)>;
  using lambda_to = std::function<uint16_t(float)>;

 public:
  void on_receive(long value) override;
  void write(long value) override;
  void set_lambdas(lambda_from lamda_from, lambda_to lambda_to);
  void set_parent(NASA_Controller *controller);

 protected:
  void control(float value) override;

  lambda_from lambda_from_;
  lambda_to lambda_to_;
  NASA_Controller *controller_{nullptr};
};

}  // namespace samsung_nasa
}  // namespace esphome