#pragma once
#include <exception>
#include <string>

namespace AudioDevices {

    class VolumeControlException : public std::exception {
    private:
        std::string message;

        VolumeControlException(std::string message) : 
            std::exception(message.c_str()) 
        {
            this->message = message;
        }

        const char* what() const throw() 
        {
            return message.c_str();
        }
    };
}