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
    string line;
    while(getline(cin, line))
    {
        try 
        {
            string method, command, question_mark, key, value;
            map<string, string> parameters;
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
                    bool publisher = false;
                    if (has_key(parameters, "publisher") && parameters["publisher"] == "true")
                        publisher = true;
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
                    get_parameters(line_stream, parameters);
                    if(method == POST)
                    { 
                        bool ok = net->add_film(stoi(parameters["year"]), 
                            stoi(parameters["length"]), stoi(parameters["price"]),parameters["name"],
                            parameters["summary"], parameters["director"]);
                        if (ok)  
                            cout << OK;
                    }
                    else if (method == PUT)
                    {
                        net->edit_film(stoi(parameters["film_id"]), parameters);
                        cout << OK;
                    }
                    else if (method == DEL)
                    {
                       bool ok = net->delete_film(stoi(parameters["film_id"]));
                       if (ok)
                        cout << OK;
                    }
                    else if (method == GET)
                    {
                        if (has_key(parameters, "film_id"))
                            net->get_details(stoi(parameters["film_id"]));
                        else 
                        {
                        // todo: search
                        }
                    }
                }
                else if (command == FOLLOWERS)
                {
                    if (method == GET)
                        net->get_followers();
                    else if (method == POST)
                    {
                        get_parameters(line_stream, parameters);    
                        bool ok = net->follow(stoi(parameters["user_id"]));
                        if (ok)
                            cout << OK;
                    }
                    else 
                        throw Bad_Request_Ex();
                }
                else if (command == MONEY)
                {
                    if (method == POST)
                    {
                        get_parameters(line_stream, parameters); 
                        bool ok;
                        if (has_key(parameters, "amount"))
                            ok = net->inc_money(stoi(parameters["amount"]));
                        else
                            ok = net->give_money();
                        if (ok)
                            cout << OK;
                    }
                    else
                        throw Bad_Request_Ex();
                    
                }
                else if (command == PUBLISHED)
                {  
                   //todo 
                }
                else if (command == REPLIES)
                {
                    if (method == POST)
                    {
                        get_parameters(line_stream, parameters);
                        bool ok = net->reply(stoi(parameters["film_id"]), stoi(parameters["comment_id"]), parameters["content"]);                   
                        if (ok)
                            cout << OK;
                    }
                    else
                        throw Bad_Request_Ex();
                }
                else if (command == COMMENTS)
                {
                    if (method == DEL)
                    {
                        get_parameters(line_stream, parameters);
                        bool ok = net->delete_comment(stoi(parameters["film_id"]), stoi(parameters["comment_id"]));
                        if (ok)
                            cout << OK;                        
                    }   
                    else if (method == POST)
                    {
                        get_parameters(line_stream, parameters);
                        net->add_comment(stoi(parameters["film_id"]), parameters["content"]);
                        cout << OK;                                           
                    }
                    else 
                        throw Bad_Request_Ex();
                }
                else if (command == BUY)
                {
                    if (method == POST)
                    {
                        get_parameters(line_stream, parameters);
                        net->buy_film(stoi(parameters["film_id"]));                    
                        cout << OK;
                    }
                    else 
                        throw Bad_Request_Ex();
                }
                else if (command == RATE)
                {
                    get_parameters(line_stream, parameters);
                    net->rate_film(stoi(parameters["film_id"]), stoi(parameters["score"]));
                    cout << OK;                    
                }
                else if (command == PURCHASED)
                {
                    //todo
                }
                else if (command == NOTIFICATIONS)
                {
                    //todo                    
                }
            }
        }
        catch (Exception &ex)
        {
            cout << ex.what();
        }
        catch (...)
        {   
            cout << "Unhandled exception occured!" << endl;//lazeme?
        }
    }
}

bool CommandsHandler::has_key(map<string, string> m, string key)
{
    return !(m.find(key) == m.end());
}