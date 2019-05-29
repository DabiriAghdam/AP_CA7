#include "network.h"

int main()
{
    Network net;
    net.initialize_handlers();
    net.start();
    return 0;
}