#ifndef EGSERVER_NETWORKING_H
#define EGSERVER_NETWORKING_H

#define EG_ENGINE_TCP_PORT 42424
#define EG_ENGINE_UDP_PORT 42425

#define NETWORK_SERVER_CLIENT_ID 0

#define NETWORK_ACTION_AUTH 1
#define NETWORK_ACTION_AUTH_RESPONSE 2
#define NETWORK_ACTION_MESSAGE_BROADCAST 3
#define NETWORK_ACTION_MESSAGE_RELAY 4
#define NETWORK_ACTION_LAN_DISCOVERY 5
#define NETWORK_ACTION_LAN_DISCOVERY_RESPONSE 5

#include <iostream>
#include <queue>
#include <map>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include "Packet.h"

namespace EGServer {
    class Networking {
        public:
            enum ClientState {
                CLIENT_DISCONNECTED = 0,
                CLIENT_CONNECTED = 1,
                CLIENT_AUTHENTICATED = 2
            };

            Networking(void);
            ~Networking(void);

            void Update(void);
            // TCP Send
            void SendPacket(unsigned int client_id, Packet *p);
            // UDP Send
            void SendPacket(sf::IpAddress &ip_address, Packet *p);
            // TCP Recieve
            Packet *ReceivePacket(bool &response, unsigned int &client_id);
            // UDP Recieve TODO: Make this same name as above
            Packet *ReceiveConnectionlessPacket(bool &response, sf::IpAddress &ip_address);

            // Gets the next available Client ID
            unsigned int GetNextClientId(void);
            // Gets a TCP Client's State
            ClientState GetClientState(unsigned int client_id);
            // Updates a TCP Client's State
            void SetClientState(unsigned int client_id, ClientState state);
            // Gets the SFML TCP Socket
            sf::TcpSocket *GetClientSocket(unsigned int client_id);

            // Iterators over the tcp client sockets
            std::map<unsigned int, sf::TcpSocket *>::iterator ClientsBegin(void);
            std::map<unsigned int, sf::TcpSocket *>::iterator ClientsEnd(void);
        private:
            // Connection and Packet Acceptance
            // Threads
            sf::Thread *thread;
            // Mutexes
            sf::Mutex mutex;
            // Functions
            void Listener(void);
            // Main Thread is for Data Processing

            // New Connection Storage
            sf::TcpListener listener;
            sf::TcpSocket *current_client;

            // Next Packet to Collect
            Packet *next_packet;

            // UDP Socket
            sf::UdpSocket udp;

            // TCP Sockets
            sf::SocketSelector selector;
            std::map<unsigned int, sf::TcpSocket *> clients;

            // Client States
            std::map<unsigned int, ClientState> client_states;

            // Packets Recieved
            std::queue<std::pair<Packet *, unsigned int> > packets_received;
            std::queue<std::pair<Packet *, sf::IpAddress> > udp_packets_received;
    };
};

#endif
