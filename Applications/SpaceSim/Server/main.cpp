#include <iostream>
#include "SpaceSimServer.h"
#include "../Common/SpaceSimNetworkActionTypes.h"

int main(int argc, char **argv) {
    SpaceSimServer *server = new SpaceSimServer();
    while (!(server->IsDone())) {
        server->Update();
    }
    delete server;
    std::cout << "Main Thread Done" << std::endl;

    return 0;
}
