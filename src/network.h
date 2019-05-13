#ifndef NETWORK_H
#define NETWORK_H

#include "customer.h"
#include "film.h"
#include "film_repository.h"
#include "commands_handler.h"
#include <vector>
#include <map>

class Network {
protected:
    int revenue;
    Customer* logged_in_user;
    std::vector<Customer> users;
    std::map<int, int> publishers_revenue;
    Film_Repository film_repository;
    Commands_Handler commands_handler;
public:
    Network() : commands_handler(this) { revenue = 0; commands_handler.start(); }
    void signup(std::string email, std::string username, std::string password, int age, bool publisher);
    void login(std::string username, std::string password);
    void give_money(int publisher_id);
    Film_Repository* get_film_repository() { return &film_repository;    }
    Customer* get_logged_in_user()  {   return logged_in_user;  }
    Customer* find(int id);
};

#endif