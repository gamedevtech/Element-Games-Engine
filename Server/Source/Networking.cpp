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
            client_states[client_id] = CLIENT_CONNECTED;
        }

        std::map<unsigned int, sf::TcpSocket *>::iterator client_iterator = clients.begin();
        while (client_iterator != clients.end()) {
            unsigned int client_id = client_iterator->first;
            sf::TcpSocket *client = client_iterator->second;
            ClientState client_state = client_states[client_id];

            bool done = false;
            while (!done) {
                sf::Socket::Status s = client->receive(*(next_packet->GetPacket()));
                if (s == sf::Socket::Disconnected) {
                    std::cout << "Client Disconnected: " << client->getRemoteAddress() << std::endl;
                    clients.erase(client_iterator);
                    client_states.erase(client_states.find(client_id));
                    done = true;
                } else if (s == sf::Socket::Error) {
                    // Handle Error
                    std::cout << "Client Error: " << client->getRemoteAddress() << std::endl;
                    done = true;
                } else if (s == sf::Socket::Done) {
                    // Handle Data
                    packets_received.push(std::pair<Packet *, unsigned int>(next_packet, client_id));
                    next_packet = new Packet();
                    std::cout << "Client Data: " << client->getRemoteAddress() << std::endl;
                } else if (s == sf::Socket::NotReady) {
                    done = true; // waiting
                }
            }

            ++client_iterator;
        }
    }

    void Networking::SendPacket(unsigned int client_id, Packet *p, bool connectionless) {
        if (connectionless) {
            // TODO: UDP
        } else {
            if (clients.count(client_id) > 0) {
                sf::TcpSocket *s = clients[client_id];
                s->send(*(p->GetPacket()));
            }
        }
    }

    Packet *Networking::ReceivePacket(bool &response, unsigned int &client_id) {
        if (packets_received.empty()) {
            response = false;
            return NULL;
        }
        std::pair<Packet *, unsigned int> r = packets_received.front();
        client_id = r.second;
        packets_received.pop();
        response = true;
        return r.first;
    }

    Networking::ClientState Networking::GetClientState(unsigned int client_id) {
        if (client_states.count(client_id) > 0) {
            return client_states[client_id];
        }
        return CLIENT_DISCONNECTED;
    }

    void Networking::SetClientState(unsigned int client_id, ClientState state) {
        client_states[client_id] = state;
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
