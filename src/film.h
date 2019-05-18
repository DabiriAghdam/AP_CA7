#ifndef FILM_H
#define FILM_H

#include "comment.h"
#include "exception.h"
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
    Comment* get_comment(int id);
    int get_id()    {   return id; }
    Publisher* get_publisher()  {   return  publisher;  }
    void unpublish() {  published = false;  }
    void delete_comment(int id);
    std::string get_name()  {   return name;    }
    std::string get_director()  {   return director;    }
    std::string get_summary()  {   return summary;    }
    int get_length()  {   return length;    }
    int get_year()  {   return year;    }
    float get_score() {  return score;   }
    int get_price() {   return price;   }
    std::vector<Comment>* get_comments()    {   return &comments;    }

};

#endif