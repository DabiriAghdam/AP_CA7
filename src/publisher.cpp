#include "publisher.h"
#include <sstream>

using namespace std;

void Publisher::add_follower(Customer* follower)
{
    followers.push_back(follower);
}

void Publisher::add_film(Film* film)
{
    published_films.push_back(film);
}

vector<Film*> Publisher::get_published_films(map<string, string> filters)
{
    vector<Film*> result = published_films;
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
        // else
        //     throw Bad_Request_Ex(); //are?!   
    }
    return result;
}

void Publisher::notify_followers_new_film()
{
    stringstream msg;
    msg << "Publisher " << username << " with id " << id << " register new film.";
    for (int i = 0; i < followers.size(); i++)
        followers[i]->add_notification(msg.str());
}