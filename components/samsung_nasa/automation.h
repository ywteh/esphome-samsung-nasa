#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/log.h"
#include "nasa_base.h"
#include "nasa_controller.h"
#include <vector>
#include <algorithm>
#include <initializer_list>

namespace esphome {
namespace samsung_nasa {

template<typename... Ts> class NASA_Request_Read_Action : public Action<Ts...> {
 public:
  explicit NASA_Request_Read_Action(NASA_Controller *controller) : controller_(controller) {}

  void request_read(std::vector<NASA_Base *> components) { this->components_ = components; }

  void play(Ts... x) override {
    if (debug_log_messages) {
      ESP_LOGI(TAG, "Request Read Action for messages:");
    }
    std::vector<uint16_t> messages;
    std::for_each(begin(components_), end(components_), [&messages](NASA_Base *c) {
      if (debug_log_messages) {
        ESP_LOGI(TAG, "  -> 0x%X [%s]", c->get_message(), c->get_label().c_str());
      };
      messages.push_back(c->get_message());
    });
    this->controller_->read(messages);
  }

 protected:
  std::vector<NASA_Base *> components_;
  NASA_Controller *controller_;
};

}  // namespace samsung_nasa
}  // namespace esphome