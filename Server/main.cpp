#include <iostream>
#include "Networking.h"

int main(int argc, char **argv) {
    EGServer::Networking *n = new EGServer::Networking();
    while (true) {
        n->Update();
    }
}
