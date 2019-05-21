#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

class Notification {
protected:
    bool read;
    std::string message;
public:
    Notification(std::string msg) { read = false;   message = msg;  }
    bool get_read() {    return read;    }
    std::string get_message()   {   return message;    }
    void is_read(bool state) {   read = state;    }
};
#endif