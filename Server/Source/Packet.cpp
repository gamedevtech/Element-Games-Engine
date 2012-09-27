#include <iostream>
#include "../Packet.h"

namespace EGServer{
    Packet::Packet(void) {
        packet = new sf::Packet();
    }

    Packet::~Packet(void) {
        delete packet;
    }

    sf::Packet *Packet::GetPacket(void) {
        return packet;
    }
};
