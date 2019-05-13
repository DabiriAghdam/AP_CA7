#ifndef FILM_H
#define FILM_H

#include "comment.h"
#include <string>
#include <vector>

class Film {
protected:
    int id;
    int year;
    int length;
    int price;
    float score;
    std::string name;
    std::string summary;
    std::string director;
    std::vector<Comment> comments;    
};

#endif