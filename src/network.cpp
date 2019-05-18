#include "network.h"
#include <iostream>

using namespace std;

Network::Network() : commands_handler(this) 
{ 
    revenue = 0;
    logged_in_user = NULL;
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
    if (publisher)
    {
        Publisher* new_user = new Publisher(id, age, 0, email, username, password);
        user_repository.add(new_user);
        publishers_revenue[id] = 0;
    }
    else 
    {
        Customer* new_user = new Customer(id, age, 0, email, username, password);
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
    check_user_access();
    int id = film_repository.get_films_count() + 1;
    Film new_film(id, year, length, price, name, summary, director, (Publisher*)logged_in_user);
    film_repository.add(new_film);
    return true;
}

bool Network::delete_film(int id)
{
    check_user_access();
    Film* film = film_repository.find(id);
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

void Network::get_followers()
{
    check_user_access();
    vector<Customer*> followers = ((Publisher*)logged_in_user)->get_followers();
    cout << "List of followers" << endl << "#. User Id | User Username | User Email" << endl;
    for (int i = 0; i < followers.size(); i++)
    {
        cout << i + 1 << ". " << followers[i]->get_id() << " | " << followers[i]->get_username()
            << " | " << followers[i]->get_email() << endl;
    }
}

bool Network::give_money()
{
    check_user_access();
    int money = publishers_revenue[logged_in_user->get_id()];
    logged_in_user->inc_money(money);
    publishers_revenue[logged_in_user->get_id()] = 0;
    return true;
}

void Network::check_user_access()
{
    if (logged_in_user == NULL || logged_in_user->get_type() == CUSTOMER)
        throw Permission_Denied_Ex();   
}

bool Network::reply(int film_id, int comment_id, string content)
{
    check_user_access();
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    Comment* comment = film->get_comment(comment_id);
    if (comment == NULL)
        throw Not_Found_Ex();
    comment->add_reply(content);
    // author->add_notification(); //todo
    return true;
}

bool Network::delete_comment(int film_id, int comment_id)
{
    check_user_access();
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    film->delete_comment(comment_id);
    return true;
}
