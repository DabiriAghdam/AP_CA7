#ifndef FILM_REPOSITORY_H
#define FILM_REPOSITORY_H

#include "film.h"
#include <string>
#include <map>

class FilmRepository {
protected:
    std::vector<Film*> films;
public:
    std::vector<Film*> find(std::map<std::string, std::string> filters);
    Film* find(int film_id);
    // std::string get_details(int film_id); //lazeme?
    void add(Film* new_film);
    void edit(std::map<std::string, std::string> parameters);
    void remove(int film_id);
    int get_films_count()   {   return films.size();    }
};

#endif