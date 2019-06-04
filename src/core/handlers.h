#ifndef _MY_HANDLERS_
#define _MY_HANDLERS_

#include "../server/server.hpp"
#include <iostream>

class Network;

class AddFilm : public RequestHandler {
private:
  	Network* net;
public:
  	AddFilm(Network* _net);
  	Response *callback(Request *);
};

class SignUpHandler : public RequestHandler {
private:
  	Network* net;
public:
  	SignUpHandler(Network* _net);
  	Response *callback(Request *);
};

class GetSignup : public RequestHandler {
private:
  	Network* net;
public:
  	GetSignup(Network* _net);
  	Response *callback(Request *);
};

class LoginHandler : public RequestHandler {
private:
  	Network* net;
public:
  	LoginHandler(Network* _net);
  	Response *callback(Request *);
};

class GetLogin : public RequestHandler {
private:
  	Network* net;
public:
  	GetLogin(Network* _net);
  	Response *callback(Request *);
};

class LogoutHandler : public RequestHandler {
private:
  	Network* net;
public:
  	LogoutHandler(Network* _net);
  	Response *callback(Request *);
};

class GetFilm : public TemplateHandler  {
private:
  	Network* net;
public:
	GetFilm(Network* _net, std::string filePath);
  	std::map<std::string, std::string> handle(Request *req);
};

class GetAddFilm : public RequestHandler {
private:
  	Network* net;
public:
  	GetAddFilm(Network* _net);
  	Response *callback(Request *);
};

class GetHome : public TemplateHandler  {
private:
  	Network* net;
public:
	GetHome(Network* _net, std::string filePath);
  	std::map<std::string, std::string> handle(Request *req);
};

class GetProfile : public TemplateHandler  {
private:
  	Network* net;
public:
	GetProfile(Network* _net, std::string filePath);
  	std::map<std::string, std::string> handle(Request *req);
};

class AddMoney : public RequestHandler {
private:
  	Network* net;
public:
  	AddMoney(Network* _net);
  	Response *callback(Request *);
};

class DeleteFilm : public RequestHandler {
private:
  	Network* net;
public:
  	DeleteFilm(Network* _net);
  	Response *callback(Request *);
};

class BuyFilm : public RequestHandler {
private:
  	Network* net;
public:
  	BuyFilm(Network* _net);
  	Response *callback(Request *);
};

class RateFilm : public RequestHandler {
private:
  	Network* net;
public:
  	RateFilm(Network* _net);
  	Response *callback(Request *);
};

class AddComment : public RequestHandler {
private:
  	Network* net;
public:
  	AddComment(Network* _net);
  	Response *callback(Request *);
};

#endif