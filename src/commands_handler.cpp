#include "network.h"
#include "commands_handler.h"
#include <iostream>
#include <sstream>

using namespace std;

bool has_key(map<string, string> m, string key)
{
    return !(m.find(key) == m.end());
}

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
    //beshkon b func haye riz tar ?!
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
                if (method != POST && method != PUT && method != GET && method != DEL)
                    throw Bad_Request_Ex();
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
                    net->signup(parameters.at("email"), parameters.at("username"), parameters.at("password"), stoi(parameters.at("age")), publisher);  
                    cout << OK;
                }
                else if (command == LOGIN)
                {
                    if (method != POST)
                        throw Bad_Request_Ex();
                    get_parameters(line_stream, parameters);
                    net->login(parameters.at("username"), parameters.at("password"));
                    cout << OK;
                }
                else if (command == FILMS)
                {
                    get_parameters(line_stream, parameters);
                    if(method == POST)
                    { 
                        net->add_film(stoi(parameters.at("year")), 
                            stoi(parameters.at("length")), stoi(parameters.at("price")),
                            parameters.at("name"), parameters.at("summary"), parameters.at("director"));
                        cout << OK;
                    }
                    else if (method == PUT)
                    {
                        net->edit_film(stoi(parameters.at("film_id")), parameters);
                        cout << OK;
                    }
                    else if (method == DEL)
                    {
                       net->delete_film(stoi(parameters.at("film_id")));
                       cout << OK;
                    }
                    else if (method == GET)
                    {
                        if (has_key(parameters, "film_id"))
                            net->get_details(stoi(parameters.at("film_id")));
                        else 
                            net->find_films(parameters);
                    }
                }
                else if (command == FOLLOWERS)
                {
                    if (method == GET)
                        net->get_followers();
                    else if (method == POST)
                    {
                        get_parameters(line_stream, parameters);    
                        net->follow(stoi(parameters.at("user_id")));
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
                        if (has_key(parameters, "amount"))
                            net->inc_money(stoi(parameters.at("amount")));
                        else
                            net->get_money();
                        cout << OK;
                    }
                    else
                        throw Bad_Request_Ex();
                    
                }
                else if (command == PUBLISHED)
                {
                    if (method == GET)
                    {
                        get_parameters(line_stream, parameters);
                        net->get_published_films(parameters);
                    }
                    else 
                        throw Bad_Request_Ex();
                }
                else if (command == REPLIES)
                {
                    if (method == POST)
                    {
                        get_parameters(line_stream, parameters);
                        net->reply(stoi(parameters.at("film_id")), stoi(parameters.at("comment_id")),
                             parameters.at("content"));                   
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
                        net->delete_comment(stoi(parameters.at("film_id")), stoi(parameters.at("comment_id")));
                        cout << OK;                        
                    }   
                    else if (method == POST)
                    {
                        get_parameters(line_stream, parameters);
                        net->add_comment(stoi(parameters.at("film_id")), parameters.at("content"));
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
                        net->buy_film(stoi(parameters.at("film_id")));                    
                        cout << OK;
                    }
                    else 
                        throw Bad_Request_Ex();
                }
                else if (command == RATE)
                {
                    get_parameters(line_stream, parameters);
                    net->rate_film(stoi(parameters.at("film_id")), stoi(parameters.at("score")));
                    cout << OK;                    
                }
                else if (command == PURCHASED)
                {
                    if (method == GET)
                    {
                    get_parameters(line_stream, parameters);
                    net->get_purchased_films(parameters);
                    }
                    else 
                        throw Bad_Request_Ex();
                }
                else if (command == NOTIFICATIONS)
                {
                    if (question_mark == "read")
                    {
                        line_stream >> question_mark;
                        get_parameters(line_stream, parameters);
                        net->get_all_notifications(stoi(parameters.at("limit")));
                    }
                    else
                       net->get_unread_notifications();
                }
            }
        }
        catch (invalid_argument &ia)
        {
            cout << BAD_REQUEST;
        }
        catch (out_of_range &ia)
        {
            cout << BAD_REQUEST;
        }
        catch (exception &ex)
        {
            cout << ex.what();
        }
    }
}
