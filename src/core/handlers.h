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

class LoginHandler : public RequestHandler {
private:
  	Network* net;
public:
  	LoginHandler(Network* _net);
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

#endif