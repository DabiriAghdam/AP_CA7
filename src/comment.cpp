#include "comment.h"

using namespace std;

void Comment::add_reply(string content)
{
    replies.push_back(content);
}