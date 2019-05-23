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

    void check_user_access();
    float get_percent(int film_id);
public:
    Network();
    void signup(std::string email, std::string username, std::string password, int age, bool publisher);
    void login(std::string username, std::string password);
    void get_money();
    void reply(int film_id, int comment_id, std::string content);
    void delete_comment(int film_id, int comment_id);
    void delete_film(int film_id);
    void add_comment(int film_id, std::string);
    void edit_film(int film_id, std::map<std::string, std::string> parameters);
    void add_film(int year, int length, int price, std::string name, std::string summary, std::string director);
    FilmRepository* get_film_repository() { return &film_repository;    }
    void start();
    void get_followers();
    void follow(int publisher_id);
    void inc_money(int value);
    void get_details(int film_id);
    void buy_film(int film_id);
    void rate_film(int film_id, int score);
    void find_films(std::map<std::string, std::string> filters);
    void get_purchased_films(std::map<std::string, std::string> filters);
    void get_published_films(std::map<std::string, std::string> filters);
    void get_unread_notifications();
    void get_all_notifications(int limit);
};

#endif