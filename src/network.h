#ifndef NETWORK_H
#define NETWORK_H

#include "customer.h"
#include "film.h"
#include "film_repository.h"
#include "commands_handler.h"
#include <vector>

class Network {
protected:
    std::vector<Customer> users;
    Film_Repository film_repository;
};

#endif