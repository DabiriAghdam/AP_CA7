#include "film_repository.h"
#include "customer.h"
#include <algorithm>

using namespace std;

FilmRepository::~FilmRepository()
{
    for (int i = 0; i < films.size(); i++)
        delete films[i];    
}

void FilmRepository::add(Film* new_film)
{
    films.push_back(new_film);
    films_adjacency_matrix.resize(get_films_count());
    for (int i = 0; i < films_adjacency_matrix.size(); i++)
        films_adjacency_matrix[i].resize(get_films_count());
    id_location[new_film->get_id()] = films_adjacency_matrix.size() - 1;
}

Film* FilmRepository::find(int film_id)
{
    for (int i = 0; i < films.size(); i++)
    {
        if (films[i]->get_id() == film_id)
            return films[i];
    }
    return NULL;
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
                    vector<Film*>::iterator it = std::find(result.begin(), result.end(), films[i]);
                    if (it == result.end())
                        result.push_back(films[i]);
                }
            for (int i = 0; i < result.size(); i++)
                if (result[i]->get_year() > stoi(it->second))
                    result.erase(result.begin() + i);
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
        else
            throw Bad_Request_Ex();
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
    int location = id_location[film_id];
    for (int i = 0; i < films_adjacency_matrix[location].size(); i++)
    {
        films_adjacency_matrix[location][i] = -1;
        films_adjacency_matrix[i][location] = -1;
    }
}

int FilmRepository::get_films_count()   
{   
    return films.size();    
}

void FilmRepository::update_adjacency_matrix(Customer* user, Film* film)
{
    map<string, string> all;
    int new_film_location = id_location[film->get_id()];
    vector<Film*> purchased = user->get_purchased_films(all);
    for (int i = 0; i < purchased.size(); i++)
    {
        if (purchased[i]->is_published() && purchased[i] != film) 
        {
            int location = id_location[purchased[i]->get_id()];
            if (films_adjacency_matrix[location][new_film_location] != -1)
                films_adjacency_matrix[location][new_film_location]++;
            if (films_adjacency_matrix[new_film_location][location] != -1)            
                films_adjacency_matrix[new_film_location][location]++;
        }
    }
}

int FilmRepository::find_largest_index(vector<int> input, vector<int> skips)
{
    bool skip = false;
    int largest_value = -1;
    int largest_index = -1;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] > largest_value)
        {
            for (int j = 0; j < skips.size(); j++)
            {
                if (i == skips[j])
                {
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                largest_value = input[i];
                largest_index = i;
            }
        }
        skip = false;
    }
    return largest_index;
}

vector<Film*> FilmRepository::get_recommendations(Film* film, Customer* user)
{
    vector<Film*> result;
    int location = id_location[film->get_id()];
    vector<int> recommended_films_index(RECOMMENDED_COUNT);
    vector<int> skips;
    skips.push_back(location);

    for (int i = 0; i < RECOMMENDED_COUNT; i++)
    {    
        recommended_films_index[i] = find_largest_index(films_adjacency_matrix[location], skips);
        skips.push_back(recommended_films_index[i]);
    }
    for (int i = 0; i < RECOMMENDED_COUNT; i++)
    {
        for (map<int, int>::iterator it = id_location.begin(); it != id_location.end(); it++)
        {
            if (recommended_films_index[i] == it->second)
            {
                Film* recommended = find(it->first);
                result.push_back(recommended);
                break;
            }
        }
    }
    return result;
}