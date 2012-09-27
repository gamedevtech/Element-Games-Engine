#include "../Networking.h"
#include "../../Utility/Console.h"

namespace EG {
    namespace Network {
        Network::Network(std::string server_ip_string) {
            tcp = new sf::TcpSocket();
            tcp->setBlocking(false);
            udp = new sf::UdpSocket();
            udp->setBlocking(false);
            client_id = 999999999999;
            has_client_id = false;
            next_packet = new Packet();

            server_ip_address = new sf::IpAddress(server_ip_string);
            server_ip_address->toInteger();
            if (server_ip_address->toInteger() == 0) {
                delete server_ip_address;
                server_ip_address = new sf::IpAddress("127.0.0.1");
            }

            sf::Socket::Status s = tcp->connect(*server_ip_address, EG_ENGINE_PORT);
            connected = true;
            udp->bind(EG_ENGINE_PORT);
        }

        Network::~Network(void) {
            //
        }

        void Network::PrintStatus(sf::Socket::Status s) {
            if (s == sf::Socket::Done) {
                std::cout << "Socket Status: Done" << std::endl;
            } else if (s == sf::Socket::Disconnected) {
                std::cout << "Socket Status: Disconnected" << std::endl;
            } else if (s == sf::Socket::Error) {
                std::cout << "Socket Status: Error" << std::endl;
            } else if (s == sf::Socket::NotReady) {
                std::cout << "Socket Status: NotReady" << std::endl;
            }
        }

        void Network::SendPacket(Packet *p, bool connectionless) {
            if (connectionless) {
                udp->send(*(p->GetPacket()), *server_ip_address, EG_ENGINE_PORT);
            } else {
                tcp->send(*(p->GetPacket()));
            }
        }

        void Network::Update(void) {
            // TODO: UDP Receive
            if (!connected) {
                return;
            }

            bool done = false;
            while (!done) {
                sf::Packet *sfp = next_packet->GetPacket();
                sf::Socket::Status s = tcp->receive(*(next_packet->GetPacket()));

                if (s == sf::Socket::Done) {
                    std::cout << "Receiving Data" << std::endl;
                    if (!has_client_id) {
                        *sfp >> client_id;
                        std::cout << "Client ID From Server: " << client_id << std::endl;
                        has_client_id = true;
                    } else {
                        packets_received.push(next_packet);
                        next_packet = new Packet();
                    }
                } else if (s == sf::Socket::NotReady) {
                    // Nothing to receive!
                    done = true;
                } else if (s == sf::Socket::Disconnected) {
                    std::cout << "Disconnected" << std::endl;
                    connected = false;
                    done = true;
                } else if (s == sf::Socket::Error) {
                    std::cout << "Error" << std::endl;
                    done = true;
                }
            }
        }

        Packet *Network::ReceivePacket(bool &response) {
            if (packets_received.empty()) {
                response = false;
                return NULL;
            }
            Packet *out = packets_received.front();
            packets_received.pop();
            response = true;
            return out;
        }

        unsigned int Network::GetClientId(void) {
            return client_id;
        }
    };
};
