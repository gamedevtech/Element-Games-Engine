#include "../Networking.h"

namespace EGServer {
    Networking::Networking(void) {
        thread = new sf::Thread(&Networking::Listener, this);
        listener.listen(EG_ENGINE_TCP_PORT);
        selector.add(listener);
        udp.bind(EG_ENGINE_UDP_CLIENT_PORT);
        selector.add(udp);
        current_client = new sf::TcpSocket();
        next_packet = new Packet();
        thread->launch();
    }

    Networking::~Networking(void) {
        thread->terminate();
        delete thread;
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

    void Networking::Listener(void) {
        std::cout << "Listening to Network" << std::endl;
        while (true) {
            while (selector.wait()) {
                mutex.lock();

                // New Connection
                if (selector.isReady(listener)) {
                    if (listener.accept(*current_client) == sf::Socket::Done) {
                        unsigned int client_id = GetNextClientId();
                        clients[client_id] = current_client;
                        selector.add(*(clients[client_id]));
                        current_client = new sf::TcpSocket();
                        client_states[client_id] = CLIENT_CONNECTED;
                        sf::Packet p;
                        p << NETWORK_ACTION_AUTH_RESPONSE << NETWORK_SERVER_CLIENT_ID << 1 << client_id;
                        clients[client_id]->send(p);
                        std::cout << "New Connection From: " << clients[client_id]->getRemoteAddress() << std::endl;
                    }
                }

                // Receive Data
                if (selector.isReady(udp)) {
                    sf::IpAddress received_ip;
                    unsigned short int received_port;
                    sf::Socket::Status s = udp.receive(*(next_packet->GetPacket()), received_ip, received_port);
                    //std::cout << "Received UDP Packets From " << received_ip.toString() << "@" << received_port << std::endl;
                    if (s == sf::Socket::Done) {
                        udp_packets_received.push(std::pair<Packet *, sf::IpAddress>(next_packet, received_ip));
                        next_packet = new Packet();
                    }
                }

                std::map<unsigned int, sf::TcpSocket *>::iterator client_iterator = clients.begin();
                while (client_iterator != clients.end()) {
                    unsigned int client_id = client_iterator->first;
                    sf::TcpSocket *client = client_iterator->second;

                    if (selector.isReady(*client)) {
                        sf::Socket::Status s = client->receive(*(next_packet->GetPacket()));
                        if (s == sf::Socket::Disconnected) {
                            std::cout << "Client Disconnected: " << client->getRemoteAddress() << std::endl;
                            clients.erase(client_iterator);
                            client_states.erase(client_states.find(client_id));
                            selector.remove(*client);
                        } else if (s == sf::Socket::Error) {
                            std::cout << "Client Error: " << client->getRemoteAddress() << std::endl;
                        } else if (s == sf::Socket::Done) {
                            packets_received.push(std::pair<Packet *, unsigned int>(next_packet, client_id));
                            next_packet = new Packet();
                        }
                    }

                    ++client_iterator;
                }
                mutex.unlock();
            }
            std::cout << "Done Listening To TCP" << std::endl;
        }
    }

    void Networking::Update(void) {
        // NOTE: Is this function needed anymore?
    }

    void Networking::SendPacket(unsigned int client_id, Packet *p) {
        mutex.lock();
        if (clients.count(client_id) > 0) {
            sf::TcpSocket *s = clients[client_id];
            s->send(*(p->GetPacket()));
        }
        mutex.unlock();
    }

    void Networking::SendPacket(sf::IpAddress &ip_address, Packet *p) {
        mutex.lock();
        udp.send(*(p->GetPacket()), ip_address, EG_ENGINE_UDP_SERVER_PORT);
        mutex.unlock();
    }

    Packet *Networking::ReceivePacket(bool &response, unsigned int &client_id) {
        mutex.lock();
        if (packets_received.empty()) {
            response = false;
            mutex.unlock();
            return NULL;
        }
        std::pair<Packet *, unsigned int> r = packets_received.front();
        client_id = r.second;
        packets_received.pop();
        response = true;
        mutex.unlock();
        return r.first;
    }

    Packet *Networking::ReceivePacket(bool &response, sf::IpAddress &ip_address) {
        mutex.lock();
        response = false;
        if (udp_packets_received.empty()) {
            mutex.unlock();
            return NULL;
        }
        std::pair<Packet *, sf::IpAddress> r = udp_packets_received.front();
        ip_address = r.second;
        udp_packets_received.pop();
        response = true;
        mutex.unlock();
        return r.first;
    }

    Networking::ClientState Networking::GetClientState(unsigned int client_id) {
        mutex.lock();
        if (client_states.count(client_id) > 0) {
            mutex.unlock();
            return client_states[client_id];
        }
        mutex.unlock();
        return CLIENT_DISCONNECTED;
    }

    void Networking::SetClientState(unsigned int client_id, ClientState state) {
        mutex.lock();
        client_states[client_id] = state;
        mutex.unlock();
    }

    sf::TcpSocket *Networking::GetClientSocket(unsigned int client_id) {
        mutex.lock();
        if (clients.count(client_id) > 0) {
            mutex.unlock();
            return clients[client_id];
        }
        mutex.unlock();
        return NULL;
    }

    std::map<unsigned int, sf::TcpSocket *>::iterator Networking::ClientsBegin(void) {
        return clients.begin();
    }

    std::map<unsigned int, sf::TcpSocket *>::iterator Networking::ClientsEnd(void) {
        return clients.end();
    }
};
