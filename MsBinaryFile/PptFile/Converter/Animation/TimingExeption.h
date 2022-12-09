#pragma once

#include <exception>
#include <string>

namespace PPT {
namespace Converter {
class TimingExeption : virtual public std::exception
{
public:
    TimingExeption() {}
    TimingExeption(std::string message) : errorMessage(message) {}
    TimingExeption(std::string message, std::exception& ex) : std::exception(ex), errorMessage(message) {}
    TimingExeption(std::wstring message) : werrorMessage(message) {}
    TimingExeption(std::wstring message, std::exception& ex) : std::exception(ex), werrorMessage(message) {}

    virtual ~TimingExeption() throw () {}

    virtual const char* what() const throw () {
        return errorMessage.c_str();
    }
    virtual const wchar_t* what_w() const throw () {
        return werrorMessage.c_str();
    }

protected:
    std::string errorMessage;
    std::wstring werrorMessage;
};
}
}
