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
    adjacency_matrix.resize(get_films_count());
    for (int i = 0; i < adjacency_matrix.size(); i++)
        adjacency_matrix[i].resize(get_films_count());
    id_location[new_film->get_id()] = adjacency_matrix.size() - 1;
}

Film* FilmRepository::find(int film_id)
{
    for (int i = 0; i < films.size(); i++)
    {
        if (films[i]->get_id() == film_id)
            return films[i];
    }
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
    //change adjancency matrix ?
    int location = id_location[film_id];
    for (int i = 0; i < adjacency_matrix[location].size(); i++)
       if ( i != location || 1) //are?
       {
            adjacency_matrix[location][i] = -1;
            adjacency_matrix[i][location] = -1;
       }
}

int FilmRepository::get_films_count()   
{   
    return films.size();    
}

// bool cmp(Film* film1, Film* film2)
// {
//     return film1->get_score() > film2->get_score();
// }

void find_largest(vector<int> input, vector<int> skips, int &largest_value, int &largest_index)
{
    bool skip = false;
    largest_value = 0;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] >= largest_value)
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
}

vector<Film*> FilmRepository::get_recommendations(Film* film, Customer* user)
{
    vector<Film*> result;
    int count = RECOMMENDED_COUNT;
    int location = id_location[film->get_id()];
    // vector<int> test;
    // test.push_back(1);
    // test.push_back(5);
    // test.push_back(0);
    // test.push_back(4);
    // test.push_back(3);
    // adjacency_matrix[location] = test;

    int first, first_index = -1;
    int second, second_index = -1;
    int third, third_index = -1;
    int fourth, fourth_index = -1; 
    vector<int> skips;
    skips.push_back(location);
    find_largest(adjacency_matrix[location], skips, first, first_index);
    skips.push_back(first_index);
    find_largest(adjacency_matrix[location], skips, second, second_index);
    skips.push_back(second_index);
    find_largest(adjacency_matrix[location], skips, third, third_index);
    skips.push_back(third_index);
    find_largest(adjacency_matrix[location], skips, fourth, fourth_index);
    skips.push_back(fourth_index);

    for (map<int, int>::iterator it = id_location.begin(); it != id_location.end(); it++)
    {
        if (first_index == it->second)
        {
            Film* temp = find(it->first);
            result.push_back(temp);
        }

        if (second_index == it->second)
        {
            Film* temp = find(it->first);
            result.push_back(temp);
        }

        if (third_index == it->second)
        {
            Film* temp = find(it->first);
            result.push_back(temp);
        }

        if (fourth_index == it->second)
        {
            Film* temp = find(it->first);
            result.push_back(temp);
        }
    } 
    return result;
}

void FilmRepository::update_adjacency_matrix(Customer* user, Film* film)
{
    map<string, string> filter;//are?
    int new_film_location = id_location[film->get_id()];
    vector<Film*> purchased = user->get_purchased_films(filter);
    for (int i = 0; i < purchased.size(); i++)
    {
        if (purchased[i]->is_published() && purchased[i] != film) 
        {
            int location = id_location[purchased[i]->get_id()];
            if (adjacency_matrix[location][new_film_location] != -1)
                adjacency_matrix[location][new_film_location]++;
            if (adjacency_matrix[new_film_location][location] != -1)            
                adjacency_matrix[new_film_location][location]++;
        }
    }
}