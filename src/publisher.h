#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "customer.h"

#define PUBLISHER "Publisher"

class Publisher : public Customer {
protected:
    std::vector<Customer*> followers;
    std::vector<Film*> published_films;
public:
    Publisher(int _id, int _age, int _money, std::string _email, std::string _username, std::string _password);
    virtual std::string get_type();
    std::vector<Customer*> get_followers();
    void add_follower(Customer* follower);
    std::vector<Film*> get_published_films(std::map<std::string, std::string> filters);
    void add_film(Film* film);
    void notify_followers_new_film();
};

#endif