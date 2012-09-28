#ifndef EGSERVER_NETWORKING_H
#define EGSERVER_NETWORKING_H

#define EG_ENGINE_PORT 93123

#define NETWORK_SERVER_CLIENT_ID 0

#define NETWORK_ACTION_AUTH 1
#define NETWORK_ACTION_AUTH_RESPONSE 2
#define NETWORK_ACTION_MESSAGE_BROADCAST 3
#define NETWORK_ACTION_MESSAGE_RELAY 4

#include <iostream>
#include <queue>
#include <map>
#include <SFML/Network.hpp>
#include "Packet.h"

namespace EGServer {
    class Networking {
        public:
            enum ClientState {
                CLIENT_DISCONNECTED = 0,
                CLIENT_CONNECTED = 1,
                CLIENT_AUTHENTICATED = 2
            };

            Networking(void);

            void Update(void);
            void SendPacket(unsigned int client_id, Packet *p, bool connectionless = false);
            Packet *ReceivePacket(bool &response, unsigned int &client_id);

            unsigned int GetNextClientId(void);
            ClientState GetClientState(unsigned int client_id);
            void SetClientState(unsigned int client_id, ClientState state);
            sf::TcpSocket *GetClientSocket(unsigned int client_id);

            std::map<unsigned int, sf::TcpSocket *>::iterator ClientsBegin(void);
            std::map<unsigned int, sf::TcpSocket *>::iterator ClientsEnd(void);
        private:
            sf::TcpSocket *current_client;
            sf::TcpListener *listener;
            Packet *next_packet;
            std::map<unsigned int, sf::TcpSocket *> clients;
            std::map<unsigned int, ClientState> client_states;
            std::queue<std::pair<Packet *, unsigned int> > packets_received;
    };
};

#endif
