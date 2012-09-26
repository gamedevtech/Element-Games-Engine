#ifndef EGSERVER_NETWORKING_H
#define EGSERVER_NETWORKING_H

#define EG_ENGINE_PORT 93123

#include <iostream>
#include <queue>
#include <map>
#include <SFML/Network.hpp>
//#include "Packet.h"

namespace EGServer {
    class Networking {
        public:
            Networking(void);
            ~Networking(void);

            unsigned int GetNextClientId(void);
            void Update(void);
        private:
            sf::TcpSocket *current_client;
            sf::TcpListener *listener;
            std::map<unsigned int, sf::TcpSocket *> clients;
    };
};

#endif
