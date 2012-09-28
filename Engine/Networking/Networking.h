#ifndef EG_NETWORK_NETWORKING_H
#define EG_NETWORK_NETWORKING_H

#define EG_ENGINE_PORT 93123

#define NETWORK_SERVER_CLIENT_ID 0

#define NETWORK_ACTION_AUTH 1
#define NETWORK_ACTION_AUTH_RESPONSE 2
#define NETWORK_ACTION_MESSAGE_BROADCAST 3
#define NETWORK_ACTION_MESSAGE_RELAY 4

#include <queue>
#include <SFML/Network.hpp>
#include "Packet.h"

namespace EG {
    namespace Network {
        // Client
        class Network {
            public:
                Network(void);
                ~Network(void);

                void Connect(std::string server_address, std::string username = "", std::string password = "");
                std::vector<std::pair<std::string, std::string> > PollLAN(void);
                void SendPacket(Packet *p, bool connectionless = false);
                void Update(void);
                Packet *ReceivePacket(bool &response);
                void PrintStatus(sf::Socket::Status s);
                unsigned int GetClientId(void);
            private:
                bool connected, authenticated, has_client_id;
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
