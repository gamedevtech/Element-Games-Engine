#include "../Networking.h"

namespace EGServer {
    Networking::Networking(void) {
        listener = new sf::TcpListener();
        listener->setBlocking(false);
        listener->listen(EG_ENGINE_PORT);
        current_client = new sf::TcpSocket();
        next_packet = new Packet();
    }

    unsigned int Networking::GetNextClientId(void) {
        unsigned int i = 1;
        bool found = false;
        while (!found) {
            if (clients.count(i) < 1) {
                found = true;
            } else {
                i += 1;
            }
        }
        return i;
    }

    void Networking::Update(void) {
        // Check for connections
        if (listener->accept(*current_client) == sf::Socket::Done) {
            std::cout << "New Connection From: " << current_client->getRemoteAddress() << std::endl;
            unsigned int client_id = GetNextClientId();
            clients[client_id] = current_client;
            clients[client_id]->setBlocking(false);
            current_client = new sf::TcpSocket();
            sf::Packet p;
            p << client_id;
            clients[client_id]->send(p);
        }

        std::map<unsigned int, sf::TcpSocket *>::iterator client_iterator = clients.begin();
        while (client_iterator != clients.end()) {
            unsigned int client_id = client_iterator->first;
            sf::TcpSocket *client = client_iterator->second;

            bool done = false;
            while (!done) {
                sf::Socket::Status s = client->receive(*(next_packet->GetPacket()));
                if (s == sf::Socket::Disconnected) {
                    std::cout << "Client Disconnected: " << client->getRemoteAddress() << std::endl;
                    clients.erase(client_iterator);
                    done = true;
                } else if (s == sf::Socket::Error) {
                    // Handle Error
                    std::cout << "Client Error: " << client->getRemoteAddress() << std::endl;
                    done = true;
                } else if (s == sf::Socket::Done) {
                    // Handle Data
                    packets_received.push(next_packet);
                    next_packet = new Packet();
                    std::cout << "Client Data: " << client->getRemoteAddress() << std::endl;
                } else if (s == sf::Socket::NotReady) {
                    done = true; // waiting
                }
            }

            ++client_iterator;
        }
    }

    Packet *Networking::ReceivePacket(bool &response) {
        if (packets_received.empty()) {
            response = false;
            return NULL;
        }
        Packet *out = packets_received.front();
        packets_received.pop();
        response = true;
        return out;
    }

    sf::TcpSocket *Networking::GetClientSocket(unsigned int client_id) {
        if (clients.count(client_id) > 0) {
            return clients[client_id];
        }
        return NULL;
    }

    std::map<unsigned int, sf::TcpSocket *>::iterator Networking::ClientsBegin(void) {
        return clients.begin();
    }

    std::map<unsigned int, sf::TcpSocket *>::iterator Networking::ClientsEnd(void) {
        return clients.end();
    }
};
