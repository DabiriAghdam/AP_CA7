#include "film_repository.h"

void FilmRepository::add(Film new_film)
{
    films.push_back(new_film);
}
Film* FilmRepository::find(int film_id)
{
    for (int i = 0; i< films.size(); i++)
    {
        if (films[i].get_id() == film_id)
            return &films[i];
    }
    return NULL;
}

void FilmRepository::remove(int film_id)
{
    Film* film = find(film_id);
    film->unpublish();
}