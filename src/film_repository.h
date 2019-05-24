#ifndef FILM_REPOSITORY_H
#define FILM_REPOSITORY_H

#include "film.h"

#define RECOMMENDED_COUNT 4

class FilmRepository {
protected:
    std::vector<Film*> films;
    std::vector<std::vector<int>> adjacency_matrix;
    std::map<int, int> id_location;
public:
    ~FilmRepository();
    std::vector<Film*> find(std::map<std::string, std::string> filters);
    std::vector<Film*> get_recommendations(Film* film, Customer* user);
    Film* find(int film_id);
    void add(Film* new_film);
    void edit(std::map<std::string, std::string> parameters);
    void remove(int film_id);
    int get_films_count();
    void update_adjacency_matrix(Customer* user, Film* film);
};

#endif