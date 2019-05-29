#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

class Notification {
protected:
    bool read;
    std::string message;
public:
    Notification(std::string msg);
    bool get_read();
    std::string get_message();
    void is_read(bool state);
};
#endif