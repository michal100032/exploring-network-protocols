#pragma once

#include <stdint.h>
#include <iostream>

struct mac_addr {
    uint8_t address[6] = { 0, 0, 0, 0, 0, 0};
    bool isZero() {
        for(int i = 0; i < 6; i++) {
            if(address[i] != 0) {
                return false;
            }
        }
        return true;
    }
};

std::ostream& operator<<(std::ostream& stream, const mac_addr& mac);
