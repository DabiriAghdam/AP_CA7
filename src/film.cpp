#include "film.h"

using namespace std;

Film::Film(int _id, int _year, int _length, int _price, string _name, string _summary, string _director, Publisher* _publisher)
{
    id = _id;
    year = _year;
    length = _length;
    price = _price;
    name = _name;
    summary = _summary;
    director = _director;
    score = 0;
    scores_count = 0;
    publisher = _publisher;
    published = true;
}

Comment* Film::get_comment(int id)
{
    for (int i = 0; i < comments.size(); i++)
    {
        if (comments[i].get_id() == id)
            return &comments[i];
    }
    return NULL;
}

void Film::delete_comment(int id)
{
    Comment* comment = get_comment(id);
    if (comment == NULL)
        throw Not_Found_Ex();
    comment->unpublish();
}

void Film::set_score(int score)
{
    score = float((score * scores_count) + score) / (scores_count + 1);
}