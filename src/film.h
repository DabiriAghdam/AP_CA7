#ifndef FILM_H
#define FILM_H

#include "comment.h"
#include <string>
#include <vector>

class Publisher;

class Film {
protected:
    int id;
    int year;
    int length;
    int price;
    float score;
    bool published;
    std::string name;
    std::string summary;
    std::string director;
    std::vector<Comment> comments; 
    Publisher* publisher;   
public:
    Film(int _id, int _year, int _length, int _price, std::string _name, std::string _summary, std::string _director, Publisher* _publisher);
    int get_id()    {   return id; }
    Publisher* get_publisher()  {   return  publisher;  }
    void unpublish() {  published = false;  }
};

#endif