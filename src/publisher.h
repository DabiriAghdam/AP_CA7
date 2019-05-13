#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "customer.h"
#include "film.h"
#include <vector>

class Publisher : public Customer {
protected:
    std::vector<Customer*> followers;
    std::vector<Film*> published_films;
public:
    std::vector<Customer*> get_followers()  {   return followers;   }
};

#endif