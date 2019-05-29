#include <string>
#include <sstream>
#include "handlers.h"
using namespace std;

Response* Login::callback(Request *req) {}
Response* SignUp::callback(Request *req) {}
Response* AddFilm::callback(Request *req) {}

Response *Adder::callback(Request *req) {
  Response *res = new Response;
  res->setHeader("Content-Type", "text/html");

  int num1 = stoi(req->getQueryParam("num1"));
  int num2 = stoi(req->getQueryParam("num2"));

  ostringstream body;
  body 
    << "<!DOCTYPE html>" << endl
    << "<html>" << endl
    << "<body>" << endl
    << num1 << "+" << num2 << "=" << (num1 + num2) << endl
    << "</body>" << endl
    << "</html>" << endl;
  res->setBody(body.str());
  return res;
}

