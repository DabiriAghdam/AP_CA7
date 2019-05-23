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
    publisher = _publisher;
    published = true;
}

Comment* Film::get_comment(int id)
{
    for (int i = 0; i < comments.size(); i++)
    {
        if (comments[i]->get_id() == id)
            return comments[i];
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

void Film::set_score(int user_id, int score)
{
    scores[user_id] = score;
}

float Film::get_score()
{
    if (scores.size() == 0)
        return 0;
    float sum = 0;
    for (map<int, int>::iterator it = scores.begin(); it != scores.end(); it++)
        sum += it->second;
    return sum / scores.size();
}


void Film::add_comment(string content, Customer* author)
{
    int id = comments.size() + 1;
    Comment* comment = new Comment(id, content, author);
    comments.push_back(comment);
}
