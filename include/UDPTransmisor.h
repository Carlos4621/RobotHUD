#ifndef UDP_TRANSMISSOR_HEADER
#define UDP_TRANSMISSOR_HEADER

#include "IOCommons.h"
#include <boost/asio.hpp>
#include "EasyUDP.h"

class UDPTransmissor : public IOCommons {
public:
    UDPTransmissor(const std::shared_ptr<EasyUDP>& device);

    void sendData(std::string_view data) override;

    [[nodiscard]]
    std::string receiveData() override;

private:
    std::shared_ptr<EasyUDP> device_m;
};

#endif //!UDP_TRANSMISSOR_HEADER