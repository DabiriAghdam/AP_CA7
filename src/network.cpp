#include "network.h"
#include <iostream>

using namespace std;

Network::Network() : commands_handler(this) 
{ 
    revenue = 0;
}

void Network::start()
{
    commands_handler.run();
}

bool Network::signup(string email, string username, string password, int age, bool publisher)
{
    Customer* exist = user_repository.find(username);
    if (exist)
        throw Bad_Request_Ex();
    int id = user_repository.get_users_count() + 1;
    if (!publisher)
    {
        Customer* new_user = new Customer(id, age, 0, email, username, password);
        user_repository.add(new_user);
    }
    else 
    {
        Publisher* new_user = new Publisher(id, age, 0, email, username, password);
        user_repository.add(new_user);
    }
    logged_in_user = user_repository.find(id);
    return true;
}

bool Network::login(string username, string password)
{
    Customer* user = user_repository.find(username);
    if (user == NULL || user->get_password() != password)
        throw Bad_Request_Ex();
    else 
        logged_in_user = user;
}

bool Network::add_film(int year, int length, int price, std::string name, std::string summary, std::string director)
{
    if (logged_in_user->get_type() == CUSTOMER)
        throw Permission_Denied_Ex();
    int id = film_repository.get_films_count() + 1;
    Film new_film(id, year, length, price, name, summary, director, (Publisher*)logged_in_user);
    film_repository.add(new_film);
    return true;
}

bool Network::delete_film(int id)
{
    Film* film = film_repository.find(id);
    if (logged_in_user->get_type() == CUSTOMER)
        throw Permission_Denied_Ex();
    if (film == NULL)
        throw Not_Found_Ex();
    else if (film->get_publisher() != (Publisher*)logged_in_user)
        throw Permission_Denied_Ex();
    film_repository.remove(id);
    return true;
}

bool Network::edit_film(map<string, string> parameters)
{
    //todo
}
