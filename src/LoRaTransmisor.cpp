#include "LoRaTransmisor.h"

LoRaTransmisor::LoRaTransmisor(const std::shared_ptr<EasyLoRa> &device) 
: device_m{ device }
{
}

void LoRaTransmisor::sendData(std::string_view data) {
    device_m->sendMessage(data);
}

std::string LoRaTransmisor::receiveData() {
    return device_m->receiveMessage();
}
