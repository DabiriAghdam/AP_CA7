#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

#define NOT_FOUND "Not Found\n"
#define BAD_REQUEST "Bad Request\n"
#define PERMISSION_DENIED "Permission Denied\n"

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

#endif