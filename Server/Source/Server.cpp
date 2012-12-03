#include "../Server.h"

namespace EGServer {
    Server::Server(void) {
        database = new Database();
        network = new Networking();
        done = false;
    }

    Server::~Server(void) {
        delete network;
        delete database;
        std::cout << "Cleaning Up Server" << std::endl;
    }

    bool Server::IsDone(void) {
        return done;
    }

    void Server::Update(void) {
        bool has_packet = false;
        unsigned int client_id = 0;
        EGServer::Packet *packet = network->ReceivePacket(has_packet, client_id);
        while (has_packet && client_id != 0) {
            InternalProcessPacket(client_id, packet);
            packet = network->ReceivePacket(has_packet, client_id);
        }

        has_packet = false;
        sf::IpAddress remote_ip;
        packet = network->ReceivePacket(has_packet, remote_ip);
        while (has_packet) {
            InternalProcessPacket(remote_ip, packet);
            packet = network->ReceivePacket(has_packet, remote_ip);
        }
    }

    void Server::ProcessPacket(unsigned int client_id, Packet *packet) {
        //
    }

    void Server::ProcessPacket(unsigned int action_type_id, sf::IpAddress ip_address, EGServer::Packet *packet) {
        //
    }

    void Server::InternalProcessPacket(unsigned int client_id, Packet *packet) {
        sf::Packet *p = packet->GetPacket();
        Networking::ClientState client_state = network->GetClientState(client_id);
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
            ProcessPacket(client_id, packet);
        }
    }

    void Server::InternalProcessPacket(sf::IpAddress ip_address, EGServer::Packet *packet) {
        sf::Packet *p = packet->GetPacket();
        unsigned int action_type_id;
        (*p) >> action_type_id;
        std::cout << "UDP Pakcet with Action Type: " << action_type_id << "@" << ip_address.toString() << std::endl;
        if (action_type_id == NETWORK_ACTION_LAN_DISCOVERY) {
            Packet *out = new Packet();
            *(out->GetPacket()) << NETWORK_ACTION_LAN_DISCOVERY_RESPONSE;
            std::cout << "Sending LAN Discovery Response" << std::endl;
            network->SendPacket(ip_address, out);
            // TODO: Cleanup Packet
        } else {
            ProcessPacket(action_type_id, ip_address, packet);
        }
    }
};
