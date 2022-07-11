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
    CFException(std::string message, std::exception& ex) : std::exception(ex), errorMessage(message) {}
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

class CFCorruptedFileException : public CFException
{
public:
    CFCorruptedFileException() {}
    CFCorruptedFileException(std::string message) : CFException(message) {}
    virtual ~CFCorruptedFileException() throw () {}
};


class CFDuplicatedItemException : public CFException
{
public:
    CFDuplicatedItemException() {}
    CFDuplicatedItemException(std::wstring message) : CFException(message) {}
    virtual ~CFDuplicatedItemException() throw () {}
};


}
