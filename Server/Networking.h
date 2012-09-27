#ifndef EGSERVER_NETWORKING_H
#define EGSERVER_NETWORKING_H

#define EG_ENGINE_PORT 93123

#include <iostream>
#include <queue>
#include <map>
#include <SFML/Network.hpp>
#include "Packet.h"

namespace EGServer {
    class Networking {
        public:
            Networking(void);
            ~Networking(void);

            unsigned int GetNextClientId(void);
            void Update(void);
            Packet *ReceivePacket(bool &response);
            sf::TcpSocket *GetClientSocket(unsigned int client_id);
            std::map<unsigned int, sf::TcpSocket *>::iterator ClientsBegin(void);
            std::map<unsigned int, sf::TcpSocket *>::iterator ClientsEnd(void);
        private:
            sf::TcpSocket *current_client;
            sf::TcpListener *listener;
            Packet *next_packet;
            std::map<unsigned int, sf::TcpSocket *> clients;
            std::queue<Packet *> packets_received;
    };
};

#endif
