/// \file   sax_win32_xmllite.cpp
/// \brief  –еализаци€ интерфейса SAX через библиотеку XmlLite

#include "precompiled_cpxml.h"

#include <string>

#include "win32ver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>

using namespace ATL;

#include <windows.h>
#include <xmllite.h>


#include <cpdoccore/xml/sax.h>
#include "sax_win32_xmllite.h"
#include "istream2Win32Stream.h"

namespace cpdoccore {
namespace xml {

class SaxWin32XmlLite : public sax
{
public:
    SaxWin32XmlLite(const wchar_t * FileName);
    SaxWin32XmlLite(shared_ptr< std::istream >::Type istreamPtr); 
    SaxWin32XmlLite(std::istream & istreamVal); 
    SaxWin32XmlLite();
    
public:
    virtual unsigned int attrCount();
    virtual std::wstring baseURI();
    virtual unsigned int depth();

    // <p:abc/>, returns "abc".
    virtual std::wstring nodeLocalName();
    
    // <p:abc />, returns "p:abc".
    virtual std::wstring nodeQualifiedName();

    // <xyz:abc xmlns:xyz="u://1" />, it returns "xyz".
    virtual std::wstring namespacePrefix();

    // Returns "u://1" for the element <xyz:abc xmlns:xyz="u://1" />.
    virtual std::wstring namespaceUri();

    virtual NodeType nodeType();
    virtual std::wstring value();

    // <element attribute="123"></element> Ч false
    // <element attribute="123"/> - true
    virtual bool isEmptyElement();
    
    virtual bool isEof();

    virtual bool moveToAttrByName(const wchar_t * Name, const wchar_t * nsURI);
    virtual bool moveToAttrFirst();
    virtual bool moveToAttrNext();
    virtual void moveToAttrOwner();
    virtual bool attrDefault();

    virtual NodeType next();
    
    virtual unsigned int lineNumber();
    virtual unsigned int linePosition();

    virtual unsigned int readChunk(wchar_t * Buffer, unsigned int Size);

private:
    ::ATL::CComPtr< IXmlReader > xml_;

};

namespace 
{
    NodeType NodeTypeConvert(XmlNodeType Type)
    {
        switch(Type)
        {
        case XmlNodeType_None:
            return typeNone;
        case XmlNodeType_Element:
            return typeElement;
        case XmlNodeType_Attribute:
            return typeAttribute;
        case XmlNodeType_Text:
            return typeText;
        case XmlNodeType_CDATA:
            return typeCDATA;
        case XmlNodeType_ProcessingInstruction:
            return typeProcessingInstruction;
        case XmlNodeType_Comment:
            return typeComment;
        case XmlNodeType_DocumentType:
            return typeDocumentType;
        case XmlNodeType_Whitespace:
            return typeWhitespace;
        case XmlNodeType_EndElement:
            return typeEndElement;
        case XmlNodeType_XmlDeclaration:
            return typeXmlDeclaration;
        default:
            return typeNone;
        }
    }
}

SaxWin32XmlLite::SaxWin32XmlLite(const wchar_t * FileName)
{
    ::ATL::CComPtr< IStream > pFileStream;
    HRESULT hr;
    if (FAILED(hr = ::SHCreateStreamOnFileW(FileName, STGM_READ, &pFileStream)))
    {
        //wprintf(L"Error creating file reader, error is %08.8lx", hr);
        throw error( error::openFile );
    }

    if (FAILED(hr = ::CreateXmlReader(__uuidof(IXmlReader), (void**)&xml_, NULL)))
    {
        //wprintf(L"Error creating xml reader, error is %08.8lx", hr);
        throw error( error::createReader );
    }

    if FAILED(xml_->SetInput(pFileStream))
    {
        throw error( error::createReader );
    };

}

SaxWin32XmlLite::SaxWin32XmlLite(shared_ptr< std::istream >::Type istreamPtr)
{
    HRESULT hr;
    ::ATL::CComPtr< IStream > stream = istreamToWin32IStream(istreamPtr);

    if (FAILED(hr = ::CreateXmlReader(__uuidof(IXmlReader), (void**)&xml_, NULL)))
    {
        //wprintf(L"Error creating xml reader, error is %08.8lx", hr);
        throw error( error::createReader );
    }

    if FAILED(xml_->SetInput(stream))
    {
        throw error( error::createReader );
    };
    
}

SaxWin32XmlLite::SaxWin32XmlLite(std::istream & istreamVal)
{
   HRESULT hr;
    ::ATL::CComPtr< IStream > stream = istreamToWin32IStream(istreamVal);

    if (FAILED(hr = ::CreateXmlReader(__uuidof(IXmlReader), (void**)&xml_, NULL)))
    {
        //wprintf(L"Error creating xml reader, error is %08.8lx", hr);
        throw error( error::createReader );
    }

    if FAILED(xml_->SetInput(stream))
    {
        throw error( error::createReader );
    };    
}

unsigned int SaxWin32XmlLite::attrCount()
{
    HRESULT hr;
    UINT count = 0;
    if FAILED(hr = xml_->GetAttributeCount(&count))
    {
        throw error(error::internalErr);    
    }
    return count;
}

std::wstring SaxWin32XmlLite::baseURI()
{
    HRESULT hr;
    UINT size = 0;
    const wchar_t * val;
    if FAILED(hr = xml_->GetBaseUri(&val, &size))
    {
        throw error(error::internalErr);    
    }
    if (size)
        return std::wstring(&val[0], &val[size]);
    else
        return L"";

}

unsigned int SaxWin32XmlLite::depth()
{
    HRESULT hr;
    UINT depth = 0;
    if FAILED(hr = xml_->GetDepth(&depth))
    {
        throw error(error::internalErr);    
    }
    return depth;
}

// <p:abc/>, returns "abc".
std::wstring SaxWin32XmlLite::nodeLocalName()
{
    HRESULT hr;
    UINT size = 0;
    const wchar_t * val;
    if FAILED(hr = xml_->GetLocalName(&val, &size))
    {
        throw error(error::internalErr);    
    }
    if (size)
        return std::wstring(&val[0], &val[size]);
    else
        return L"";
}

// <p:abc />, returns "p:abc".
std::wstring SaxWin32XmlLite::nodeQualifiedName()
{
    HRESULT hr;
    UINT size = 0;
    const wchar_t * val;
    if FAILED(hr = xml_->GetQualifiedName(&val, &size))
    {
        throw error(error::internalErr);    
    }
    if (size)
        return std::wstring(&val[0], &val[size]);
    else
        return L"";

}

// <xyz:abc xmlns:xyz="u://1" />, it returns "xyz".
std::wstring SaxWin32XmlLite::namespacePrefix()
{
    HRESULT hr;
    UINT size = 0;
    const wchar_t * val;
    if FAILED(hr = xml_->GetPrefix(&val, &size))
    {
        throw error(error::internalErr);    
    }
    if (size)
        return std::wstring(&val[0], &val[size]);
    else
        return L"";

}

// Returns "u://1" for the element <xyz:abc xmlns:xyz="u://1" />.
std::wstring SaxWin32XmlLite::namespaceUri()
{
    HRESULT hr;
    UINT size = 0;
    const wchar_t * val;
    if FAILED(hr = xml_->GetNamespaceUri(&val, &size))
    {
        throw error(error::internalErr);    
    }
    if (size)
        return std::wstring(&val[0], &val[size]);
    else
        return L"";

}

NodeType SaxWin32XmlLite::nodeType()
{
    HRESULT hr;
    XmlNodeType nodeType;
    if FAILED(hr = xml_->GetNodeType(&nodeType))
    {
        throw error(error::internalErr);    
    }
    return NodeTypeConvert(nodeType); 
}

std::wstring SaxWin32XmlLite::value()
{
    HRESULT hr;
    UINT size = 0;
    const wchar_t * val;
    if FAILED(hr = xml_->GetValue(&val, &size))
    {
        throw error(error::internalErr);    
    }
    if (size)
        return std::wstring(&val[0], &val[size]);
    else
        return L"";

}

// <element attribute="123"></element> Ч false
// <element attribute="123"/> - true
bool SaxWin32XmlLite::isEmptyElement()
{
    return (xml_->IsEmptyElement() == TRUE);    
}

bool SaxWin32XmlLite::isEof()
{
    return (xml_->IsEOF() == TRUE);
}

bool SaxWin32XmlLite::moveToAttrByName(const wchar_t * Name, const wchar_t * nsURI)
{
    HRESULT hr;
    if FAILED(hr = xml_->MoveToAttributeByName(Name, nsURI))
    {
        throw error(error::internalErr);    
    }

    return (S_FALSE != hr);
}

bool SaxWin32XmlLite::moveToAttrFirst()
{
    HRESULT hr;
    if FAILED(hr = xml_->MoveToFirstAttribute())
    {
        throw error(error::internalErr);    
    }

    return (S_FALSE != hr);
}

bool SaxWin32XmlLite::moveToAttrNext()
{
    HRESULT hr;
    if FAILED(hr = xml_->MoveToNextAttribute())
    {
        throw error(error::internalErr);    
    }

    return (S_FALSE != hr);
}

void SaxWin32XmlLite::moveToAttrOwner()
{
    HRESULT hr;
    if FAILED(hr = xml_->MoveToElement())
    {
        throw error(error::internalErr);    
    }
}

bool SaxWin32XmlLite::attrDefault()
{
    return (FALSE != xml_->IsDefault());
}

NodeType SaxWin32XmlLite::next()
{
    HRESULT hr;
    XmlNodeType nodeType;
    if FAILED(hr = xml_->Read(&nodeType))
    {
        return typeEOF;   
    }
    if (S_FALSE == hr)
        return typeEOF;
    else
        return NodeTypeConvert(nodeType);
}

unsigned int SaxWin32XmlLite::lineNumber()
{
    HRESULT hr;
    UINT val = 0;
    if FAILED(hr = xml_->GetLineNumber(&val))
    {
        throw error(error::internalErr);    
    }
    return val;
}

unsigned int SaxWin32XmlLite::linePosition()
{
    HRESULT hr;
    UINT val = 0;
    if FAILED(hr = xml_->GetLinePosition(&val))
    {
        throw error(error::internalErr);    
    }
    return val;
}

unsigned int SaxWin32XmlLite::readChunk(wchar_t * const Buffer, unsigned int Size)
{
    UINT readed = 0;
    HRESULT hr;
    if FAILED(hr = xml_->ReadValueChunk(Buffer, Size, &readed))
    {
        throw error(error::internalErr);
    }
    return readed;
}


////////////////

sax_ptr create_sax_win32_xmllite(const wchar_t * FileName)
{
    return sax_ptr( new SaxWin32XmlLite(FileName) );
}

sax_ptr create_sax_win32_xmllite( shared_ptr< std::istream >::Type istreamPtr)
{
     return sax_ptr( new SaxWin32XmlLite(istreamPtr) );    
}

sax_ptr create_sax_win32_xmllite( std::istream & istreamVal )
{
     return sax_ptr( new SaxWin32XmlLite(istreamVal) );    
}

}
}
