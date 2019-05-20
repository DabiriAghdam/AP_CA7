#include "comment.h"

using namespace std;

Comment::Comment(int _id, std::string _content, Customer* _author)   
{
    id = _id;
    published = true;
    content = _content;   
    author = _author;    
}
void Comment::add_reply(string content)
{
    replies.push_back(content);
    //todo : notification to author 
}

