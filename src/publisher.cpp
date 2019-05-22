#include "publisher.h"
#include <sstream>
#include <iostream>
#include <algorithm>

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
    vector<Film*> result;
    for (map<string, string>::iterator it = filters.begin(); it != filters.end(); it++)
    {
        if (it->first == "name")
        {
            for (int i = 0; i < published_films.size(); i++)
               if (published_films[i]->is_published() && published_films[i]->get_name() == it->second)
                    result.push_back(published_films[i]);
        }
        else if (it->first == "min_rate")
        {
            for (int i = 0; i < published_films.size(); i++)
               if (published_films[i]->is_published() && published_films[i]->get_score() >= stoi(it->second))
                result.push_back(published_films[i]);
        }
        else if (it->first == "min_year")
        {
            for (int i = 0; i < published_films.size(); i++)
               if (published_films[i]->is_published() && published_films[i]->get_year() >= stoi(it->second))
                result.push_back(published_films[i]);
        }
        else if (it->first == "max_year")
        {
            for (int i = 0; i < published_films.size(); i++)
                if (published_films[i]->is_published() && published_films[i]->get_year() <= stoi(it->second))
                    {
                        std::vector<Film*>::iterator it = find(result.begin(), result.end(), published_films[i]);
                        if (it == result.end())
                            result.push_back(published_films[i]);
                    }
        }
        else if (it->first == "price")
        {
            for (int i = 0; i < published_films.size(); i++)
               if (published_films[i]->is_published() && published_films[i]->get_price() == stoi(it->second))
                result.push_back(published_films[i]);
        }
        else if (it->first == "director")
        {
            for (int i = 0; i < published_films.size(); i++)
               if (published_films[i]->is_published() && published_films[i]->get_director() == it->second)
                    result.push_back(published_films[i]);
        }
    }
    if (filters.size() == 0)
    {
        for (int i = 0; i < published_films.size(); i++)
            if (published_films[i]->is_published())
                result.push_back(published_films[i]);
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