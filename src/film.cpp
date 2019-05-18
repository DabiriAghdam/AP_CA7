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
    publisher = _publisher;
    published = true;
}