#pragma once

#include "../nasa_base.h"
#include "../nasa_controller.h"
#include "esphome/components/select/select.h"

namespace esphome {
namespace samsung_nasa {

class NASA_Select : public select::Select, public NASA_Base {
  using NASA_Base::NASA_Base;
  using lambda_from = std::function<size_t(size_t)>;
  using lambda_to = std::function<size_t(size_t)>;

 public:
  void on_receive(long value) override;
  void write(long value) override;
  void set_lambdas(lambda_from lamda_from, lambda_to lambda_to);
  void set_parent(NASA_Controller *controller);

 protected:
  void control(const std::string &value) override;

  lambda_from lambda_from_;
  lambda_to lambda_to_;
  NASA_Controller *controller_{nullptr};
};

}  // namespace samsung_nasa
}  // namespace esphome