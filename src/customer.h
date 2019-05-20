#ifndef USER_H
#define USER_H

#include "film.h"
#include "notification.h"
#include <string>
#include <vector>

#define CUSTOMER "Customer"

class Customer {
protected:
    int id;
    int age;
    int money;
    std::string email;
    std::string username;
    std::string password;
    std::vector<Film*>  purchased_films;
    std::vector<Notification*> notifications; 
    // std::vector<Publisher*> following;
public:
    Customer(int _id, int _age, int _money, std::string _email, std::string _username, std::string _password);
    void inc_money(int value) {  money += value; }
    int get_id() {  return id;  }
    std::string get_username() {    return username;    }
    std::string get_password() {    return password;    }
    std::string get_email() {   return email;   }
    virtual std::string get_type() {    return CUSTOMER;  }
    // std::vector<Film*> get_purchased_films() {  return purchased_films; }
    Film* find_in_purchased_films(int film_id);

};

#endif