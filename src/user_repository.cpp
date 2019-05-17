#include "user_repository.h"

using namespace std;

Customer* UserRepository::find(int customer_id)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].get_id() == customer_id)
            return &users[i];
    }
    return NULL;//throw exception?
}

Customer* UserRepository::find(string username)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].get_username() == username)
            return &users[i];
    }
    return NULL;
}

void UserRepository::add(Customer new_user)
{
    users.push_back(new_user);
}
