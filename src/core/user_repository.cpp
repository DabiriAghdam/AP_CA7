#include "user_repository.h"

using namespace std;

UserRepository::~UserRepository()
{
    for (int i = 0; i < users.size(); i++)
        delete users[i];
}

Customer* UserRepository::find(int customer_id)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i]->get_id() == customer_id)
            return users[i];
    }
    return NULL;
}

Customer* UserRepository::find(string username)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i]->get_username() == username)
            return users[i];
    }
    return NULL;
}

void UserRepository::add(Customer* new_user)
{
    users.push_back(new_user);
}

int UserRepository::get_users_count() 
{ 
    return users.size();    
}    
