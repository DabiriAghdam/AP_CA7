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

vector<Film*> Customer::get_purchased_films(map<string, string> filters)
{
    vector<Film*> result = purchased_films;
    for (map<string, string>::iterator it = filters.begin(); it != filters.end(); it++)
    {
        if (it->first == "name")
        {
            for (int i = 0; i < result.size(); i++)
               if (result[i]->get_name() != it->second)
                    result.erase(result.begin() + i);
        }
        else if (it->first == "min_rate")
        {
            for (int i = 0; i < result.size(); i++)
               if (result[i]->get_score() < stoi(it->second))
                result.erase(result.begin() + i);
        }
        else if (it->first == "min_year")
        {
            for (int i = 0; i < result.size(); i++)
               if (result[i]->get_year() < stoi(it->second))
                result.erase(result.begin() + i);
        }
        else if (it->first == "max_year")
        {
            for (int i = 0; i < result.size(); i++)
               if (result[i]->get_year() > stoi(it->second))
                result.erase(result.begin() + i);
        }
        else if (it->first == "price")
        {
            for (int i = 0; i < result.size(); i++)
               if (result[i]->get_price() > stoi(it->second))
                result.erase(result.begin() + i);
        }
        else if (it->first == "director")
        {
            for (int i = 0; i < result.size(); i++)
               if (result[i]->get_director() != it->second)
                    result.erase(result.begin() + i);
        }
    }
    return result;
}

void Customer::purchase_film(Film* film)
{
    purchased_films.push_back(film);
}
