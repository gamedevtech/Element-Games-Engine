#ifndef EG_NETWORK_NETWORKING_H
#define EG_NETWORK_NETWORKING_H

#define EG_ENGINE_TCP_PORT 42424
#define EG_ENGINE_UDP_PORT 42425

#define NETWORK_SERVER_CLIENT_ID 0

#define NETWORK_ACTION_AUTH 1
#define NETWORK_ACTION_AUTH_RESPONSE 2
#define NETWORK_ACTION_MESSAGE_BROADCAST 3
#define NETWORK_ACTION_MESSAGE_RELAY 4
#define NETWORK_ACTION_LAN_DISCOVERY 5
#define NETWORK_ACTION_LAN_DISCOVERY_RESPONSE 6

#include <queue>
#include <SFML/Network.hpp>
#include "Packet.h"
#include "../Utility/Time.h"

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
                void Update(float frame_time);
                Packet *ReceivePacket(bool &response);
                void PrintStatus(sf::Socket::Status s);
                unsigned int GetClientId(void);
                bool IsPollingLAN(void);
                std::vector<sf::IpAddress> *GetServerIpAddresses(void);
            private:
                float timer;
                bool connected, authenticated, has_client_id, polling_lan; // Switch to state machine with enum
                unsigned int client_id;
                Packet *next_packet;
                sf::IpAddress *server_ip_address;
                sf::TcpSocket *tcp;
                sf::UdpSocket *udp;
                std::queue<Packet *> packets_received;
                std::vector<sf::IpAddress> server_ip_addresses;
        };
    };
};

#endif
