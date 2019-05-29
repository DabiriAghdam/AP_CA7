#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "customer.h"

class UserRepository {
protected:
     std::vector<Customer*> users;
public:
    ~UserRepository();
    Customer* find(int customer_id);
    Customer* find(std::string username);
    void add(Customer* new_user);
    int get_users_count();
};

#endif