#pragma once

#include "nasa_climate.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/components/climate/climate_mode.h"

namespace esphome {
namespace samsung_nasa {


template<typename... Ts> class ClimateSetAction : public Action<Ts...> {
 public:
  ClimateSetAction(NASA_Climate *climate) : climate_(climate) {}
  TEMPLATABLE_VALUE(climate::ClimateAction, value)

  void play(Ts... x) override {
    if (this->climate_->update_action(this->value_.value(x...)))
        this->climate_->publish_state();    
  }

 protected:
  NASA_Climate *climate_;
};

}  // namespace samsung_nasa
}  // namespace esphome