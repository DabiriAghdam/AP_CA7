#include "handlers.h"
#include "network.h"
#include <string>
#include <sstream>

using namespace std;

AddFilm::AddFilm(Network* _net) : net(_net) {}

SignUpHandler::SignUpHandler(Network* _net) : net(_net) {}

LoginHandler::LoginHandler(Network* _net) : net(_net) {}

LogoutHandler::LogoutHandler(Network* _net) : net(_net) {}

GetFilm::GetFilm(Network* _net, string filePath) : net(_net), TemplateHandler(filePath) {}


Response* SignUpHandler::callback(Request *req) 
{
	if (req->getSessionId() != "")
	return Response::redirect("/home");

	Response* res = new Response(303);
	string email = req->getBodyParam("email");
	string username = req->getBodyParam("username");
	string password = req->getBodyParam("password");
	int age = stoi(req->getBodyParam("age"));
	bool publisher = false;
	if (req->getBodyParam("publisher") == "on")
	publisher = true;
	try
	{
		int id = net->signup(email, username, password, age, publisher);
		res->setSessionId(to_string(id));
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	res->setHeader("Location", "/home");
	return res;
}

Response* LoginHandler::callback(Request *req) 
{
	if (req->getSessionId() != "")
		return Response::redirect("/homepage");

	Response* res = new Response(303);
	string username = req->getBodyParam("username");
	string password = req->getBodyParam("password");
	try
	{
		int id = net->login(username, password);
		res->setSessionId(to_string(id));
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
  	res->setHeader("Location", "/home");
	return res;
}

Response* LogoutHandler::callback(Request *req) 
{
	Response* res = new Response(303);
	try
	{
		net->logout();
		res->setSessionId("");
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
  	res->setHeader("Location", "/login");
	return res;
}

Response* AddFilm::callback(Request *req) 
{
	if (req->getSessionId() == "")
	return Response::redirect("/login");

	Response* res = new Response(303);
	string name = req->getBodyParam("name");
	string director = req->getBodyParam("director");
	string summary = req->getBodyParam("summary");
	int price = stoi(req->getBodyParam("price"));
	int year = stoi(req->getBodyParam("year"));
	int length = stoi(req->getBodyParam("length"));
	try
	{
		net->add_film(year, length, price, name, summary, director);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	res->setHeader("Location", "/home");
	return res;
}

map<string, string> GetFilm::handle(Request *req)
{
	map<string, string> context;
	try
	{
		int film_id = stoi(req->getQueryParam("film_id"));
		context = net->get_details(film_id);
	}
	catch (Not_Found_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
  	return context;
}
