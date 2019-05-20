#include "customer.h"

using namespace std;

Customer::Customer(int _id,int _age, int _money, string _email, string _username, string _password)
{
    id = _id;
    age = _age;
    money = _money;
    email = _email;
    username = _username;
    password = _password;
}

Film* Customer::find_in_purchased_films(int film_id)
{
    for (int i = 0; i < purchased_films.size(); i++)
    {
        if (purchased_films[i]->get_id() == film_id)
            return purchased_films[i];
    }
    return NULL;
}