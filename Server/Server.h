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

            void Update(void);
            virtual void ProcessPacket(unsigned int client_id, EGServer::Packet *packet);
        protected:
            Database *database;
            Networking *network;
        private:
            void InternalProcessPacket(unsigned int client_id, EGServer::Packet *packet);
    };
};

#endif
