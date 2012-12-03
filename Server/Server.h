#ifndef EGSERVER_SERVER_H
#define EGSERVER_SERVER_H

#include "Packet.h"
#include "Database.h"
#include "Networking.h"

namespace EGServer {
    class Server {
        public:
            Server(void);
            ~Server(void);

            bool IsDone(void);
            void Update(void);
            virtual void ProcessPacket(unsigned int client_id, EGServer::Packet *packet);
            virtual void ProcessPacket(sf::IpAddress ip_address, EGServer::Packet *packet);
        protected:
            bool done;
            Database *database;
            Networking *network;
        private:
            void InternalProcessPacket(unsigned int client_id, EGServer::Packet *packet);
            void InternalProcessPacket(unsigned int action_type_id, sf::IpAddress ip_address, EGServer::Packet *packet);
    };
};

#endif
