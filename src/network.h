#ifndef NETWORK_H
#define NETWORK_H

#include "customer.h"
#include "film.h"
#include "film_repository.h"
#include "user_repository.h"
#include "commands_handler.h"
#include <vector>
#include <map>

class Network {
protected:
    int revenue;
    Customer* logged_in_user;
    std::map<int, int> publishers_revenue;
    UserRepository user_repository;
    FilmRepository film_repository;
    CommandsHandler commands_handler;
public:
    Network();
    void signup(std::string email, std::string username, std::string password, int age, bool publisher);
    void login(std::string username, std::string password);
    void give_money(int publisher_id);
    FilmRepository* get_film_repository() { return &film_repository;    }
    Customer* get_logged_in_user()  {   return logged_in_user;  }
};

#endif