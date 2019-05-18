#ifndef COMMENT_H
#define COMMENT_H

#include <string>
#include <vector>

class Customer;

class Comment {
protected:
    int id;
    bool published;
    Customer* author;
    std::string content;
    std::vector<std::string> replies;
public:
    Comment()   {   published = true;    }
    int get_id() {  return id;  }
    void add_reply(std::string content);
    Customer* get_author() {    return author;  }
    void unpublish()    {   published = false; }
    std::string get_content() { return content; }
    std::vector<std::string>* get_replies() {   return &replies; }
};

#endif