#ifndef EG_NETWORK_PACKET_H
#define EG_NETWORK_PACKET_H

#include <SFML/Network.hpp>
#include "../Math/Math.h"

namespace EG {
    namespace Network {
        class Packet {
            public:
                Packet(void);
                ~Packet(void);

                sf::Packet *GetPacket(void);
            private:
                sf::Packet *packet;
        };
    };
};

#endif
