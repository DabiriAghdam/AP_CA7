#include "customer.h"
#include <algorithm>

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

void Customer::inc_money(int value) 
{  
    money += value; 
}

int Customer::get_id() 
{  
    return id;  
}

int Customer::get_money() 
{   
    return  money;  
}

string Customer::get_username() 
{    
    return username;    
}

string Customer::get_password() 
{    
    return password;   
}

string Customer::get_email() 
{   
    return email;   
}

string Customer::get_type() 
{    
    return CUSTOMER;  
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

vector<Film*> Customer::get_purchased_films(map<string, string> filters)
{
    vector<Film*> result;
    for (map<string, string>::iterator it = filters.begin(); it != filters.end(); it++)
    {
        if (it->first == "name")
        {
            for (int i = 0; i < purchased_films.size(); i++)
               if (purchased_films[i]->is_published() && purchased_films[i]->get_name() == it->second)
                    result.push_back(purchased_films[i]);
        }
        else if (it->first == "min_rate")
        {
            for (int i = 0; i < purchased_films.size(); i++)
               if (purchased_films[i]->is_published() && purchased_films[i]->get_score() >= stoi(it->second))
                    result.push_back(purchased_films[i]);
        }
        else if (it->first == "min_year")
        {
            for (int i = 0; i < purchased_films.size(); i++)
               if (purchased_films[i]->is_published() && purchased_films[i]->get_year() >= stoi(it->second))
                    result.push_back(purchased_films[i]);
        }
        else if (it->first == "max_year")
        {
            for (int i = 0; i < purchased_films.size(); i++)
               if (purchased_films[i]->is_published() && purchased_films[i]->get_year() <= stoi(it->second))
               {
                    vector<Film*>::iterator it = find(result.begin(), result.end(), purchased_films[i]);
                    if (it == result.end())
                        result.push_back(purchased_films[i]);
                }
            for (int i = 0; i < result.size(); i++)
                if (result[i]->get_year() > stoi(it->second))
                    result.erase(result.begin() + i);
        }
        else if (it->first == "price")
        {
            for (int i = 0; i < purchased_films.size(); i++)
               if (purchased_films[i]->is_published() && purchased_films[i]->get_price() == stoi(it->second))
                    result.push_back(purchased_films[i]);
        }
        else if (it->first == "director")
        {
            for (int i = 0; i < purchased_films.size(); i++)
               if (purchased_films[i]->is_published() && purchased_films[i]->get_director() == it->second)
                    result.push_back(purchased_films[i]);
        }
        else
            throw Bad_Request_Ex();
        
    }
    if (filters.size() == 0)
    {
        for (int i = 0; i < purchased_films.size(); i++)
            if (purchased_films[i]->is_published())
                result.push_back(purchased_films[i]);
    }
    return result;
}

void Customer::purchase_film(Film* film)
{
    purchased_films.push_back(film);
}

vector<Notification*> Customer::get_unread_notifications()
{
    vector<Notification*> result;
    for (int i = 0; i < notifications.size(); i++)
        if (!notifications[i]->get_read())
            result.push_back(notifications[i]);
    return result;
}

vector<Notification*> Customer::get_all_notifications()
{
    vector<Notification*> result;
    for (int i = 0; i < notifications.size(); i++)
        if (!notifications[i]->get_read())
            result.push_back(notifications[i]);
    return result;
}

void Customer::add_notification(string message)
{
    Notification* notification = new Notification(message);
    notifications.push_back(notification);
}