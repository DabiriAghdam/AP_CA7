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
    Comment(int _id, std::string _content, Customer* _author);
    int get_id() {  return id;  }
    void add_reply(std::string content);
    Customer* get_author();
    void unpublish();
    std::string get_content();
    std::vector<std::string>* get_replies();
};

#endif