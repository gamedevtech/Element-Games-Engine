#ifndef SPACESIMSERVER_H
#define SPACESIMSERVER_H

#include "../../../Server/Server.h"

class SpaceSimServer : public EGServer::Server {
    public:
        SpaceSimServer(void);
        ~SpaceSimServer(void);

        void ProcessPacket(unsigned int client_id, EGServer::Packet *packet);
    private:
        //
};

#endif
