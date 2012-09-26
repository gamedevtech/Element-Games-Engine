#include "../Packet.h"

namespace EG{
    namespace Network{
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
};
