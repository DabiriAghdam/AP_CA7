#include "network.h"
#include "commands_handler.h"
#include <iostream>

using namespace std;

Commands_Handler::Commands_Handler(Network* _net)
{
    net = _net;
}

void Commands_Handler::start()
{
    string method, command;
    map<string, string> parameters;
    
	while (cin >> method) 
    {
        if (method != "POST" || method != "PUT" || method != "GET" || method != "DELETE")
        {
            throw Bad_Request_Ex();
        }
        cin >> command;
        if (command == "signup")
        {

        }
        else if (command == "login")
        {

        }
        else if (command == "films")
        {
            
        }
        else if (command == "followers")
        {
            
        }
        else if (command == "money")
        {
            
        }
        else if (command == "published")
        {
            
        }
        else if (command == "replies")
        {
            
        }
        else if (command == "comments")
        {
            
        }
        else if (command == "buy")
        {
            
        }
        else if (command == "rate")
        {
            
        }
        else if (command == "purchased")
        {
            
        }
        else if (command == "notifications")
        {
            
        }
        else if (command == "rate")
        {
            
        }
	}
}