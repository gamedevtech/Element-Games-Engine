#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

int main(int argc, char **argv){
	std::cout << "Starting..." << std::endl;

	sf::IpAddress Address1;                  // By default : invalid address
	sf::IpAddress Address2("192.168.0.1");   // From a string representation
	sf::IpAddress Address3("computer_name"); // From a host name
	sf::IpAddress Address4(192, 168, 0, 1);  // From 4 bytes
	sf::IpAddress Address5 = sf::IpAddress::LocalHost; // 127.0.0.1 -- your own computer
	// Your address in the local area network (like 192.168.1.100 -- the one you get with ipconfig)
	sf::IpAddress Address6 = sf::IpAddress::GetLocalAddress();
	// Your address in the world wide web (like 83.2.124.68 -- the one you get with www.whatismyip.org)
	sf::IpAddress Address7 = sf::IpAddress::GetPublicAddress();

	std::cout << "Local Address: " << Address6.ToString() << std::endl;
	std::cout << "Public Address: " << Address7.ToString() << std::endl;

	std::cout << "Stopping..." << std::endl;

	return 0;
}

