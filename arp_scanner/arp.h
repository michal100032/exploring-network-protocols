#pragma once

#include <vector>

#include <arpa/inet.h>
#include <linux/if_packet.h>

#include "interface.h"

#pragma pack(push, 1)
struct arp_message {
    arp_message() {}
    arp_message(mac_addr senderMac, in_addr senderIp, mac_addr targetMac, in_addr targetIp) : 
        senderHardwareAddr(senderMac),
        senderProtocolAddr(senderIp),
        targetHardwareAddr(targetMac),
        targetProtocolAddr(targetIp) {}

    uint16_t hardwareType = 0x0100; // 0x0001 - ethernet
    uint16_t protocolType = 0x0008; // 0x0800 - ipv4

    uint8_t hardwareAddrLen = 6; // eth
    uint8_t protocolAddrLen = 4; // ip

    uint16_t operation = 0x0100; // 0x0001 - request

    mac_addr senderHardwareAddr;
    in_addr senderProtocolAddr;

    mac_addr targetHardwareAddr;
    in_addr targetProtocolAddr;
};
#pragma pack(pop)

struct arp_record {
    in_addr ip;
    mac_addr mac;

    arp_record(const in_addr& ip, const mac_addr& mac) : ip(ip), mac(mac) {}
};

class arp {
public:

    static arp& get() {
        static arp instance;
        return instance;
    }

    static std::vector<arp_record> scan(interface& intf) {
        return get().scanInst(intf);
    }

private:
    arp();

    sockaddr_ll makeBroadcastSock(interface& intf);

    std::vector<arp_record> scanInst(interface& intf);
    void receiveArp(int sock, std::vector<arp_record>& arps);
};
