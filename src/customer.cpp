#include "customer.h"

using namespace std;

Customer::Customer(int _id,int _age, int _money, string _email, string _username, string _password)
{
    id = _id;
    age = _age;
    money = _money;
    email = _email;
    username = _username;
    password = _password;
}
