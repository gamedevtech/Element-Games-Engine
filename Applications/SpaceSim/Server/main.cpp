#include <iostream>
#include "SpaceSimServer.h"
#include "../Common/SpaceSimNetworkActionTypes.h"

int main(int argc, char **argv) {
    SpaceSimServer *server = new SpaceSimServer();
    while (true) {
        server->Update();
    }
}
