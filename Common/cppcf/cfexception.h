#pragma once

#include <exception>
#include <string>

namespace CFCPP
{
class CFException : virtual public std::exception
{
public:
    CFException() {}
    CFException(std::string message) : errorMessage(message) {}
    virtual ~CFException() throw () {}

    virtual const char* what() const throw () {
        return errorMessage.c_str();
    }

protected:
    std::string errorMessage;
};


class CFFileFormatException : public CFException
{
public:
    CFFileFormatException() {}
    CFFileFormatException(std::string message) : CFException(message) {}
    virtual ~CFFileFormatException() throw () {}
};

class CFDisposedException : public CFException
{
public:
    CFDisposedException() {}
    CFDisposedException(std::string message) : CFException(message) {}
    virtual ~CFDisposedException() throw () {}
};

class CFInvalidOperation : public CFException
{
public:
    CFInvalidOperation() {}
    CFInvalidOperation(std::string message) : CFException(message) {}
    virtual ~CFInvalidOperation() throw () {}
};


}
