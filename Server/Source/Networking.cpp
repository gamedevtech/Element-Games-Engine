#include "../Networking.h"

namespace EGServer {
    Networking::Networking(void) {
        tcp_thread = new sf::Thread(&Networking::TCPListener, this);
        listener.listen(EG_ENGINE_TCP_PORT);
        selector.add(listener);
        //udp->bind(EG_ENGINE_UDP_PORT);
        current_client = new sf::TcpSocket();
        next_packet = new Packet();
        tcp_thread->launch();
        //udp_thread.launch();
    }

    Networking::~Networking(void) {
        tcp_thread->terminate();
        //udp_thread.terminate();
        delete tcp_thread;
        //delete udp_thread;
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

    void Networking::TCPListener(void) {
        std::cout << "Listening To TCP" << std::endl;
        while (selector.wait()) {
            std::cout << "Shit's Going Down!" << std::endl;
            //tcp_mutex.lock();

            // New Connection
            if (selector.isReady(listener)) {
                std::cout << "TCP Listener is Ready" << std::endl;
                if (listener.accept(*current_client) == sf::Socket::Done) {
                    unsigned int client_id = GetNextClientId();
                    clients[client_id] = current_client;
                    selector.add(*(clients[client_id]));
                    current_client = new sf::TcpSocket();
                    client_states[client_id] = CLIENT_CONNECTED;
                    std::cout << "New Connection From: " << clients[client_id]->getRemoteAddress() << std::endl;
                }
            }

            // Receive Data
            std::map<unsigned int, sf::TcpSocket *>::iterator client_iterator = clients.begin();
            while (client_iterator != clients.end()) {
                unsigned int client_id = client_iterator->first;
                sf::TcpSocket *client = client_iterator->second;

                if (selector.isReady(*client)) {
                    std::cout << "Client Socket for " << client_id << " is ready!" << std::endl;
                    sf::Socket::Status s = client->receive(*(next_packet->GetPacket()));
                    if (s == sf::Socket::Disconnected) {
                        std::cout << "Client Disconnected: " << client->getRemoteAddress() << std::endl;
                        clients.erase(client_iterator);
                        client_states.erase(client_states.find(client_id));
                        selector.remove(*client);
                    } else if (s == sf::Socket::Error) {
                        // Handle Error
                        std::cout << "Client Error: " << client->getRemoteAddress() << std::endl;
                    } else if (s == sf::Socket::Done) {
                        // Handle Data
                        packets_received.push(std::pair<Packet *, unsigned int>(next_packet, client_id));
                        next_packet = new Packet();
                        std::cout << "Client Data: " << client->getRemoteAddress() << std::endl;
                    } else if (s == sf::Socket::NotReady) {
                        // Not Ready
                    }
                }

                ++client_iterator;
            }
            //tcp_mutex.unlock();
        }
        std::cout << "Done Listening To TCP" << std::endl;
    }

    void Networking::UDPListener(void) {
        // Needs Work!
        std::cout << "Listening for UDP Packets" << std::endl;
        unsigned short int received_port;
        sf::IpAddress received_ip;
        sf::Socket::Status s;
        while (s = udp.receive(*(next_packet->GetPacket()), received_ip, received_port)) {
            if (s == sf::Socket::Done) {
                udp_mutex.lock();
                udp_packets_received.push(std::pair<Packet *, sf::IpAddress>(next_packet, received_ip));
                udp_mutex.unlock();
            }
        }
        std::cout << "Done Listening for UDP Packets" << std::endl;
    }

    void Networking::Update(void) {
        // NOTE: Is this function needed anymore?

        /*// Check for connections
        while (listener->accept(*current_client) == sf::Socket::Done) {
            std::cout << "New Connection From: " << current_client->getRemoteAddress() << std::endl;
            unsigned int client_id = GetNextClientId();
            clients[client_id] = current_client;
            clients[client_id]->setBlocking(false);
            current_client = new sf::TcpSocket();
            client_states[client_id] = CLIENT_CONNECTED;
        }

        bool done = false;
        while (!done) {
            unsigned short int received_port;
            sf::IpAddress received_ip;
            sf::Socket::Status s = udp->receive(*(next_packet->GetPacket()), received_ip, received_port);
            if (s == sf::Socket::Done) {
                udp_packets_received.push(std::pair<Packet *, sf::IpAddress>(next_packet, received_ip));
            } else {
                done = true;
            }
        }

        bool done = false;
        std::map<unsigned int, sf::TcpSocket *>::iterator client_iterator = clients.begin();
        while (client_iterator != clients.end()) {
            unsigned int client_id = client_iterator->first;
            sf::TcpSocket *client = client_iterator->second;
            ClientState client_state = client_states[client_id];

            done = false;
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
        }*/
    }

    void Networking::SendPacket(unsigned int client_id, Packet *p) {
        tcp_mutex.lock();
        if (clients.count(client_id) > 0) {
            sf::TcpSocket *s = clients[client_id];
            s->send(*(p->GetPacket()));
        }
        tcp_mutex.unlock();
    }

    void Networking::SendPacket(sf::IpAddress &ip_address, Packet *p) {
        udp_mutex.lock();
        udp.send(*(p->GetPacket()), ip_address, EG_ENGINE_UDP_PORT);
        udp_mutex.unlock();
    }

    Packet *Networking::ReceivePacket(bool &response, unsigned int &client_id) {
        tcp_mutex.lock();
        if (packets_received.empty()) {
            response = false;
            return NULL;
        }
        std::pair<Packet *, unsigned int> r = packets_received.front();
        client_id = r.second;
        packets_received.pop();
        response = true;
        tcp_mutex.unlock();
        return r.first;
    }

    Packet *Networking::ReceiveConnectionlessPacket(bool &response, sf::IpAddress &ip_address) {
        udp_mutex.lock();
        if (udp_packets_received.empty()) {
            response = false;
            return NULL;
        }
        std::pair<Packet *, sf::IpAddress> r = udp_packets_received.front();
        ip_address = r.second;
        udp_packets_received.pop();
        response = true;
        udp_mutex.unlock();
        return r.first;
    }

    Networking::ClientState Networking::GetClientState(unsigned int client_id) {
        tcp_mutex.lock();
        if (client_states.count(client_id) > 0) {
            return client_states[client_id];
        }
        tcp_mutex.unlock();
        return CLIENT_DISCONNECTED;
    }

    void Networking::SetClientState(unsigned int client_id, ClientState state) {
        tcp_mutex.lock();
        client_states[client_id] = state;
        tcp_mutex.unlock();
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
