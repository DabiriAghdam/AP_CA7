#ifndef FILM_REPOSITORY_H
#define FILM_REPOSITORY_H

#include "film.h"
#include <string>
#include <map>

class Film_Repository {
protected:
    std::vector<Film> films;
public:
    std::vector<Film> find(std::map<std::string, std::string> filters);
    std::string get_details(int film_id);
    void add_film(std::string name, int year, int length, int price, std::string summary, std::string director);
    void edit_film(std::map<std::string, std::string> parameters);
    void delete_film(int film_id);
};

#endif