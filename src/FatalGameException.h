//
// Created by Fiskie on 24/05/15.
//

#ifndef rocketcats_FATALGAMEEXCEPTION_H
#define rocketcats_FATALGAMEEXCEPTION_H

#include <exception>
#include <stdio.h>
#include <string.h>

using namespace std;

class FatalGameException : public exception {
private:
    const char *message;

public:
    FatalGameException(char *message) {
        this->message = message;
    }

    FatalGameException(const char *message) {
        this->message = message;
    }

    virtual const char *what() const throw() {
        char str[] = "Fatal game exception: ";

        return strcat(str, message);
    }
};


#endif //rocketcats_FATALGAMEEXCEPTION_H
