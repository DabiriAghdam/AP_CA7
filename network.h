#ifndef NETWORK_H
#define NETWORK_H

#include "customer.h"
#include "film.h"
#include "orders_handler.h"
#include <vector>

class Network {
protected:
    std::vector<Customer> users;
    std::vector<Film> films;
};

#endif