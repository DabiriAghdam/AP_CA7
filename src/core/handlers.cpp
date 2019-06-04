#include "handlers.h"
#include "network.h"
#include <string>
#include <sstream>

using namespace std;

AddFilm::AddFilm(Network* _net) : net(_net) {}

SignUpHandler::SignUpHandler(Network* _net) : net(_net) {}

LoginHandler::LoginHandler(Network* _net) : net(_net) {}

LogoutHandler::LogoutHandler(Network* _net) : net(_net) {}

GetSignup::GetSignup(Network* _net) : net(_net) {}

GetLogin::GetLogin(Network* _net) : net(_net) {}

GetFilm::GetFilm(Network* _net, string filePath) : net(_net), TemplateHandler(filePath) {}

GetAddFilm::GetAddFilm(Network* _net) : net(_net) {}

GetHome::GetHome(Network* _net, string filePath) : net(_net), TemplateHandler(filePath) {}

GetProfile::GetProfile(Network* _net, string filePath) : net(_net), TemplateHandler(filePath) {}

AddMoney::AddMoney(Network* _net) : net(_net) {}

DeleteFilm::DeleteFilm(Network* _net) : net(_net) {}

BuyFilm::BuyFilm(Network* _net) : net(_net) {}

RateFilm::RateFilm(Network* _net) : net(_net) {}

AddComment::AddComment(Network* _net) : net(_net) {}

Response* SignUpHandler::callback(Request *req) 
{
	if (req->getSessionId() != "")
	return Response::redirect("/home");

	Response* res = new Response(303);
	string email = req->getBodyParam("email");
	string username = req->getBodyParam("username");
	string password = req->getBodyParam("password");
	string password_repeat = req->getBodyParam("password_repeat");
	int age = stoi(req->getBodyParam("age"));
	bool publisher = false;
	if (req->getBodyParam("publisher") == "on")
	publisher = true;
	try
	{
		int id = net->signup(-1, email, username, password, password_repeat, age, publisher);
		res->setSessionId(to_string(id));
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
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
		throw Server::Exception(ex.what());
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}
	
  	res->setHeader("Location", "/home");
	return res;
}

Response* GetLogin::callback(Request *req) 
{
	if (req->getSessionId() != "")
	{
		int session_id = stoi(req->getSessionId());
		if (net->logged_in(session_id))
			return Response::redirect("/home");
	}

	Response *res = new Response;
  	res->setHeader("Content-Type", "html");
  	res->setBody(readFile("src/static/login.html"));
  	return res;
}

Response* GetSignup::callback(Request *req) 
{
	if (req->getSessionId() != "")
	{
		int session_id = stoi(req->getSessionId());
		if (net->logged_in(session_id))
			return Response::redirect("/home");
	}
		
	Response *res = new Response;
  	res->setHeader("Content-Type", "html");
  	res->setBody(readFile("src/static/signup.html"));
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
		throw Server::Exception(ex.what());
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST);
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}

  	res->setHeader("Location", "/login");
	return res;
}

Response* AddFilm::callback(Request *req) 
{
	if (req->getSessionId() == "")
		throw Server::Exception(PERMISSION_DENIED); 

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
		throw Server::Exception(ex.what());
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what()); 
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST); 
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}

	res->setHeader("Location", "/home");
	return res;
}


Response* GetAddFilm::callback(Request *req) 
{
	if (req->getSessionId() == "")
		throw Server::Exception(PERMISSION_DENIED);

	Response *res = new Response;
	try
	{
		int session_id = stoi(req->getSessionId());
		net->check_logged_in(session_id);
		net->check_user_access(session_id);
  		res->setHeader("Content-Type", "html");
  		res->setBody(readFile("src/static/addfilm.html"));
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}

  	return res;
}

map<string, string> GetFilm::handle(Request *req)
{
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
		throw Server::Exception(ex.what());
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST);
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}

  	return context;
}


map<string, string> GetHome::handle(Request *req)
{	
	map<string, string> context;
	try
	{
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		string director = req->getQueryParam("director");
		context = net->get_home_films(session_id, director);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST);
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}

  	return context;
}

map<string, string> GetProfile::handle(Request *req)
{	
	map<string, string> context;
	try
	{
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		map<string, string> filters;
		context = net->get_profile_films(session_id);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST);
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}

  	return context;
}

Response* AddMoney::callback(Request *req) 
{
	if (req->getSessionId() == "")
		throw Server::Exception(PERMISSION_DENIED); 		

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
		throw Server::Exception(ex.what());
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST);
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}

	res->setHeader("Location", "/profile");
	return res;
}

Response* DeleteFilm::callback(Request *req) 
{
	if (req->getSessionId() == "")
		throw Server::Exception(PERMISSION_DENIED); 				

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
		throw Server::Exception(ex.what());
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST);
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}

	res->setHeader("Location", "/home");
	return res;
}

Response* BuyFilm::callback(Request *req) 
{
	if (req->getSessionId() == "")
		throw Server::Exception(PERMISSION_DENIED); 						

	Response* res = new Response(303);
	try
	{
		int film_id = stoi(req->getQueryParam("film_id"));
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		net->buy_film(session_id, film_id);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST);
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}

	res->setHeader("Location", "/filmdetails?film_id=" + req->getQueryParam("film_id"));
	return res;
}

Response* RateFilm::callback(Request *req) 
{
	if (req->getSessionId() == "")
		throw Server::Exception(PERMISSION_DENIED); 						

	Response* res = new Response(303);
	try
	{
		int film_id = stoi(req->getQueryParam("film_id"));
		int score = stoi(req->getQueryParam("score"));
		int session_id = -1;
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		net->rate_film(session_id, film_id, score);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST);
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}

	res->setHeader("Location", "/filmdetails?film_id=" + req->getQueryParam("film_id"));
	return res;
}

Response* AddComment::callback(Request *req) 
{
	if (req->getSessionId() == "") 
		throw Server::Exception(PERMISSION_DENIED); 				

	Response* res = new Response(303);
	try
	{
		int session_id = -1;
		string content = req->getBodyParam("comment");
		if (req->getSessionId() != "")
			session_id = stoi(req->getSessionId());
		int film_id = stoi(req->getBodyParam("film_id"));;
		net->add_comment(session_id, film_id, content);
	}
	catch (Bad_Request_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (Permission_Denied_Ex ex)
	{
		throw Server::Exception(ex.what());
	}
	catch (invalid_argument)
	{
		throw Server::Exception(BAD_REQUEST);
	}
	catch (Server_Ex ex)
	{
		throw Server::Exception(ex.what());		
	}

	res->setHeader("Location", "/filmdetails?film_id=" + req->getBodyParam("film_id"));
	return res;
}