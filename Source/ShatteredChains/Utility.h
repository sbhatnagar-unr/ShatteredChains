#pragma once

#include <string>
#include <exception>

namespace Validity
{
    class NullPointerException final : public std::exception
    {
    public:
        explicit NullPointerException(const std::string& message) : message(message) {}
        virtual const char* what() const noexcept override
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

    // Overloaded check_value that lets you pick the bad value
    template <typename T> T check_value(T value, T bad_value, const std::string& error_message)
    {
        /*
        Checks is value is nullptr and if it is throw exception
        */
        if (value == bad_value) {
            throw NullPointerException(error_message);
        }
        return value;
    }
}