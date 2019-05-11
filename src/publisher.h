#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "customer.h"
#include "film.h"
#include <vector>

class Publisher : public Customer {
protected:
    int revenue;
    std::vector<Customer*> followers;
    std::vector<Film*> published_films;
};

#endif