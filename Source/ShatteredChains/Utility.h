#pragma once

#include <string>
#include <exception>

namespace Validity
{
    class NullPointerException : public std::exception
    {
    public:
        NullPointerException(const std::string& message) : message(message) {}
        const char* what() const noexcept override
        {
            return message.c_str();
        }
    private:
        std::string message;
    };


    template <typename T> T* check_value(T* value, const std::string& error_message)
    {
        /*
        Checks is value is nullptr and if it is throw exception
        */
        if (value == nullptr) {
            throw NullPointerException(error_message);
        }
        return value;
    }
}
