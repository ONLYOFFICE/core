/// \brief  јбстрактный интерфейс Simple API XML
/// \author pogorskiy@gmail.com (@avsmedia.net)

#ifndef _CPDOCCORE_XML_SAX_H_
#define _CPDOCCORE_XML_SAX_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>
#include <exception>
#include <iosfwd>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/xml/nodetype.h>

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
    virtual unsigned int attrCount() = 0;
    virtual std::wstring baseURI() = 0;
    virtual unsigned int depth() = 0;

    // <p:abc/>, returns "abc".
    virtual std::wstring nodeLocalName() = 0;
    
    // <p:abc />, returns "p:abc".
    virtual std::wstring nodeQualifiedName() = 0;

    // <xyz:abc xmlns:xyz="u://1" />, it returns "xyz".
    virtual std::wstring namespacePrefix() = 0;

    // Returns "u://1" for the element <xyz:abc xmlns:xyz="u://1" />.
    virtual std::wstring namespaceUri() = 0;

    virtual NodeType nodeType() = 0;
    virtual std::wstring value() = 0;

    // <element attribute="123"></element> Ч false
    // <element attribute="123"/> - true
    virtual bool isEmptyElement() = 0;
    
    virtual bool isEof() = 0;

    virtual bool moveToAttrByName(const wchar_t * Name, const wchar_t * nsURI) = 0;
    virtual bool moveToAttrFirst() = 0;
    virtual bool moveToAttrNext() = 0;
    virtual void moveToAttrOwner() = 0;
    virtual bool attrDefault() = 0;

    virtual NodeType next() = 0;
    
    virtual unsigned int lineNumber() = 0;
    virtual unsigned int linePosition() = 0;

    virtual unsigned int readChunk(wchar_t * Buffer, unsigned int Size) = 0;

    virtual ~sax() = 0;

};


inline sax::~sax() {};

sax_ptr create_sax(const wchar_t * FileName);
sax_ptr create_sax(shared_ptr< std::istream >::Type istreamPtr);
sax_ptr create_sax(std::istream & istreamVal);

//sax_ptr create_sax(const std::string & String);

} // namespace xml
} // namespace cpdoccore


#endif // #ifndef _CPDOCCORE_XML_SAX_H_
