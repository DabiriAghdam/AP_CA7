#include "film_repository.h"

using namespace std;

void FilmRepository::add(Film* new_film)
{
    films.push_back(new_film);
}
Film* FilmRepository::find(int film_id)
{
    if (film_id - 1 > films.size() || film_id - 1 < 0)
        return NULL;
    else 
        return films[film_id - 1];
}

vector<Film*> FilmRepository::find(map<string, string> filters)
{
    vector<Film*> result = films;
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
        else
            throw Bad_Request_Ex(); //are?!   
    }
    return result;
}


void FilmRepository::remove(int film_id)
{
    Film* film = find(film_id);
    film->unpublish();
}