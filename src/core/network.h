#ifndef NETWORK_H
#define NETWORK_H

#include "publisher.h"
#include "film_repository.h"
#include "user_repository.h"
#include "http_server.h"

#define PORT 5000
#define LOW 5
#define MID 8
#define WEAK 0.8
#define AVERAGE 0.9
#define STRONG 0.95
#define ADMIN_USER "admin"
#define ADMIN_PASS "admin"
#define ADMIN_EMAIL "admin@admin.com"

class Network {
protected:
    int revenue;
    std::map<int, Customer*> logged_in_users;
    std::map<int, int> publishers_revenue;
    UserRepository user_repository;
    FilmRepository film_repository;
    HttpServer http_server;

    void check_logged_in(int user_id);
    void check_user_access(int user_id);
    float get_percent(int film_id);
    void print_film_details(Film* film);
    void print_recommended_films(int user_id, Film* film);
    void print_film_comments(Film* film);
    bool logged_in(int user_id);
    bool is_admin(int user_id);
    std::map<std::string, std::string> get_published_films(int user_id, std::map<std::string, std::string> filters);
    std::map<std::string, std::string> get_purchased_films(int user_id, std::map<std::string, std::string> filters);
public:
    Network();
    void start();
    void initialize_handlers();
    void logout(int user_id);
    int signup(int user_id, std::string email, std::string username, std::string password, int age, bool publisher);
    int login(int user_id, std::string username, std::string password);
    void give_money(int user_id);
    void get_money(int user_id);
    void reply(int user_id, int film_id, int comment_id, std::string content);
    void delete_comment(int user_id, int film_id, int comment_id);
    void delete_film(int user_id, int film_id);
    void add_comment(int user_id, int film_id, std::string);
    void edit_film(int user_id, int film_id, std::map<std::string, std::string> parameters);
    void add_film(int user_id, int year, int length, int price, std::string name, std::string summary, std::string director);
    void get_followers(int user_id);
    void follow(int user_id, int publisher_id);
    void inc_money(int user_id, int value);
    std::map<std::string, std::string> get_details(int user_id, int film_id);
    void buy_film(int user_id, int film_id);
    void rate_film(int user_id, int film_id, int score);
    void find_films(int user_id, std::map<std::string, std::string> filters);
    std::map<std::string, std::string> get_home_films(int user_id);
    std::map<std::string, std::string> get_profile_films(int user_id);
    void get_unread_notifications(int user_id);
    void get_all_notifications(int user_id, int limit);
};

#endif