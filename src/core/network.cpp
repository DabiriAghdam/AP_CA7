#include "network.h"
#include "handlers.h"
#include "md5.h"
#include <iostream>
#include <iomanip> 
#include <regex>

using namespace std;

bool is_valid_email(const string& email)
{
   const regex pattern ("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
   return regex_match(email, pattern);
}
Network::Network() : http_server(PORT)// commands_handler(this) 
{ 
    logged_in_user = NULL;
    revenue = 0;
    int age = -1;
    signup(ADMIN_EMAIL, ADMIN_USER, ADMIN_PASS, age, false);
    logged_in_user = NULL;
}

void Network::initialize_handlers()
{
    http_server.get("/", new HelloWorld());
    http_server.get("/add", new Adder());
    http_server.get("/addform", new ShowPage("src/static/addform.html"));
}

void Network::start()
{
    // commands_handler.run();
    try 
    {
        http_server.run();
    } 
    catch (Server::Exception e) 
    {
    cerr << e.getMessage() << endl;
    }
}

bool Network::logged_in()
{
    return (logged_in_user != NULL);
}

void Network::logout()
{
    check_logged_in();
    logged_in_user = NULL;
}

void Network::signup(string email, string username, string password, int age, bool publisher)
{
    Customer* exist = user_repository.find(username);
    if (logged_in() || exist || !is_valid_email(email))
        throw Bad_Request_Ex();
    
    string hashed_pass = md5(password);
    int id = user_repository.get_users_count() + 1;
    int money = 0;
    if (publisher)
    {
        Publisher* new_user = new Publisher(id, age, money, email, username, hashed_pass);
        user_repository.add(new_user);
        publishers_revenue[id] = money;
    }
    else 
    {
        Customer* new_user = new Customer(id, age, money, email, username, hashed_pass);
        user_repository.add(new_user);
    }
    logged_in_user = user_repository.find(id);
}

void Network::login(string username, string password)
{
    Customer* user = user_repository.find(username);
    string hashed_pass = md5(password);
    if (logged_in() || user == NULL || user->get_password() != hashed_pass)
        throw Bad_Request_Ex();
    else 
        logged_in_user = user;
}

void Network::add_film(int year, int length, int price, string name, string summary, string director)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    check_user_access();
    int id = film_repository.get_films_count() + 1;
    Film* new_film = new Film(id, year, length, price, name, summary, director, (Publisher*)logged_in_user);
    film_repository.add(new_film);
    ((Publisher*)logged_in_user)->add_film(new_film);
    ((Publisher*)logged_in_user)->notify_followers_new_film();
}

void Network::delete_film(int film_id)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    check_user_access();
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    else if (film->get_publisher() != (Publisher*)logged_in_user)
        throw Permission_Denied_Ex();
    film_repository.remove(film_id);
}

void Network::edit_film(int film_id, map<string, string> parameters)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    check_user_access();
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    else if (film->get_publisher() != (Publisher*)logged_in_user)
        throw Permission_Denied_Ex();
    for (map<string, string>::iterator it = parameters.begin(); it != parameters.end(); it++)
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
        else if (it->first != "film_id")
            throw Bad_Request_Ex();
    }
}

void Network::get_followers()
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    check_user_access();
    vector<Customer*> followers = ((Publisher*)logged_in_user)->get_followers();
    cout << "List of Followers" << endl << "#. User Id | User Username | User Email" << endl;
    for (int i = 0; i < followers.size(); i++)
    {
        cout << i + 1 << ". " << followers[i]->get_id() << " | " << followers[i]->get_username()
            << " | " << followers[i]->get_email() << endl;
    }
}

void Network::give_money()
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    check_user_access();
    int money = publishers_revenue[logged_in_user->get_id()];
    logged_in_user->inc_money(money);
    publishers_revenue[logged_in_user->get_id()] = 0;
    revenue -= money;
}

bool Network::is_admin()
{
    if (logged_in_user != NULL)
        return (logged_in_user->get_username() == ADMIN_USER);
    else
        return false;
}

void Network::get_money()
{
    check_logged_in();
    if (is_admin())
        cout << revenue << endl;
    else
        cout << logged_in_user->get_money() << endl;
}

void Network::check_logged_in()
{
    if (logged_in_user == NULL)
        throw Permission_Denied_Ex();
}

void Network::check_user_access()
{
    if (logged_in_user->get_type() == CUSTOMER)
        throw Permission_Denied_Ex();   
}

void Network::reply(int film_id, int comment_id, string content)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    check_user_access();
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    if (film->get_publisher() != logged_in_user)
        throw Permission_Denied_Ex();
    Comment* comment = film->get_comment(comment_id);
    if (comment == NULL)
        throw Not_Found_Ex();
    comment->add_reply(content);
    Customer* author = comment->get_author();
    stringstream msg;
    msg << "Publisher " << logged_in_user->get_username() << " with id " 
        << logged_in_user->get_id() << " reply to your comment.";
    author->add_notification(msg.str());
}

void Network::delete_comment(int film_id, int comment_id)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    check_user_access();
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    film->delete_comment(comment_id);
}

void Network::follow(int publisher_id)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    Customer* publisher = user_repository.find(publisher_id);
    if (publisher == NULL)
        throw Not_Found_Ex();
    if (publisher->get_type() == CUSTOMER)
        throw Bad_Request_Ex();
    ((Publisher*)publisher)->add_follower(logged_in_user);
    stringstream msg;
    msg << "User " << logged_in_user->get_username() << " with id " 
        << logged_in_user->get_id() << " follow you.";
    publisher->add_notification(msg.str());
}

void Network::inc_money(int value)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    logged_in_user->inc_money(value);
}

void Network::print_film_details(Film* film)
{
    cout << "Details of Film " << film->get_name() << endl << "Id = " << film->get_id() << endl
    << "Director = " << film->get_director() << endl << "Length = " << film->get_length() << endl 
    << "Year = " << film->get_year() << endl << "Summary = " << film->get_summary() << endl
    << "Rate = "<< setprecision(2) << film->get_score() << endl << "Price = " << film->get_price() << endl;
}

void Network::print_film_comments(Film* film)
{
    cout << endl << "Comments" << endl;
    vector<Comment*> comments  = film->get_all_comments();
    for (int i = 0; i < comments.size(); i++)
    {
        cout << comments[i]->get_id() << ". " << comments[i]->get_content() << endl;
        vector<string>* replies = comments[i]->get_replies();
        for (int j = 0; j < replies->size(); j++)
           cout << comments[i]->get_id() << '.' << i + 1 << ". " << (*replies)[j] << endl;
    }
}

void Network::print_recommended_films(Film* film)
{
    vector<Film*> recommended = film_repository.get_recommendations(film, logged_in_user);
    cout << endl << "Recommendation Film" << endl
         << "#. Film Id | Film Name | Film Length | Film Director" << endl;
    for (int i = 0; i < recommended.size(); i++)
    {
        cout << i + 1 << ". " << recommended[i]->get_id() << " | " << recommended[i]->get_name()
            << " | " << recommended[i]->get_length() << " | " << recommended[i]->get_director() << endl;
    }
}

void Network::get_details(int film_id)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    Film* film = film_repository.find(film_id);
    if (film == NULL || !film->is_published())
        throw Not_Found_Ex();
    print_film_details(film);
    print_film_comments(film);
    print_recommended_films(film);
}

void Network::buy_film(int film_id)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    if (logged_in_user->find_in_purchased_films(film_id) == NULL)
    {
        Publisher* publisher = film->get_publisher();
        int price = film->get_price();
        if (logged_in_user->get_money() < price)
            throw Permission_Denied_Ex();
        revenue += price;
        logged_in_user->inc_money(-price); 
        float percent = get_percent(film_id);
        publishers_revenue[publisher->get_id()] += price * percent;
        logged_in_user->purchase_film(film);  
        stringstream msg;
        msg << "User " << logged_in_user->get_username() << " with id " 
            << logged_in_user->get_id() << " buy your film " << film->get_name()
            << " with id " << film->get_id() << '.';
        publisher->add_notification(msg.str());
        film_repository.update_adjacency_matrix(logged_in_user, film);
    }
}

float Network::get_percent(int film_id)
{
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    float score = film->get_score();
    if (score < LOW)
        return WEAK;
    else if (score < MID)
        return AVERAGE;
    else
        return STRONG; 
}

void Network::rate_film(int film_id, int score)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    Film* film = logged_in_user->find_in_purchased_films(film_id);
    if (film == NULL)
        throw Permission_Denied_Ex();
    if (score > 10 || score < 1)  
        throw Bad_Request_Ex();
    film->set_score(logged_in_user->get_id(), score);
    stringstream msg;
    msg << "User " << logged_in_user->get_username() << " with id " 
        << logged_in_user->get_id() << " rate your film " << film->get_name()
        << " with id " << film->get_id() << '.';
    film->get_publisher()->add_notification(msg.str());
}

void Network::add_comment(int film_id, string content)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    Film* film = logged_in_user->find_in_purchased_films(film_id);
    if (film == NULL)
        throw Permission_Denied_Ex();
    film->add_comment(content, logged_in_user);
    stringstream msg;
    msg << "User " << logged_in_user->get_username() << " with id " 
        << logged_in_user->get_id() << " comment on your film " << film->get_name()
        << " with id " << film->get_id() << '.';
    film->get_publisher()->add_notification(msg.str()); 
}

void Network::find_films(map<string, string> filters)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    vector<Film*> result = film_repository.find(filters);
    cout << "#. Film Id | Film Name | Film Length | Film price | Rate | Production Year | Film Director" << endl;
    for (int i = 0; i < result.size(); i++)
    {
        cout << i + 1 << ". " << result[i]->get_id() << " | " << result[i]->get_name()
            << " | " << result[i]->get_length() << " | " << result[i]->get_price()
            << " | "<< setprecision(2) << result[i]->get_score() << " | " << result[i]->get_year() 
            << " | " << result[i]->get_director() << endl;
    }
}

void Network::get_purchased_films(map<string, string> filters)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    vector<Film*> result = logged_in_user->get_purchased_films(filters);
    cout << "#. Film Id | Film Name | Film Length | Film price | Rate | Production Year | Film Director" << endl;
    for (int i = 0; i < result.size(); i++)
    {
        cout << i + 1 << ". " << result[i]->get_id() << " | " << result[i]->get_name()
            << " | " << result[i]->get_length() << " | " << result[i]->get_price()
            << " | "<< setprecision(2) << result[i]->get_score() << " | " << result[i]->get_year() 
            << " | " << result[i]->get_director() << endl;
    }
}

void Network::get_published_films(map<string, string> filters)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    check_user_access();
    vector<Film*> result = ((Publisher*)logged_in_user)->get_published_films(filters);
    cout << "#. Film Id | Film Name | Film Length | Film price | Rate | Production Year | Film Director" << endl;
    for (int i = 0; i < result.size(); i++)
    {
        cout << i + 1 << ". " << result[i]->get_id() << " | " << result[i]->get_name()
            << " | " << result[i]->get_length() << " | " << result[i]->get_price()
            << " | " << setprecision(2) << result[i]->get_score() << " | " << result[i]->get_year() 
            << " | " << result[i]->get_director() << endl;
    }
}

void Network::get_unread_notifications()
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    vector<Notification*> unreads = logged_in_user->get_unread_notifications();
    cout << "#. Notification Message" << endl;
    for (int i = unreads.size() - 1; i >= 0; i--)
    {
        cout << unreads.size() - i << ". " << unreads[i]->get_message() << endl;
        unreads[i]->is_read(true);
    }
}

void Network::get_all_notifications(int limit)
{
    if (is_admin())
        throw Bad_Request_Ex();
    check_logged_in();
    vector<Notification*> notifs = logged_in_user->get_all_notifications();
    cout << "#. Notification Message" << endl;
    for (int i = notifs.size() - 1; i >= 0 && limit > 0; i--)
    {
        cout << notifs.size() - i << ". " << notifs[i]->get_message() << endl;
        notifs[i]->is_read(true);
        limit--;
    }   
}