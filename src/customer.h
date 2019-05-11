#ifndef USER_H
#define USER_H

#include "film.h"
#include "notification.h"
#include <string>
#include <vector>

// class Publisher;

class Customer {
protected:
    int id;
    int age;
    int money;
    std::string email;
    std::string username;
    std::string password;
    std::vector<Film*>  purchased_films;
    std::vector<Notification> notifications; 
    // std::vector<Publisher*> following;
public:

};

#endif