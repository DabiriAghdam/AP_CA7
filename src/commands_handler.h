#ifndef ORDERS_HANDLER_H
#define ORDERS_HANDLER_H

#include "exception.h"
#include <map>

#define OK "OK\n"
#define POST "POST"
#define PUT "PUT"
#define DEL "DELETE"
#define GET "GET"
//login o ... ro ham define kon

class Network;

class CommandsHandler {
private:
    Network* net;
    void get_parameters(std::istringstream &line_stream, std::map<std::string, std::string> &params);
public:
    CommandsHandler(Network* _net);
    void run();
};

#endif