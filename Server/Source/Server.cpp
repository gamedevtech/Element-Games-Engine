#include "../Server.h"

namespace EGServer {
    Server::Server(void) {
        database = new Database();
        network = new Networking();
    }

    Server::~Server(void) {
        delete network;
        delete database;
    }

    void Server::Update(void) {
        //network->Update();
        bool has_packet = false;
        unsigned int client_id = 0;
        EGServer::Packet *packet = network->ReceivePacket(has_packet, client_id);
        while (has_packet && client_id != 0) {
            InternalProcessPacket(client_id, packet);
            packet = network->ReceivePacket(has_packet, client_id);
        }
    }

    void Server::ProcessPacket(unsigned int client_id, Packet *packet) {
        //
    }

    void Server::ProcessConnectionlessPacket(sf::IpAddress ip_address, EGServer::Packet *packet) {
        //
    }

    void Server::InternalProcessPacket(unsigned int client_id, Packet *packet) {
        sf::Packet *p = packet->GetPacket();
        Networking::ClientState client_state = network->GetClientState(client_id);
        std::cout << "Packet Recieved From " << client_id << "@" << network->GetClientSocket(client_id)->getRemoteAddress() << std::endl;
        if (client_state == Networking::CLIENT_CONNECTED) {
            std::cout << "Client: " << client_id << "@" << network->GetClientSocket(client_id)->getRemoteAddress() << " is authenticating!" << std::endl;
            std::string username, password;
            (*p) >> username >> password;
            // TODO: Check with database
            Packet *r = new Packet();
            (*(r->GetPacket())) << NETWORK_ACTION_AUTH_RESPONSE << 1 << client_id;
            network->SendPacket(client_id, r);
            network->SetClientState(client_id, Networking::CLIENT_AUTHENTICATED);
            std::cout << "Client Authenticated!" << std::endl;
        } else if (client_state == Networking::CLIENT_AUTHENTICATED) {
            std::cout << "Processing Packet Normally From " << client_id << "@" << network->GetClientSocket(client_id)->getRemoteAddress() << std::endl;
            ProcessPacket(client_id, packet);
        }
    }

    void Server::InternalProcessConnectionlessPacket(sf::IpAddress ip_address, EGServer::Packet *packet) {
        sf::Packet *p = packet->GetPacket();
        unsigned int action_type_id;
        (*p) >> action_type_id;
        if (action_type_id == NETWORK_ACTION_LAN_DISCOVERY) {
            Packet *out = new Packet();
            *(out->GetPacket()) << NETWORK_ACTION_LAN_DISCOVERY_RESPONSE;
            network->SendPacket(ip_address, out);
        } else {
            ProcessConnectionlessPacket(ip_address, packet);
        }
    }
};
