#ifndef COMMANDS_HANDLER_H
#define COMMANDS_HANDLER_H

#include "exception.h"
#include <map>

#define OK "OK\n"
#define POST "POST"
#define PUT "PUT"
#define DEL "DELETE"
#define GET "GET"
#define LOGIN "login"
#define SIGNUP "signup"
#define ADD_FILM "films"
#define EDIT_FILM "put_films"
#define DELETE_FILM "delete_films"
#define FOLLOWERS "followers"
#define MONEY "money"
#define PUBLISHED "published"
#define REPLIES "replies"
#define BUY "buy"
#define ADD_COMMENT "comments"
#define DELETE_COMMENT "delete_comments"
#define RATE "rate"
#define PURCHASED "purchased"
#define NOTIFICATIONS "notifications"
#define READ "read"

class Network;

class CommandsHandler {
private:
    Network* net;
    
    void get_parameters(std::istringstream &line_stream, std::map<std::string, std::string> &params);
    void signup(std::map<std::string, std::string> parameters);
    void login(std::map<std::string, std::string> parameters);
    void add_film(std::map<std::string, std::string> parameters);
    void edit_film(std::map<std::string, std::string> parameters);
    void delete_film(std::map<std::string, std::string> parameters);
    void get_film(std::map<std::string, std::string> parameters);
    void follow(std::map<std::string, std::string> parameters);
    void money(std::map<std::string, std::string> parameters);
    void reply(std::map<std::string, std::string> parameters);
    void delete_comment(std::map<std::string, std::string> parameters);
    void add_comment(std::map<std::string, std::string> parameters);
    void buy_film(std::map<std::string, std::string> parameters);
    void rate_film(std::map<std::string, std::string> parameters);
public:
    CommandsHandler(Network* _net);
    void run();
};

#endif