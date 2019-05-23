#include "notification.h"

using namespace std;

Notification::Notification(string msg) 
{ 
    read = false;  
    message = msg;  
}
bool Notification::get_read() 
{    
    return read;    
}
string Notification::get_message()   
{   
    return message;    
}
void Notification::is_read(bool state) 
{   
    read = state;    
}