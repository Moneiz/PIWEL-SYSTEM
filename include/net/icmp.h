#ifndef PIWEL_SYSTEM_ICMP_H
#define PIWEL_SYSTEM_ICMP_H

#include <common/types.h>
#include <net/ipv4.h>

namespace net{
    struct InternetControlMessageProtocolMessage{
        common::uint8_t type;
        common::uint8_t code;
        common::uint16_t checksum;
        common::uint32_t data;
    } __attribute__((packed));

    class InternetControlMessageProtocol : InternetProtocolHandler{
    public:
        InternetControlMessageProtocol(InternetProtocolProvider* backend);
        ~InternetControlMessageProtocol();

        bool OnInternetProtocolReceived(common::uint32_t srcIP_BE, common::uint32_t dstIP_BE, common::uint8_t* internetprotocolePayload, common::uint32_t size);
        void RequestEchoReply(common::uint32_t ip_be);
    };
}

#endif //PIWEL_SYSTEM_ICMP_H
