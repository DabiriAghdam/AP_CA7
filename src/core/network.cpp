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

bool has_key(map<int, Customer*> m, int key)
{
    return !(m.find(key) == m.end());
}

Network::Network() : http_server(PORT)
{ 
    revenue = 0;
    int age = -1;
    signup(-1, ADMIN_EMAIL, ADMIN_USER, ADMIN_PASS, ADMIN_PASS, age, false);
}

void Network::initialize_handlers()
{
    http_server.setNotFoundErrPage("src/static/404.html");
    http_server.post("/signup", new SignUpHandler(this));
    http_server.get("/signup", new GetSignup(this));
    http_server.get("/", new GetLogin(this));
    http_server.post("/login", new LoginHandler(this));
    http_server.get("/login", new GetLogin(this));
    http_server.get("/logout", new LogoutHandler(this));
    http_server.get("/addfilm", new GetAddFilm(this));
    http_server.post("/addfilm", new AddFilm(this));
    http_server.get("/deletefilm", new DeleteFilm(this));
    http_server.get("/buyfilm", new BuyFilm(this));
    http_server.get("/ratefilm", new RateFilm(this));
    http_server.post("/addmoney", new AddMoney(this));
    http_server.post("/addcomment", new AddComment(this));
    http_server.get("/home", new GetHome(this, "src/template/home.html"));
    http_server.get("/profile", new GetProfile(this, "src/template/profile.html"));
    http_server.get("/filmdetails", new GetFilm(this, "src/template/filmdetails.html"));
}

void Network::start()
{
    try 
    {
        initialize_handlers();
        http_server.run();
    } 
    catch (Server::Exception e) 
    {
        cerr << e.getMessage() << endl;
    }
}

bool Network::logged_in(int user_id)
{
    if (has_key(logged_in_users, user_id))
        return true;
    return false;
}

void Network::logout(int user_id)
{
    check_logged_in(user_id);
    logged_in_users.erase(user_id);
}

int Network::signup(int user_id, string email, string username, string password, string password_repeat, int age, bool publisher)
{
    if (logged_in(user_id))
        throw Bad_Request_Ex();
    Customer* exist = user_repository.find(username);
    if (exist)
        throw Server_Ex("Username already exist");
    else if (!is_valid_email(email))
        throw Server_Ex("Invalid Email address");
    else if (password != password_repeat)
        throw Server_Ex("Passwords don't match");
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
    logged_in_users.insert(pair<int, Customer*> (id, user_repository.find(id)));
    return id;
}

int Network::login(int user_id, string username, string password)
{
    if (logged_in(user_id))
        throw Bad_Request_Ex();
    Customer* user = user_repository.find(username);
    string hashed_pass = md5(password);
    if (user == NULL || user->get_password() != hashed_pass)
        throw Server_Ex("Incorrect Username/Password");
    logged_in_users[user->get_id()] = user;
    return user->get_id();
}

void Network::add_film(int user_id, int year, int length, int price, string name, string summary, string director)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    check_user_access(user_id);
    int id = film_repository.get_films_count() + 1;
    Film* new_film = new Film(id, year, length, price, name, summary, director, (Publisher*)logged_in_users.at(user_id));
    film_repository.add(new_film);
    ((Publisher*)logged_in_users.at(user_id))->add_film(new_film);
    ((Publisher*)logged_in_users.at(user_id))->notify_followers_new_film();
}

void Network::delete_film(int user_id, int film_id)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    check_user_access(user_id);
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    else if (film->get_publisher() != (Publisher*)logged_in_users.at(user_id))
        throw Permission_Denied_Ex();
    film_repository.remove(film_id);
}

void Network::edit_film(int user_id, int film_id, map<string, string> parameters)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    check_user_access(user_id);
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    else if (film->get_publisher() != (Publisher*)logged_in_users.at(user_id))
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

void Network::get_followers(int user_id)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    check_user_access(user_id);
    vector<Customer*> followers = ((Publisher*)logged_in_users.at(user_id))->get_followers();
    cout << "List of Followers" << endl << "#. User Id | User Username | User Email" << endl;
    for (int i = 0; i < followers.size(); i++)
    {
        cout << i + 1 << ". " << followers[i]->get_id() << " | " << followers[i]->get_username()
            << " | " << followers[i]->get_email() << endl;
    }
}

void Network::give_money(int user_id)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    check_user_access(user_id);
    int money = publishers_revenue[logged_in_users.at(user_id)->get_id()];
    logged_in_users.at(user_id)->inc_money(money);
    publishers_revenue[logged_in_users.at(user_id)->get_id()] = 0;
    revenue -= money;
}

bool Network::is_admin(int user_id)
{
    if (has_key(logged_in_users, user_id))
        return (logged_in_users.at(user_id)->get_username() == ADMIN_USER);
    else
        return false;
}

void Network::get_money(int user_id)
{
    check_logged_in(user_id);
    if (is_admin(user_id))
        cout << revenue << endl;
    else
        cout << logged_in_users.at(user_id)->get_money() << endl;
}

void Network::check_logged_in(int user_id)
{
    if (!has_key(logged_in_users, user_id))
        throw Permission_Denied_Ex();
}

void Network::check_user_access(int user_id)
{
    if (logged_in_users.at(user_id)->get_type() == CUSTOMER)
        throw Permission_Denied_Ex();   
}

void Network::reply(int user_id, int film_id, int comment_id, string content)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    check_user_access(user_id);
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    if (film->get_publisher() != logged_in_users.at(user_id))
        throw Permission_Denied_Ex();
    Comment* comment = film->get_comment(comment_id);
    if (comment == NULL)
        throw Not_Found_Ex();
    comment->add_reply(content);
    Customer* author = comment->get_author();
    stringstream msg;
    msg << "Publisher " << logged_in_users.at(user_id)->get_username() << " with id " 
        << logged_in_users.at(user_id)->get_id() << " reply to your comment.";
    author->add_notification(msg.str());
}

void Network::delete_comment(int user_id, int film_id, int comment_id)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    check_user_access(user_id);
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    film->delete_comment(comment_id);
}

void Network::follow(int user_id, int publisher_id)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    Customer* publisher = user_repository.find(publisher_id);
    if (publisher == NULL)
        throw Not_Found_Ex();
    if (publisher->get_type() == CUSTOMER)
        throw Bad_Request_Ex();
    ((Publisher*)publisher)->add_follower(logged_in_users.at(user_id));
    stringstream msg;
    msg << "User " << logged_in_users.at(user_id)->get_username() << " with id " 
        << logged_in_users.at(user_id)->get_id() << " follow you.";
    publisher->add_notification(msg.str());
}

void Network::inc_money(int user_id, int value)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    logged_in_users.at(user_id)->inc_money(value);
}

void Network::print_film_details(Film* film)
{
    cout << "Details of Film " << film->get_name() << "\nId = " << film->get_id()
        << "\nDirector = " << film->get_director() << "\nLength = " << film->get_length() 
        << "\nYear = " << film->get_year() << "\nSummary = " << film->get_summary()
        << "\nRate = "<< setprecision(2) << film->get_score() << "\nPrice = " << film->get_price() << endl;
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

void Network::print_recommended_films(int user_id, Film* film)
{;
    vector<Film*> recommended = film_repository.get_recommendations(film, logged_in_users.at(user_id));
    cout << endl << "Recommendation Film" << endl
         << "#. Film Id | Film Name | Film Length | Film Director" << endl;
    for (int i = 0; i < recommended.size(); i++)
    {
        cout << i + 1 << ". " << recommended[i]->get_id() << " | " << recommended[i]->get_name()
            << " | " << recommended[i]->get_length() << " | " << recommended[i]->get_director() << endl;
    }
}

map<string, string> Network::get_details(int user_id, int film_id)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    Film* film = film_repository.find(film_id);
    if (film == NULL || !film->is_published())
        throw Not_Found_Ex();
    map<string, string> context;
    context["name"] = film->get_name();
    context["director"] = film->get_director();
    context["summary"] = film->get_summary();
    context["length"] = to_string(film->get_length());
    context["year"] =to_string(film->get_year());
    context["price"] = to_string(film->get_price());
    context["rate"] = to_string(film->get_score());
    context["id"] = to_string(film->get_id());
    context["purchased"] = "no";
    if (logged_in_users.at(user_id)->find_in_purchased_films(film_id) != NULL)
        context["purchased"] = "yes";
    vector<Film*> recommended = film_repository.get_recommendations(film, logged_in_users.at(user_id));
    context["recommended_count"] = to_string(recommended.size());
    for (int i = 0; i < recommended.size(); i++)
    {
        context["name" + to_string(i)] = recommended[i]->get_name();
        context["director" + to_string(i)] = recommended[i]->get_director();
        context["length" + to_string(i)] = to_string(recommended[i]->get_length());
        context["id" + to_string(i)] = to_string(recommended[i]->get_id());
    }
    vector<Comment*> comments  = film->get_all_comments();
    context["comments_count"] = to_string(comments.size());
    for (int i = 0; i < comments.size(); i++)
        context["comment" + to_string(i)] = comments[i]->get_content();

    return context;
}

void Network::buy_film(int user_id, int film_id)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    Film* film = film_repository.find(film_id);
    if (film == NULL)
        throw Not_Found_Ex();
    if (logged_in_users.at(user_id)->find_in_purchased_films(film_id) == NULL)
    {
        Publisher* publisher = film->get_publisher();
        int price = film->get_price();
        if (logged_in_users.at(user_id)->get_money() < price)
            throw Permission_Denied_Ex();
        revenue += price;
        logged_in_users.at(user_id)->inc_money(-price); 
        float percent = get_percent(film_id);
        publishers_revenue[publisher->get_id()] += price * percent;
        logged_in_users.at(user_id)->purchase_film(film);  
        stringstream msg;
        msg << "User " << logged_in_users.at(user_id)->get_username() << " with id " 
            << logged_in_users.at(user_id)->get_id() << " buy your film " << film->get_name()
            << " with id " << film->get_id() << '.';
        publisher->add_notification(msg.str());
        film_repository.update_adjacency_matrix(logged_in_users.at(user_id), film);
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

void Network::rate_film(int user_id, int film_id, int score)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    Film* film = logged_in_users.at(user_id)->find_in_purchased_films(film_id);
    if (film == NULL)
        throw Permission_Denied_Ex();
    if (score > 10 || score < 1)  
        throw Bad_Request_Ex();
    film->set_score(logged_in_users.at(user_id)->get_id(), score);
    stringstream msg;
    msg << "User " << logged_in_users.at(user_id)->get_username() << " with id " 
        << logged_in_users.at(user_id)->get_id() << " rate your film " << film->get_name()
        << " with id " << film->get_id() << '.';
    film->get_publisher()->add_notification(msg.str());
}

void Network::add_comment(int user_id, int film_id, string content)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    Film* film = logged_in_users.at(user_id)->find_in_purchased_films(film_id);
    if (film == NULL)
        throw Permission_Denied_Ex();
    film->add_comment(content, logged_in_users.at(user_id));
    stringstream msg;
    msg << "User " << logged_in_users.at(user_id)->get_username() << " with id " 
        << logged_in_users.at(user_id)->get_id() << " comment on your film " << film->get_name()
        << " with id " << film->get_id() << '.';
    film->get_publisher()->add_notification(msg.str()); 
}

void Network::find_films(int user_id, map<string, string> filters)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
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

map<string, string> Network::get_profile_films(int user_id)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    map<string, string> context;
    map<string, string> filters;
    context = get_purchased_films(user_id, filters);
     if (logged_in_users.at(user_id)->get_type() == PUBLISHER)
        context["publisher"] = "yes";
    else
        context["publisher"] = "no";
    return context;
}


map<string, string> Network::get_home_films(int user_id, string director)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    map<string, string> filters;
    map<string, string> context;
	filters["price"] = to_string(logged_in_users.at(user_id)->get_money());
    vector<Film*> results = film_repository.find(filters);
    vector<Film*> not_purchased_results;
    for (int i = 0; i < results.size(); i++)
    {
        if (logged_in_users.at(user_id)->find_in_purchased_films(results[i]->get_id()) == NULL)
            not_purchased_results.push_back(results[i]);
    }
    context["count"] = to_string(not_purchased_results.size());
    for (int i = 0; i < not_purchased_results.size(); i++)
    {
        context["name" + to_string(i)] = not_purchased_results[i]->get_name();
        context["director" + to_string(i)] = not_purchased_results[i]->get_director();
        context["length" + to_string(i)] = to_string(not_purchased_results[i]->get_length());
        context["year" + to_string(i)] = to_string(not_purchased_results[i]->get_year());
        context["rate" + to_string(i)] = to_string(not_purchased_results[i]->get_score());
        context["price" + to_string(i)] = to_string(not_purchased_results[i]->get_price());
        context["id" + to_string(i)] = to_string(not_purchased_results[i]->get_id());
    }
     if (logged_in_users.at(user_id)->get_type() == PUBLISHER)
     {
        context["publisher"] = "yes";
        context["director_name"] = "all directors";
        map<string, string> filter;
        if (director != "")
        {
            filter["director"] = director;
            context["director_name"] = "director " + director;
        }
        results = ((Publisher*)logged_in_users.at(user_id))->get_published_films(filter);
        context["published_count"] = to_string(results.size());
        for (int i = 0; i < results.size(); i++)
        {
            context["published_name" + to_string(i)] = results[i]->get_name();
            context["published_director" + to_string(i)] = results[i]->get_director();
            context["published_length" + to_string(i)] = to_string(results[i]->get_length());
            context["published_year" + to_string(i)] = to_string(results[i]->get_year());
            context["published_rate" + to_string(i)] = to_string(results[i]->get_score());
            context["published_price" + to_string(i)] = to_string(results[i]->get_price());
            context["published_id" + to_string(i)] = to_string(results[i]->get_id());
        }
     }
    else
        context["publisher"] = "no";
return context;
}

 map<string, string> Network::get_purchased_films(int user_id, map<string, string> filters)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    map<string, string> context;
    vector<Film*> result = logged_in_users.at(user_id)->get_purchased_films(filters);
    context["money"] = to_string(logged_in_users.at(user_id)->get_money());
    context["count"] = to_string(result.size());
    for (int i = 0; i < result.size(); i++)
    {
        context["name" + to_string(i)] = result[i]->get_name();
        context["director" + to_string(i)] = result[i]->get_director();
        context["length" + to_string(i)] = to_string(result[i]->get_length());
        context["year" + to_string(i)] = to_string(result[i]->get_year());
        context["rate" + to_string(i)] = to_string(result[i]->get_score());
        context["price" + to_string(i)] = to_string(result[i]->get_price());
        context["id" + to_string(i)] = to_string(result[i]->get_id());
    }
    return context;
}

map<string, string>  Network::get_published_films(int user_id, map<string, string> filters)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    check_user_access(user_id);
    map<string, string> context;
    vector<Film*> result = ((Publisher*)logged_in_users.at(user_id))->get_published_films(filters);
    context["count"] = to_string(result.size());
    for (int i = 0; i < result.size(); i++)
    {
        context["name" + to_string(i)] = result[i]->get_name();
        context["director" + to_string(i)] = result[i]->get_director();
        context["length" + to_string(i)] = to_string(result[i]->get_length());
        context["year" + to_string(i)] = to_string(result[i]->get_year());
        context["rate" + to_string(i)] = to_string(result[i]->get_score());
        context["price" + to_string(i)] = to_string(result[i]->get_price());
        context["id" + to_string(i)] = to_string(result[i]->get_id());
    }
    return context;
}

void Network::get_unread_notifications(int user_id)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    vector<Notification*> unreads = logged_in_users.at(user_id)->get_unread_notifications();
    cout << "#. Notification Message" << endl;
    for (int i = unreads.size() - 1; i >= 0; i--)
    {
        cout << unreads.size() - i << ". " << unreads[i]->get_message() << endl;
        unreads[i]->is_read(true);
    }
}

void Network::get_all_notifications(int user_id, int limit)
{
    if (is_admin(user_id))
        throw Bad_Request_Ex();
    check_logged_in(user_id);
    vector<Notification*> notifs = logged_in_users.at(user_id)->get_all_notifications();
    cout << "#. Notification Message" << endl;
    for (int i = notifs.size() - 1; i >= 0 && limit > 0; i--)
    {
        cout << notifs.size() - i << ". " << notifs[i]->get_message() << endl;
        notifs[i]->is_read(true);
        limit--;
    }   
}