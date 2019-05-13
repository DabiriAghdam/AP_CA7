#include "network.h"
#include "commands_handler.h"
#include <iostream>
#include <sstream>

using namespace std;

Commands_Handler::Commands_Handler(Network* _net)
{
    net = _net;
}

void Commands_Handler::get_parameters(istringstream& line_stream, map<string, string> &params)
{
    string key, value;
    while(line_stream >> key)
    {
        line_stream >> value;
        params[key] = value;
    } 
}

void Commands_Handler::start()
{
    string line, method, command, question_mark, key, value;
    map<string, string> parameters;
    while(getline(cin, line))
    {
        istringstream line_stream(line);
        while (line_stream >> method) 
        {
            if (method != "POST" && method != "PUT" && method != "GET" && method != "DELETE")
            {
                throw Bad_Request_Ex();
            }
            line_stream >> question_mark;
            line_stream >> command;
            if (command == "signup")
            {
                get_parameters(line_stream, parameters);
                bool publisher;
                if (parameters.find("publisher") == parameters.end())
                    publisher = false;
                else 
                {
                    if (parameters["publisher"] == "false")
                        publisher = false;
                    else 
                        publisher = true;
                }
                // net->signup(parameters["email"], parameters["username"], parameters["password"], stoi(parameters["age"]), publisher);  
            }
            else if (command == "login")
            {
                get_parameters(line_stream, parameters);
                // net->login(parameters["username"], parameters["password"]);
            }
            else if (command == "films")
            {
                get_parameters(line_stream, parameters);
                if(method == "POST") ;
                    // net->get_film_repository()->add_film(parameters["name"], stoi(parameters["year"]), 
                    // stoi(parameters["length"]), stoi(parameters["price"]),
                    // parameters["summary"], parameters["director"]);
                else if (method == "PUT");
                    //net->get_film_repository()->edit_film(parameters);
                else if (method == "DELETE");
                    // net->get_film_repository()->delete_film(stoi(parameters["film_id"]));
            }
            else if (command == "followers")
            {
                // net->get_logged_in_user()->get_followers();
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
}