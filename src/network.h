#ifndef NETWORK_H
#define NETWORK_H

#include "customer.h"
#include "publisher.h"
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
    bool signup(std::string email, std::string username, std::string password, int age, bool publisher);
    bool login(std::string username, std::string password);
    void give_money(int publisher_id);
    bool delete_film(int id);
    bool edit_film(std::map<std::string, std::string> parameters);
    bool add_film(int year, int length, int price, std::string name, std::string summary, std::string director);
    FilmRepository* get_film_repository() { return &film_repository;    }
    void start();
    // Customer* get_logged_in_user()  {   return logged_in_user;  }
};

#endif