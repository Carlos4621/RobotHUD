#include "UDPTransmisor.h"

UDPTransmissor::UDPTransmissor(const std::shared_ptr<EasyUDP> &device)
: device_m{ device }
{
}

void UDPTransmissor::sendData(std::string_view data) {
    device_m->sendData(data);
}

std::string UDPTransmissor::receiveData() {
    return device_m->receiveData();
}
