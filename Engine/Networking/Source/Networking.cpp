#include "../Networking.h"
#include "../../Utility/Console.h"

namespace EG {
    namespace Network {
        Network::Network(void) {
            tcp = new sf::TcpSocket();
            tcp->setBlocking(false);
            udp = new sf::UdpSocket();
            udp->setBlocking(false);
            client_id = 0;
            connected = false;
            authenticated = false;
            next_packet = new Packet();
        }

        Network::~Network(void) {
            //
        }

        std::vector<std::pair<std::string, std::string> > Network::PollLAN(void) {
            // Searches LAN for Servers
        }

        void Network::Connect(std::string server_address, std::string username, std::string password) {
            // Negotiate A Connection0
            server_ip_address = new sf::IpAddress(server_address);
            tcp->connect(*server_ip_address, EG_ENGINE_PORT);
            connected = true;
            udp->bind(EG_ENGINE_PORT);

            sf::Packet auth;
            auth << username << password;
            tcp->send(auth);
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
            if (!connected) {
                return;
            }

            // TODO: UDP Receive

            bool done = false;
            while (!done) {
                sf::Packet *sfp = next_packet->GetPacket();
                sf::Socket::Status s = tcp->receive(*(next_packet->GetPacket()));

                if (s == sf::Socket::Done) {
                    if (!authenticated) {
                        // Check For Authentication Packet
                        // TODO: Get Encryption Key Here
                        unsigned int result;
                        *sfp >> result >> client_id;
                        if (result == 1) {
                            authenticated = true;
                            std::cout << "Authenticated... Got Client ID: " << client_id << std::endl;
                        } else {
                            connected = false;
                            std::cout << "Authentication Failed!" << std::endl;
                        }
                    } else {
                        packets_received.push(next_packet);
                        next_packet = new Packet();
                    }
                } else if (s == sf::Socket::NotReady) {
                    // Nothing to receive!
                    done = true;
                } else if (s == sf::Socket::Disconnected) {
                    std::cout << "Disconnected" << std::endl;
                    authenticated = false;
                    connected = false;
                    client_id = 0;
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
