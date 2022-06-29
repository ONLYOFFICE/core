#pragma once

#include <exception>
#include <string>

namespace RedBlackTree
{
class RBTreeException : virtual public std::exception
{
public:
    RBTreeException() {}
    RBTreeException(std::wstring message) : errorMessage(message) {}
    RBTreeException(std::wstring message, std::exception& ex) : std::exception(ex), errorMessage(message) {}
    virtual ~RBTreeException() throw () {}

    virtual const char* what() const throw () {
        return errorMessage.c_str();
    }

protected:
    std::wstring errorMessage;
};

class RBTreeDuplicatedItemException : public RBTreeException
{
public:
    RBTreeDuplicatedItemException(std::wstring msg)
    : RBTreeException(msg)
    {
    }
};
}
