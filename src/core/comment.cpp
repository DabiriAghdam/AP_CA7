#include "comment.h"

using namespace std;

Comment::Comment(int _id, string _content, Customer* _author)   
{
    id = _id;
    published = true;
    content = _content;   
    author = _author;    
}

void Comment::add_reply(string content)
{
    replies.push_back(content);
}

Customer* Comment::get_author() 
{    
    return author;  
}

void Comment::unpublish()    
{   
    published = false; 
}

string Comment::get_content() 
{ 
     return content; 
}

vector<string>* Comment::get_replies() 
{   
    return &replies; 
}