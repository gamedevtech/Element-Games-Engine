#include "SpaceSimServer.h"
#include "../Common/SpaceSimNetworkActionTypes.h"

SpaceSimServer::SpaceSimServer(void) : EGServer::Server() {
    //
}

SpaceSimServer::~SpaceSimServer(void) {
    //
}

void SpaceSimServer::ProcessPacket(unsigned int client_id, EGServer::Packet* packet) {
    sf::Packet *sfp = packet->GetPacket();
    unsigned int received_client_id, action_type_id;
    *(sfp) >> action_type_id >> received_client_id;
    std::cout << "Processing Packet, Client: " << received_client_id << " Action Type: " << action_type_id << std::endl;
    if (action_type_id == NETWORK_ACTION_MESSAGE_BROADCAST) {
        std::string message;
        *(sfp) >> message;
        std::cout << "Message Recieved From " << received_client_id << "@" << network->GetClientSocket(received_client_id)->getRemoteAddress() << " Saying: " << message << std::endl;
        std::map<unsigned int, sf::TcpSocket *>::iterator client_iter = network->ClientsBegin();
        while (client_iter != network->ClientsEnd()) {
            unsigned int to_client_id = client_iter->first;
            sf::TcpSocket *client = client_iter->second;

            if (to_client_id != received_client_id) {
                std::cout << "Relaying Message To: " << to_client_id << "@" << client->getRemoteAddress() << std::endl;
                sf::Packet p;
                p << NETWORK_ACTION_MESSAGE_RELAY << NETWORK_SERVER_CLIENT_ID << received_client_id << message;
                client->send(p);
            }

            ++client_iter;
        }
    }
}

void SpaceSimServer::ProcessPacket(sf::IpAddress ip_address, EGServer::Packet *packet) {
    sf::Packet *sfp = packet->GetPacket();
    unsigned int action_type_id, received_client_id;
    glm::vec3 pos;
    *(sfp) >> action_type_id >> received_client_id >> pos.x >> pos.y >> pos.z;
    if (action_type_id == NETWORK_ACTION_MOVEMENT_BROADCAST) {
        std::map<unsigned int, sf::TcpSocket *>::iterator client_iter = network->ClientsBegin();
        while (client_iter != network->ClientsEnd()) {
            unsigned int to_client_id = client_iter->first;
            sf::TcpSocket *client = client_iter->second;
            sf::IpAddress to_ip = client->getRemoteAddress();

            if (to_client_id != received_client_id) {
                EGServer::Packet *packet = new EGServer::Packet();
                *(packet->GetPacket()) << NETWORK_ACTION_MOVEMENT_BROADCAST_RELAY << NETWORK_SERVER_CLIENT_ID << received_client_id << pos.x << pos.y << pos.z;
                network->SendPacket(to_ip, packet);
            }

            ++client_iter;
        }
    }
}
