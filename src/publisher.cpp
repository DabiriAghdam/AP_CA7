#include "publisher.h"

void Publisher::add_follower(Customer* follower)
{
    followers.push_back(follower);
}