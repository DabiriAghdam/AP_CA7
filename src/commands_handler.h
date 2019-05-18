#ifndef ORDERS_HANDLER_H
#define ORDERS_HANDLER_H

#include "exception.h"
#include <map>

#define OK "OK\n"
#define POST "POST"
#define PUT "PUT"
#define DEL "DELETE"
#define GET "GET"
#define LOGIN "login"
#define SIGNUP "signup"
#define FILMS "films"
#define FOLLOWERS "followers"
#define MONEY "money"
#define PUBLISHED "published"
#define REPLIES "replies"
#define BUY "buy"
#define COMMENTS "comments"
#define RATE "rate"
#define PURCHASED "purchased"
#define NOTIFICATIONS "notifications"

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