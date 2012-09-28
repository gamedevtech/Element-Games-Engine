#include "SpaceSimServer.h"

SpaceSimServer::SpaceSimServer(void) : EGServer::Server() {
    //
}

SpaceSimServer::~SpaceSimServer(void) {
    //
}

void SpaceSimServer::ProcessPacket(unsigned int client_id, EGServer::Packet* packet) {
    sf::Packet *sfp = packet->GetPacket();
    unsigned int received_client_id, action_type_id;
    *(sfp) >> received_client_id >> action_type_id;
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
                p << NETWORK_SERVER_CLIENT_ID << NETWORK_ACTION_MESSAGE_RELAY << received_client_id << message;
                client->send(p);
            }

            ++client_iter;
        }
    }
}
