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

GetHome::GetHome(Network* _net, string filePath) : net(_net), TemplateHandler(filePath) {}

GetProfile::GetProfile(Network* _net, string filePath) : net(_net), TemplateHandler(filePath) {}

AddMoney::AddMoney(Network* _net) : net(_net) {}

DeleteFilm::DeleteFilm(Network* _net) : net(_net) {}

BuyFilm::BuyFilm(Network* _net) : net(_net) {}

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
		int id = net->signup(-1, email, username, password, age, publisher);
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
		return Response::redirect("/home");

	Response* res = new Response(303);
	string username = req->getBodyParam("username");
	string password = req->getBodyParam("password");
	try
	{
		int id = net->login(-1, username, password);
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
	if (req->getSessionId() == "")
		return Response::redirect("/login");
	
	Response* res = new Response(303);
	try
	{
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		net->logout(session_id);
		res->setSessionId("");
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST); //kafie?
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
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		net->add_film(session_id, year, length, price, name, summary, director);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST); //kafie?
	}
	res->setHeader("Location", "/home");
	return res;
}

map<string, string> GetFilm::handle(Request *req)
{
	// if (req->getSessionId() == "") //chi kar konim?!
	// 	return Response::redirect("/login");
	
	map<string, string> context;
	try
	{
		int film_id = stoi(req->getQueryParam("film_id"));
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		context = net->get_details(session_id, film_id);
	}
	catch (Not_Found_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST); //kafie?
	}
  	return context;
}


map<string, string> GetHome::handle(Request *req)
{
	// if (req->getSessionId() == "") //chi kar konim?!
	// 	return Response::redirect("/login");
	
	map<string, string> context;
	try
	{
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		// map<string, string> filters;//director?!
		context = net->get_home_films(session_id);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST); //kafie?
	}
  	return context;
}

map<string, string> GetProfile::handle(Request *req)
{
	// if (req->getSessionId() == "") //chi kar konim?!
	// 	return Response::redirect("/login");
	
	map<string, string> context;
	try
	{
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		map<string, string> filters;//director?!
		context = net->get_profile_films(session_id);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST); //kafie?
	}
  	return context;
}

Response* AddMoney::callback(Request *req) 
{
	if (req->getSessionId() == "")
		return Response::redirect("/login");

	Response* res = new Response(303);
	int money = stoi(req->getBodyParam("money"));
	try
	{
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		net->inc_money(session_id, money);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST); //kafie?
	}
	res->setHeader("Location", "/profile");
	return res;
}

Response* DeleteFilm::callback(Request *req) 
{
	if (req->getSessionId() == "")
		return Response::redirect("/login");

	Response* res = new Response(303);
	int film_id = stoi(req->getQueryParam("film_id"));
	try
	{
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		net->delete_film(session_id, film_id);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST); //kafie?
	}
	res->setHeader("Location", "/home");
	return res;
}

Response* BuyFilm::callback(Request *req) 
{
	if (req->getSessionId() == "")
		return Response::redirect("/login");

	Response* res = new Response(303);
	int film_id = stoi(req->getQueryParam("film_id"));
	try
	{
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		net->buy_film(session_id, film_id);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); //kafie?
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST); //kafie?
	}
	res->setHeader("Location", "/home");
	return res;
}
