#ifndef ORDERS_HANDLER_H
#define ORDERS_HANDLER_H

#include "exception.h"
#include <map>

class Network;

class Commands_Handler {
private:
    Network* net;
    void get_parameters(std::istringstream &line_stream, std::map<std::string, std::string> &params);
public:
    Commands_Handler(Network* _net);
    void start();
};

#endif