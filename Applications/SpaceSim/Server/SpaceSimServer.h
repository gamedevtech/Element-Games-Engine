#ifndef SPACESIMSERVER_H
#define SPACESIMSERVER_H

#include "../../../Server/Server.h"
#include "../../../Engine/Math/Math.h"

class SpaceSimServer : public EGServer::Server {
    public:
        SpaceSimServer(void);
        ~SpaceSimServer(void);

        void ProcessPacket(unsigned int client_id, EGServer::Packet *packet);
        void ProcessPacket(unsigned int action_type_id, sf::IpAddress ip_address, EGServer::Packet *packet);
    private:
        //
};

#endif
