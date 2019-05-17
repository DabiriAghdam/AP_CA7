#include "network.h"
#include <iostream>

using namespace std;

Network::Network() : commands_handler(this) 
{ 
    revenue = 0;
    commands_handler.start(); 
}

void Network::signup(string email, string username, string password, int age, bool publisher)
{
    int id = user_repository.get_users_count() + 1;
    Customer new_user(id, age, 0, email, username, password);
    user_repository.add(new_user);
    logged_in_user = &new_user;
}



void Network::login(string username, string password)
{
    Customer* user = user_repository.find(username);
    if (user == NULL || user->get_password() != password)
    {
        throw Bad_Request_Ex();
        //throw exception
    }
    else 
        logged_in_user = user;
}