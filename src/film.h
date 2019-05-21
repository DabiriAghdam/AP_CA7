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
    int scores_count;
    bool published;
    std::string name;
    std::string summary;
    std::string director;
    std::vector<Comment*> comments; 
    Publisher* publisher;   
public:
    Film(int _id, int _year, int _length, int _price, std::string _name, std::string _summary, std::string _director, Publisher* _publisher);
    int get_id()    {   return id; }
    void unpublish() {  published = false;  }
    Comment* get_comment(int id);
    std::vector<Comment*> get_all_comments()    {   return comments;    }
    void delete_comment(int id);
    void add_comment(std::string content, Customer* author);
    std::string get_name()  {   return name;    }
    std::string get_director()  {   return director;    }
    std::string get_summary()  {   return summary;    }
    int get_length()  {   return length;    }
    int get_year()  {   return year;    }
    float get_score() {  return score;   }
    int get_price() {   return price;   }
    Publisher* get_publisher()  {   return publisher;  }
    void set_score(int score);
    void set_name(std::string _name) {   name = _name;  }
    void set_year(int _year) { year = _year;    }
    void set_length(int _length) { length = _length;    }
    void set_summary(std::string _summary) {  summary = _summary;  }
    void set_director(std::string _director) {  director = _director;   }
    bool is_published() {   return published;   }
};

#endif