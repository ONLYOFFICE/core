/// \file   sax_win32_xmllite.cpp
/// \brief  –еализаци€ интерфейса SAX через библиотеку XmlLite

#include "precompiled_cpxml.h"

#include <string>


#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

//#include <atlbase.h>
//#include <atlcom.h>
//#include <atlwin.h>
//#include <atltypes.h>
//#include <atlctl.h>
//#include <atlhost.h>

//using namespace ATL;

#include "../../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../../../common/docxformat/source/xml/libxml2/libxml2.h"


#include "../../include/cpdoccore/xml/sax.h"

#include "sax_win32_xmllite.h"
//#include "istream2Win32Stream.h"

namespace cpdoccore {
namespace xml {

class SaxWin32XmlLite : public sax
{
public:
    SaxWin32XmlLite(const wchar_t * FileName);
    //SaxWin32XmlLite(shared_ptr< std::istream >::Type istreamPtr); 
    //SaxWin32XmlLite(std::istream & istreamVal); 
    SaxWin32XmlLite();
    
public:
    virtual unsigned int attrCount();
    //virtual std::wstring baseURI();
    virtual unsigned int depth();

    // <p:abc/>, returns "abc".
    virtual std::wstring nodeLocalName();
    
    // <p:abc />, returns "p:abc".
    virtual std::wstring nodeQualifiedName();

    // <xyz:abc xmlns:xyz="u://1" />, it returns "xyz".
    virtual std::wstring namespacePrefix();

    //// Returns "u://1" for the element <xyz:abc xmlns:xyz="u://1" />.
    //virtual std::wstring namespaceUri();

    virtual NodeType nodeType();
    virtual std::wstring value();

    // <element attribute="123"></element> Ч false
    // <element attribute="123"/> - true
    virtual bool isEmptyElement();
    
    virtual bool isEof();

    //virtual bool moveToAttrByName(const wchar_t * Name, const wchar_t * nsURI);
    virtual bool moveToAttrFirst();
    virtual bool moveToAttrNext();
    virtual void moveToAttrOwner();
    virtual bool attrDefault();

    virtual NodeType next();
    
    //virtual unsigned int readChunk(wchar_t * Buffer, unsigned int Size);

private:
	smart_ptr< XmlUtils::CXmlLiteReader > xml_;

};

namespace 
{
    NodeType NodeTypeConvert(XmlUtils::XmlNodeType Type)
    {
        switch(Type)
        {
        case XmlUtils::XmlNodeType_None:
            return typeNone;
        case XmlUtils::XmlNodeType_Element:
            return typeElement;
        case XmlUtils::XmlNodeType_Attribute:
            return typeAttribute;
        case XmlUtils::XmlNodeType_Text:
            return typeText;
		case XmlUtils::XmlNodeType_CDATA:
            return typeCDATA;
        case XmlUtils::XmlNodeType_ProcessingInstruction:
            return typeProcessingInstruction;
        case XmlUtils::XmlNodeType_Comment:
            return typeComment;
        case XmlUtils::XmlNodeType_DocumentType:
            return typeDocumentType;
        case XmlUtils::XmlNodeType_Whitespace:
            return typeWhitespace;
        case XmlUtils::XmlNodeType_EndElement:
            return typeEndElement;
        case XmlUtils::XmlNodeType_XmlDeclaration:
            return typeXmlDeclaration;
        default:
            return typeNone;
        }
    }
}

SaxWin32XmlLite::SaxWin32XmlLite(const wchar_t * FileName)
{
	xml_ = new XmlUtils::CXmlLiteReader();
	
	xml_->FromFile(FileName);
}

//SaxWin32XmlLite::SaxWin32XmlLite(shared_ptr< std::istream >::Type istreamPtr)
//{
//    HRESULT hr;
//    ::ATL::CComPtr< IStream > stream = istreamToWin32IStream(istreamPtr);
//
//    if (FAILED(hr = ::CreateXmlReader(__uuidof(IXmlReader), (void**)&xml_, NULL)))
//    {
//        //wprintf(L"Error creating xml reader, error is %08.8lx", hr);
//        throw error( error::createReader );
//    }
//
//    if FAILED(xml_->SetInput(stream))
//    {
//        throw error( error::createReader );
//    };
//    
//}
//
//SaxWin32XmlLite::SaxWin32XmlLite(std::istream & istreamVal)
//{
//   HRESULT hr;
//    ::ATL::CComPtr< IStream > stream = istreamToWin32IStream(istreamVal);
//
//    if (FAILED(hr = ::CreateXmlReader(__uuidof(IXmlReader), (void**)&xml_, NULL)))
//    {
//        //wprintf(L"Error creating xml reader, error is %08.8lx", hr);
//        throw error( error::createReader );
//    }
//
//    if FAILED(xml_->SetInput(stream))
//    {
//        throw error( error::createReader );
//    };    
//}
//
unsigned int SaxWin32XmlLite::attrCount()
{
	if (xml_.IsInit() == false) return 0;

    return xml_->GetAttributesCount();
}

//std::wstring SaxWin32XmlLite::baseURI()
//{
//    HRESULT hr;
//    UINT size = 0;
//    const wchar_t * val;
//    if FAILED(hr = xml_->GetBaseUri(&val, &size))
//    {
//        throw error(error::internalErr);    
//    }
//    if (size)
//        return std::wstring(&val[0], &val[size]);
//    else
//        return L"";
//
//}

unsigned int SaxWin32XmlLite::depth()
{
 	if (xml_.IsInit() == false) return 0;

    return xml_->GetDepth();
}

// <p:abc/>, returns "abc".
std::wstring SaxWin32XmlLite::nodeLocalName()
{
	if (xml_.IsInit() == false) return _T("");

	return XmlUtils::GetNameNoNS(xml_->GetName()).GetBuffer();
}

// <p:abc />, returns "p:abc".
std::wstring SaxWin32XmlLite::nodeQualifiedName()
{
	if (xml_.IsInit() == false) return _T("");
	return xml_->GetName();
}

// <xyz:abc xmlns:xyz="u://1" />, it returns "xyz".
std::wstring SaxWin32XmlLite::namespacePrefix()
{
	if (xml_.IsInit() == false) return _T("");

	xmlChar* pName = xmlTextReaderPrefix(xml_->getNativeReader());
	if (NULL == pName)
		return L"";

	std::wstring sTemp = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pName, (LONG)strlen((const char*)pName));
	free(pName);
	return sTemp;
}
//
//// Returns "u://1" for the element <xyz:abc xmlns:xyz="u://1" />.
//std::wstring SaxWin32XmlLite::namespaceUri()
//{
//    HRESULT hr;
//    UINT size = 0;
//    const wchar_t * val;
//    if FAILED(hr = xml_->GetNamespaceUri(&val, &size))
//    {
//        throw error(error::internalErr);    
//    }
//    if (size)
//        return std::wstring(&val[0], &val[size]);
//    else
//        return L"";
//
//}

NodeType SaxWin32XmlLite::nodeType()
{
	if (xml_.IsInit() == false) return typeNone;	

	XmlUtils::XmlNodeType nTempType = (XmlUtils::XmlNodeType)xmlTextReaderNodeType(xml_->getNativeReader());
	return NodeTypeConvert(nTempType); 
}

std::wstring SaxWin32XmlLite::value()
{
 	if (xml_.IsInit() == false) return _T("");
    
	return xml_->GetText3();

}

// <element attribute="123"></element> Ч false
// <element attribute="123"/> - true
bool SaxWin32XmlLite::isEmptyElement()
{
	if (xml_.IsInit() == false) return false;

    return (xml_->IsEmptyNode() == TRUE);    
}

bool SaxWin32XmlLite::isEof()
{
 	if (xml_.IsInit() == false) return true;
	
	return false;
}

bool SaxWin32XmlLite::moveToAttrFirst()
{
	if (xml_.IsInit() == false) return false;

	HRESULT hr;
    if FAILED(hr = xml_->MoveToFirstAttribute())
    {
        throw error(error::internalErr);    
    }

    return (S_FALSE != hr);
}

bool SaxWin32XmlLite::moveToAttrNext()
{
	if (xml_.IsInit() == false) return false;

	HRESULT hr;
    if FAILED(hr = xml_->MoveToNextAttribute())
    {
        throw error(error::internalErr);    
    }

    return (S_FALSE != hr);
}

void SaxWin32XmlLite::moveToAttrOwner()
{
	if (xml_.IsInit() == false) return;
    HRESULT hr;
    if FAILED(hr = xml_->MoveToElement())
    {
        throw error(error::internalErr);    
    }
}

bool SaxWin32XmlLite::attrDefault()
{
	if (xml_.IsInit() == false) return false;
	
    return (FALSE != xmlTextReaderIsDefault(xml_->getNativeReader()));
}

NodeType SaxWin32XmlLite::next()
{
    HRESULT hr;
    NodeType nodeType;
				
	int nCurDepth = xml_->GetDepth();
	if (xml_->ReadNextSiblingNode(nCurDepth))
	{
		XmlUtils::XmlNodeType nTempType = (XmlUtils::XmlNodeType)xmlTextReaderNodeType(xml_->getNativeReader());
		return NodeTypeConvert(nTempType);
	}
	else 
		return typeNone;

	
}
//unsigned int SaxWin32XmlLite::readChunk(wchar_t * const Buffer, unsigned int Size)
//{
// 	if (xml_.IsInit() == false) return 0;
//
//	
//	XmlUtils::XmlNodeType nTempType = (XmlUtils::XmlNodeType)xmlTextReaderNodeType(xml_->getNativeReader());
//	
//	if FAILED(hr = xml_->ReadValueChunk(Buffer, Size, &readed))
//    {
//        throw error(error::internalErr);
//    }
//    return readed;
//}
//
//
////////////////

sax_ptr create_sax_win32_xmllite(const wchar_t * FileName)
{
    return sax_ptr( new SaxWin32XmlLite(FileName) );
}

//sax_ptr create_sax_win32_xmllite( shared_ptr< std::istream >::Type istreamPtr)
//{
//     return sax_ptr( new SaxWin32XmlLite(istreamPtr) );    
//}
//
//sax_ptr create_sax_win32_xmllite( std::istream & istreamVal )
//{
//     return sax_ptr( new SaxWin32XmlLite(istreamVal) );    
//}

}
}
