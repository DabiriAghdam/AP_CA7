#ifndef FILM_REPOSITORY_H
#define FILM_REPOSITORY_H

#include "film.h"

class FilmRepository {
protected:
    std::vector<Film*> films;
public:
    ~FilmRepository();
    std::vector<Film*> find(std::map<std::string, std::string> filters);
    Film* find(int film_id);
    void add(Film* new_film);
    void edit(std::map<std::string, std::string> parameters);
    void remove(int film_id);
    int get_films_count();
    std::vector<Film*> get_recommendation(Film* film, Customer* user);
};

#endif