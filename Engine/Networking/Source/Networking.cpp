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
            polling_lan = false;
            next_packet = new Packet();
            udp->bind(EG_ENGINE_UDP_SERVER_PORT);
        }

        Network::~Network(void) {
            //
        }

        void Network::PollLAN(void) {
            server_ip_addresses.clear();
            polling_lan = true;
            sf::Packet p;
            p << NETWORK_ACTION_LAN_DISCOVERY;
            udp->send(p, sf::IpAddress::Broadcast, EG_ENGINE_UDP_CLIENT_PORT);
            timer = 0.0f;
            std::cout << "Sent LAN Discovery UDP Packet" << std::endl;
            server_ip_addresses.clear();
        }

        void Network::Connect(std::string server_address, std::string username, std::string password) {
            // Negotiate A Connection0
            server_ip_address = new sf::IpAddress(server_address);
            tcp->connect(*server_ip_address, EG_ENGINE_TCP_PORT);
            connected = true;

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
                udp->send(*(p->GetPacket()), *server_ip_address, EG_ENGINE_UDP_CLIENT_PORT);
            } else {
                tcp->send(*(p->GetPacket()));
            }
        }

        void Network::Update(float frame_time) {
            timer += frame_time;

            // TODO: UDP Receive
            if (polling_lan) {
                if (timer >= 2.0f) {
                    polling_lan = false;
                    std::vector<sf::IpAddress>::iterator ip_iter = server_ip_addresses.begin();
                    std::cout << "Done Polling... Recieved " << server_ip_addresses.size() << " Responses" << std::endl;
                    while (ip_iter != server_ip_addresses.end()) {
                        sf::IpAddress server_ip = *(ip_iter);
                        std::cout << "Recived Poll Response From: " << server_ip.toString() << std::endl;
                        ++ip_iter;
                    }
                }
            }

            bool done = false;
            while (!done) {
                sf::Packet *sfp = next_packet->GetPacket();
                sf::IpAddress ip_address;
                short unsigned int port;
                sf::Socket::Status s = udp->receive(*sfp, ip_address, port);

                if (s == sf::Socket::Done) {
                    std::cout << "Received UDP Packet" << std::endl;
                    if (polling_lan) {
                        unsigned int action_type;
                        *(sfp) >> action_type;
                        if (action_type == NETWORK_ACTION_LAN_DISCOVERY_RESPONSE) {
                            server_ip_addresses.push_back(ip_address);
                            next_packet = new Packet();
                            std::cout << "Recieved Server" << std::endl;
                        }
                    } else {
                        // TODO: Add to packets received.
                    }
                } else if (s == sf::Socket::NotReady) {
                    done = true;
                } else if (s == sf::Socket::Disconnected) {
                    done = true;
                } else if (s == sf::Socket::Error) {
                    done = true;
                }
            }

            done = false;
            if (!connected) {
                done = true;
            }
            while (!done) {
                sf::Packet *sfp = next_packet->GetPacket();
                sf::Socket::Status s = tcp->receive(*(next_packet->GetPacket()));

                if (s == sf::Socket::Done) {
                    if (!authenticated) {
                        unsigned int action_type;
                        *sfp >> action_type;
                        if (action_type == NETWORK_ACTION_AUTH_RESPONSE) {
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

        bool Network::IsPollingLAN(void) {
            return polling_lan;
        }

        std::vector<sf::IpAddress> *Network::GetServerIpAddresses(void) {
            return &server_ip_addresses;
        }
    };
};
