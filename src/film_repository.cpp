#include "film_repository.h"
#include "customer.h"
#include <algorithm>

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
    vector<Film*> result;
    for (map<string, string>::iterator it = filters.begin(); it != filters.end(); it++)
    {
        if (it->first == "name")
        {
            for (int i = 0; i < films.size(); i++)
               if (films[i]->is_published() && films[i]->get_name() == it->second)
                    result.push_back(films[i]);
        }
        else if (it->first == "min_rate")
        {
            for (int i = 0; i < films.size(); i++)
               if (films[i]->is_published() && films[i]->get_score() >= stoi(it->second))
                result.push_back(films[i]);
        }
        else if (it->first == "min_year")
        {
            for (int i = 0; i < films.size(); i++)
               if (films[i]->is_published() && films[i]->get_year() >= stoi(it->second))
                result.push_back(films[i]);
        }
        else if (it->first == "max_year")
        {
            for (int i = 0; i < films.size(); i++)
               if (films[i]->is_published() && films[i]->get_year() <= stoi(it->second))
                {
                    std::vector<Film*>::iterator it = std::find(result.begin(), result.end(), films[i]);
                    if (it == result.end())
                        result.push_back(films[i]);
                }
        }
        else if (it->first == "price")
        {
            for (int i = 0; i < films.size(); i++)
               if (films[i]->is_published() && films[i]->get_price() == stoi(it->second))
                result.push_back(films[i]);
        }
        else if (it->first == "director")
        {
            for (int i = 0; i < films.size(); i++)
               if (films[i]->is_published() && films[i]->get_director() == it->second)
                    result.push_back(films[i]);
        }
    }
    if (filters.size() == 0)
    {
        for (int i = 0; i < films.size(); i++)
            if (films[i]->is_published())
                result.push_back(films[i]);
    }
    return result;
}

void FilmRepository::remove(int film_id)
{
    Film* film = find(film_id);
    film->unpublish();
}

bool cmp(Film* film1, Film* film2)
{
    return film1->get_score() > film2->get_score();
}

std::vector<Film*> FilmRepository::get_recommendation(Film* film, Customer* user)
{
    std::vector<Film*> sorted = films;
    std::vector<Film*> result;
    int count = 4;
    std::sort(sorted.begin(), sorted.end(), cmp);
    for (int i = 0; i < sorted.size() && count > 0; i++)
    {
        if (user->find_in_purchased_films(sorted[i]->get_id()) == NULL && sorted[i]->is_published() && film != sorted[i])
        {
            result.push_back(sorted[i]); 
            count--;  
        }
    }
    return result;
}