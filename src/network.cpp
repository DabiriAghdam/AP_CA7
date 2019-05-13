#include "network.h"

void Network::signup(std::string email, std::string username, std::string password, int age, bool publisher)
{
    int id = users.size() + 1;
    Customer new_user(id, age, 0, email, username, password);
    users.push_back(new_user);
    logged_in_user = &new_user;
}