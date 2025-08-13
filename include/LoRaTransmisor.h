#ifndef LORA_TRANMISOR_HEADER
#define LORA_TRANMISOR_HEADER

#include "IOCommons.h"
#include <memory>
#include "EasyLoRa.hpp"

class LoRaTransmisor : public IOCommons {
public:
    explicit LoRaTransmisor(const std::shared_ptr<EasyLoRa>& device);

    void sendData(std::string_view data) override;

    [[nodiscard]]
    std::string receiveData() override;

private:
    std::shared_ptr<EasyLoRa> device_m;

};

#endif //!LORA_TRANMISOR_HEADER