#ifndef __MY_SERVER__
#define __MY_SERVER__

#include "../server/server.hpp"

class HttpServer : public Server {
public:
  HttpServer(int port = 5000);
};

#endif
