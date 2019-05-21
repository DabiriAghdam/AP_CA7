#include "network.h"
#include "md5.h"
#include <iostream>
#include <regex>

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

bool is_valid_email(const string& email)
{
   const std::regex pattern
      ("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
   return regex_match(email, pattern);
}

bool Network::signup(string email, string username, string password, int age, bool publisher)
{
    Customer* exist = user_repository.find(username);
    if (exist || !is_valid_email(email))
        throw Bad_Request_Ex();
    
    string hashed_pass = md5(password);
    int id = user_repository.get_users_count() + 1;
    if (publisher)
    {
        Publisher* new_user = new Publisher(id, age, 0, email, username, hashed_pass);
        user_repository.add(new_user);
        publishers_revenue[id] = 0;
    }
    else 
    {
        Customer* new_user = new Customer(id, age, 0, email, username, hashed_pass);
        user_repository.add(new_user);
    }
    logged_in_user = user_repository.find(id);
    return true;
}

bool Network::login(string username, string password)
{
    Customer* user = user_repository.find(username);
    string hashed_pass = md5(password);
    if (user == NULL || user->get_password() != hashed_pass)
        throw Bad_Request_Ex();
    else 
        logged_in_user = user;
}

bool Network::add_film(int year, int length, int price, std::string name, std::string summary, std::string director)
{
    check_user_access();
    int id = film_repository.get_films_count() + 1;
    Film* new_film = new Film(id, year, length, price, name, summary, director, (Publisher*)logged_in_user);
    film_repository.add(new_film);
    // (Publisher*)(logged_in_user)->notify_new_film(); //todo
    return true;
}

bool Network::delete_film(int film_id)
{
    check_user_access();
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    else if (film->get_publisher() != (Publisher*)logged_in_user)
        throw Permission_Denied_Ex();
    film_repository.remove(film_id);
    return true;
}

void Network::edit_film(int film_id, map<string, string> parameters)
{
    check_user_access();
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    else if (film->get_publisher() != (Publisher*)logged_in_user)
        throw Permission_Denied_Ex();
    for (map<string, string>::iterator it = parameters.begin(); it != parameters.end(); it++) //ok?
    {
        if (it->first == "name")
            film->set_name(it->second);
        else if (it->first == "year")
            film->set_year(stoi(it->second));
        else if (it->first == "length")
            film->set_length(stoi(it->second));
        else if (it->first == "summary")
            film->set_summary(it->second);
        else if (it->first == "director")
            film->set_director(it->second);
        else
            throw Bad_Request_Ex(); //are?!   
    }
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
    revenue -= money;
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

bool Network::follow(int publisher_id)
{
    if (logged_in_user == NULL)
        throw Permission_Denied_Ex();
    Customer* publisher = user_repository.find(publisher_id);
    if (publisher == NULL)
        throw Not_Found_Ex();
    ((Publisher*)publisher)->add_follower(logged_in_user);
    // ((Publisher*)publisher)->add_notification(); //todo
    return true;
}
bool Network::inc_money(int value)
{
    if (logged_in_user == NULL)
        throw Permission_Denied_Ex();
    logged_in_user->inc_money(value);
    return true;
}

void Network::get_details(int film_id) //are?
{
    if (logged_in_user == NULL)
        throw Permission_Denied_Ex();
    Film* film = film_repository.find(film_id);
    
    if (film == NULL)
        throw Not_Found_Ex();
    
    cout << "Details of Film " << film->get_name() << endl << "Id = " << film->get_id() << endl
     << "Director = " << film->get_director() << endl << "Length = " << film->get_length() << endl 
     << "Year = " << film->get_year() << endl << "Summary = " << film->get_summary() << endl
     << "Rate = " << film->get_score() << endl << "Price = " << film->get_price() << endl << endl;

    cout << "Comments" << endl << endl << "Recommendation Film" << endl 
        << " #. Film Id | Film Name | Film Length | Film Director" << endl;
    vector<Comment*> comments  = film->get_all_comments();
    for (int i = 0; i < comments.size(); i++)
    {
        cout << comments[i]->get_id() << ". " << comments[i]->get_content() << endl;
        vector<string>* replies = comments[i]->get_replies();
        for (int j = 0; j < replies->size(); j++)
           cout << comments[i]->get_id() << '.' << i + 1 << ". " << (*replies)[j] << endl;
    }

    cout << "Recommendation Film" << endl
         << "#. Film Id | Film Name | Film Length | Film Director" << endl;
    for (int i = 0; i < comments.size(); i++)
    {
        //todo
        cout << i + 1 << ". 3 | film3 | 120 | director_of_film3" << endl;
    }
}

void Network::buy_film(int film_id)
{
    if (logged_in_user == NULL)
        throw Permission_Denied_Ex();
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    Publisher* publisher = film->get_publisher();
    int price = film->get_price();
    revenue += price;
    float percent = get_percent(film_id);
    publishers_revenue[publisher->get_id()] += price * percent;
    logged_in_user->inc_money(-price);   
    // ((Publisher*)publisher)->add_notification(); //todo
}

float Network::get_percent(int film_id)
{
    //define kon?!
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    Publisher* publisher = film->get_publisher();
    int price = film->get_price();
    float score = film->get_score();
    if (score < 5)
        return 0.8;
    else if (score < 8)
        return 0.9;
    else 
        return 0.95; 
}

void Network::rate_film(int film_id, int score)
{
    if (logged_in_user == NULL)
        throw Permission_Denied_Ex();
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    if (score > 10 || score < 1)  //are?
        throw Bad_Request_Ex();

    film->set_score(score);
    // ((Publisher*)publisher)->add_notification(); //todo
}

void Network::add_comment(int film_id, string content)
{
    if (logged_in_user == NULL)
        throw Permission_Denied_Ex();
    Film* film = logged_in_user->find_in_purchased_films(film_id);
    if (film == NULL) //are?
        throw Permission_Denied_Ex();
    film->add_comment(content, logged_in_user);
    // ((Publisher*)publisher)->add_notification(); //todo
}

void Network::find_films(map<string, string> filters)
{
    if (logged_in_user == NULL)
        throw Permission_Denied_Ex();

    vector<Film*> result = film_repository.find(filters);
    cout << "#. Film Id | Film Name | Film Length | Film price | Rate | Production Year | Film Director" << endl;
    for (int i = 0; i < result.size(); i++)
    {
        cout << i + 1 << ". " << result[i]->get_id() << " | " << result[i]->get_name()
            << " | " << result[i]->get_length() << " | " << result[i]->get_price()
            << " | " << result[i]->get_score() << " | " << result[i]->get_year() 
            << " | " << result[i]->get_director() << endl;
    }
}

void Network::get_purchased_films(map<string, string> filters)
{
    if (logged_in_user == NULL)
        throw Permission_Denied_Ex();

    vector<Film*> result = logged_in_user->get_purchased_films(filters);
    cout << "#. Film Id | Film Name | Film Length | Film price | Rate | Production Year | Film Director" << endl;
    for (int i = 0; i < result.size(); i++)
    {
        cout << i + 1 << ". " << result[i]->get_id() << " | " << result[i]->get_name()
            << " | " << result[i]->get_length() << " | " << result[i]->get_price()
            << " | " << result[i]->get_score() << " | " << result[i]->get_year() 
            << " | " << result[i]->get_director() << endl;
    }
}

void Network::get_published_films(map<string, string> filters)
{
    check_user_access();
    vector<Film*> result = logged_in_user->get_purchased_films(filters);
    cout << "#. Film Id | Film Name | Film Length | Film price | Rate | Production Year | Film Director" << endl;
    for (int i = 0; i < result.size(); i++)
    {
        cout << i + 1 << ". " << result[i]->get_id() << " | " << result[i]->get_name()
            << " | " << result[i]->get_length() << " | " << result[i]->get_price()
            << " | " << result[i]->get_score() << " | " << result[i]->get_year() 
            << " | " << result[i]->get_director() << endl;
    }
}