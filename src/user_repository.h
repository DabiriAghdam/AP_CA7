#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "customer.h"
#include <vector>

class UserRepository {
protected:
     std::vector<Customer*> users;
public:
    Customer* find(int customer_id);
    Customer* find(std::string username);
    void add(Customer* new_user);
    int get_users_count() { return users.size();    }    
};

#endif