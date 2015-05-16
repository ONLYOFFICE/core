//јбстрактный интерфейс Simple XML

#pragma once


#include <string>
#include <exception>
#include <iosfwd>

#include "../CPSharedPtr.h"
#include "nodetype.h"

namespace cpdoccore {
namespace xml {

class sax;
typedef _CP_PTR(sax) sax_ptr;

class sax
{
public:

    struct error : virtual public ::std::exception
    {
    public:
        enum Code {
            no = 0,
            openFile,
            createReader,
            internalErr
        };

    public:
        error(Code _code) : code_(_code){};
        Code code() const { return code_; }

    private:
        Code code_;
    };

public:
    //virtual std::wstring baseURI() = 0;
    virtual unsigned int depth() = 0;

    // <p:abc/>, returns "abc".
    virtual std::wstring nodeLocalName() = 0;
    
    // <p:abc />, returns "p:abc".
    virtual std::wstring nodeQualifiedName() = 0;

    // <xyz:abc xmlns:xyz="u://1" />, it returns "xyz".
    virtual std::wstring namespacePrefix() = 0;

    //// Returns "u://1" for the element <xyz:abc xmlns:xyz="u://1" />.
    //virtual std::wstring namespaceUri() = 0;

    virtual NodeType		nodeType() = 0;
	virtual std::wstring	value() = 0;
	
    virtual unsigned int	attrCount() = 0;
	//virtual std::wstring	value() = 0;
	virtual bool			attrDefault() = 0;

    // <element attribute="123"></element> Ч false
    // <element attribute="123"/> - true
    virtual bool isEmptyElement() = 0;
    
    virtual bool moveToAttrFirst() = 0;
    virtual bool moveToAttrNext() = 0;
    virtual bool moveToAttrOwner() = 0;

    virtual NodeType next(int Depth) = 0;
   
    virtual ~sax() = 0;

};


inline sax::~sax() {};

sax_ptr create_sax(const wchar_t * FileName);

} // namespace xml
} // namespace cpdoccore

