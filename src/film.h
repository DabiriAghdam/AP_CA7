#ifndef FILM_H
#define FILM_H

#include "comment.h"
#include "exception.h"
#include <string>
#include <vector>
#include <map>

class Publisher;

class Film {
protected:
    int id;
    int year;
    int length;
    int price;
    // float score;
    // int scores_count;
    std::map<int, int> scores;
    bool published;
    std::string name;
    std::string summary;
    std::string director;
    std::vector<Comment*> comments; 
    Publisher* publisher;   
public:
    Film(int _id, int _year, int _length, int _price, std::string _name, std::string _summary, std::string _director, Publisher* _publisher);
    Comment* get_comment(int id);
    void delete_comment(int id);
    float get_score();
    void add_comment(std::string content, Customer* author);
    void set_score(int user_id, int score);
    std::vector<Comment*> get_all_comments();
    void unpublish();
    int get_id();
    std::string get_name();
    std::string get_director();
    std::string get_summary();
    int get_length();
    int get_year();
    int get_price();
    Publisher* get_publisher();
    void set_name(std::string _name);
    void set_year(int _year);
    void set_length(int _length);
    void set_summary(std::string _summary);
    void set_director(std::string _director);
    bool is_published();
};

#endif