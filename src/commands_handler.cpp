#include "network.h"
#include "commands_handler.h"
#include <iostream>
#include <sstream>

using namespace std;

CommandsHandler::CommandsHandler(Network* _net)
{
    net = _net;
}

void CommandsHandler::get_parameters(istringstream& line_stream, map<string, string> &params)
{
    string key, value;
    while(line_stream >> key)
    {
        line_stream >> value;
        params[key] = value;
    } 
}

void CommandsHandler::run()
{
    string line, method, command, question_mark, key, value;
    while(getline(cin, line))
    {
        try 
        {
            map<string, string> parameters;
            // parameters.erase(parameters.begin(), parameters.end());
            istringstream line_stream(line);
            while (line_stream >> method) 
            {
                if (method != POST && method != PUT && method != GET && method != DEL) //lazeme?
                {
                    throw Bad_Request_Ex();
                }
                line_stream >> command;
                line_stream >> question_mark;
                if (command == SIGNUP)
                {
                    if (method != POST)
                        throw Bad_Request_Ex();
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
                    bool ok = net->signup(parameters["email"], parameters["username"], parameters["password"], stoi(parameters["age"]), publisher);  
                    if(ok)
                        cout << OK;
                }
                else if (command == LOGIN)
                {
                    if (method != POST)
                        throw Bad_Request_Ex();
                    get_parameters(line_stream, parameters);
                    bool ok = net->login(parameters["username"], parameters["password"]);
                    if (ok) 
                        cout << OK;
                }
                else if (command == FILMS)
                {
                    if(method == POST)
                    { 
                        get_parameters(line_stream, parameters);
                        bool ok = net->add_film(stoi(parameters["year"]), 
                            stoi(parameters["length"]), stoi(parameters["price"]),parameters["name"],
                            parameters["summary"], parameters["director"]);
                        if(ok)  
                            cout << OK;
                    }
                    else if (method == PUT)
                    {
                        bool ok = net->edit_film(parameters);
                        if(ok)  
                            cout << OK;
                    }
                    else if (method == DEL);
                        // net->get_film_repository()->delete_film(stoi(parameters["film_id"]));
                }
                else if (command == FOLLOWERS)
                {
                    // net->get_logged_in_user()->get_followers();
                }
                else if (command == MONEY)
                {
                    
                }
                else if (command == PUBLISHED)
                {
                    
                }
                else if (command == REPLIES)
                {
                    
                }
                else if (command == COMMENTS)
                {
                    
                }
                else if (command == BUY)
                {
                    
                }
                else if (command == RATE)
                {
                    
                }
                else if (command == PURCHASED)
                {
                    
                }
                else if (command == NOTIFICATIONS)
                {
                    
                }
            }
        }
        catch (Exception &ex)
        {
            cout << ex.what();
        }
    }
}