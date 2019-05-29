#ifndef _MY_HANDLERS_
#define _MY_HANDLERS_

#include "../server/server.hpp"
#include <iostream>

class Adder : public RequestHandler {
public:
  Response *callback(Request *);
};

class AddFilm : public RequestHandler {
public:
  Response *callback(Request *);
};

class SignUp : public RequestHandler {
public:
  Response *callback(Request *);
};

class Login : public RequestHandler {
public:
  Response *callback(Request *);
};

#endif