#ifndef EG_NETWORK_PACKET_H
#define EG_NETWORK_PACKET_H

#include <SFML/Network.hpp>

namespace EGServer {
    class Packet {
        public:
            Packet(void);
            ~Packet(void);

            sf::Packet *GetPacket(void);
        private:
            sf::Packet *packet;
    };
};

#endif
