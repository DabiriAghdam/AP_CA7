#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

#define NOT_FOUND "Not Found"
#define BAD_REQUEST "Bad Request"
#define PERMISSION_DENIED "Permission Denied"

class Not_Found_Ex : public std::exception {
public:
    const char * what () const throw () { return NOT_FOUND; }
};

class Bad_Request_Ex : public std::exception {
public:
     const char * what () const throw () { return BAD_REQUEST; }
};

class Permission_Denied_Ex : public std::exception {
public:
    const char * what () const throw () { return PERMISSION_DENIED; }
};

class Server_Ex : public std::exception {
private:
    std::string message;
public:
    Server_Ex(std::string msg) : message(msg) {}
    const char * what () const throw () { return message.c_str(); }
};

#endif