#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

class Exception {
public:
    std::string what() {}
};

class Not_Found_Ex : public Exception {
public:
    std::string what() { return "Not Found\n"; }
};

class Bad_Request_Ex : public Exception {
public:
    std::string what() { return "Bad Request\n"; }
};

class Permission_Denied_Ex : public Exception {
public:
    std::string what() { return "Permission Denied\n"; }
};

#endif