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

Film::~Film()
{
    for (int i = 0; i < comments.size(); i++)
        delete comments[i];
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

int Film::get_price() 
{   
    return price;   
}

Publisher* Film::get_publisher()  
{   
    return publisher;  
}

void Film::set_name(string _name) 
{   
    name = _name;  
}

void Film::set_year(int _year) 
{ 
    year = _year;    
}

void Film::set_length(int _length) 
{ 
    length = _length;    
}

void Film::set_summary(string _summary) 
{  
    summary = _summary;  
}

void Film::set_director(string _director) 
{  
    director = _director;   
}

bool Film::is_published() 
{   
    return published;   
}

vector<Comment*> Film::get_all_comments()   
{   
    return comments;    
}

void Film::unpublish() 
{  
    published = false;  
}

int Film::get_id()    
{   
    return id; 
}

string Film::get_name()  
{   
    return name;    
}

string Film::get_director()  
{   
    return director;    
}

string Film::get_summary()  
{   
    return summary;    
}

int Film::get_length()  
{   
    return length;    
}

int Film::get_year()  
{  
    return year;    
}