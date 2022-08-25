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
    CFException(std::wstring message) : werrorMessage(message) {}
    CFException(std::wstring message, std::exception& ex) : std::exception(ex), werrorMessage(message) {}

    virtual ~CFException() throw () {}

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
class CFItemNotFound : public CFException
{
public:
    CFItemNotFound() {}
    CFItemNotFound(std::wstring message) : CFException(message) {}
    virtual ~CFItemNotFound() throw () {}
};


}
