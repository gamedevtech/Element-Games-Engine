#include <iostream>
#include "../../../Server/Networking.h"
#include "../Common/SpaceSimNetworkActionTypes.h"

void ProcessPacket(EGServer::Networking *network, EGServer::Packet *packet) {
    sf::Packet *sfp = packet->GetPacket();
    unsigned int client_id, action_type_id;
    *(sfp) >> client_id >> action_type_id;
    std::cout << "Processing Packet, Client: " << client_id << " Action Type: " << action_type_id << std::endl;
    if (action_type_id == NETWORK_ACTION_MESSAGE_BROADCAST) {
        std::string message;
        *(sfp) >> message;
        std::cout << "Message Recieved From " << client_id << "@" << network->GetClientSocket(client_id)->getRemoteAddress() << " Saying: " << message << std::endl;
        std::map<unsigned int, sf::TcpSocket *>::iterator client_iter = network->ClientsBegin();
        while (client_iter != network->ClientsEnd()) {
            unsigned int to_client_id = client_iter->first;
            sf::TcpSocket *client = client_iter->second;

            if (to_client_id != client_id) {
                std::cout << "Relaying Message To: " << to_client_id << "@" << client->getRemoteAddress() << std::endl;
                sf::Packet p;
                p << NETWORK_SERVER_CLIENT_ID << NETWORK_ACTION_MESSAGE_RELAY << client_id << message;
                client->send(p);
            }

            ++client_iter;
        }
    }
}

int main(int argc, char **argv) {
    EGServer::Networking *network = new EGServer::Networking;
    while (true) {
        network->Update();
        bool has_packet = false;
        EGServer::Packet *packet = network->ReceivePacket(has_packet);
        while (has_packet) {
            ProcessPacket(network, packet);
            packet = network->ReceivePacket(has_packet);
        }
    }
    return 0;
}
