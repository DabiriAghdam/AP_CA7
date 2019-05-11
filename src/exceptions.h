#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

class Exceptions { //lazeme?
public:
    virtual std::string get_msg() = 0;
};

class Not_Found_Ex : public Exceptions {
public:
    virtual std::string get_msg() { return "Not Found\n"; }
};

class Bad_Req_Ex : public Exceptions {
public:
    virtual std::string get_msg() { return "Bad Request\n"; }
};

class Permission_Denied_Ex : public Exceptions {
public:
    virtual std::string get_msg() { return "Permission Denied\n"; }
};

#endif