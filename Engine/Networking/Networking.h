#ifndef EG_NETWORK_NETWORKING_H
#define EG_NETWORK_NETWORKING_H

#define EG_ENGINE_PORT 93123

#include <queue>
#include <SFML/Network.hpp>
#include "Packet.h"

namespace EG {
    namespace Network {
        // Client
        class Network {
            public:
                Network(std::string server_ip_string = "");
                ~Network(void);

                void SendPacket(Packet *p, bool connectionless = false);
                void Update(void);
                Packet *ReceivePacket(bool &response);
                void PrintStatus(sf::Socket::Status s);
                unsigned int GetClientId(void);
            private:
                bool connected, has_client_id;
                unsigned int client_id;
                Packet *next_packet;
                sf::IpAddress *server_ip_address;
                sf::TcpSocket *tcp;
                sf::UdpSocket *udp;
                std::queue<Packet *> packets_received;
        };
    };
};

#endif
