/// \file   sax_xmllite.cpp
/// \brief  –еализаци€ интерфейса SAX через библиотеку XmlLite

#include "precompiled_cpxml.h"

#include <string>


#include "../../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../../../common/docxformat/source/xml/libxml2/libxml2.h"


#include "../../include/cpdoccore/xml/sax.h"

#include "sax_xmllite.h"
//#include "istream2Win32Stream.h"

namespace cpdoccore {
namespace xml {

class saxXmlLiteReader : public sax
{
public:
    saxXmlLiteReader(const wchar_t * FileName);
    //saxXmlLiteReader(shared_ptr< std::istream >::Type istreamPtr); 
    //saxXmlLiteReader(std::istream & istreamVal); 
    saxXmlLiteReader();
    
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

saxXmlLiteReader::saxXmlLiteReader(const wchar_t * FileName)
{
	xml_ = new XmlUtils::CXmlLiteReader();
	
	xml_->FromFile(FileName);

	if (xml_->ReadNextNode() == false)
	{
		xml_ = NULL;
	}
}

unsigned int saxXmlLiteReader::attrCount()
{
	if (xml_.IsInit() == false) return 0;

    return xml_->GetAttributesCount();
}


unsigned int saxXmlLiteReader::depth()
{
 	if (xml_.IsInit() == false) return 0;

    return xml_->GetDepth();
}

// <p:abc/>, returns "abc".
std::wstring saxXmlLiteReader::nodeLocalName()
{
	if (xml_.IsInit() == false) return _T("");

	return XmlUtils::GetNameNoNS(xml_->GetName()).GetBuffer();
}

// <p:abc />, returns "p:abc".
std::wstring saxXmlLiteReader::nodeQualifiedName()
{
	if (xml_.IsInit() == false) return _T("");
	return xml_->GetName();
}

// <xyz:abc xmlns:xyz="u://1" />, it returns "xyz".
std::wstring saxXmlLiteReader::namespacePrefix()
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
//std::wstring saxXmlLiteReader::namespaceUri()
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

NodeType saxXmlLiteReader::nodeType()
{
	if (xml_.IsInit() == false) return typeNone;	

	XmlUtils::XmlNodeType nTempType = (XmlUtils::XmlNodeType)xmlTextReaderNodeType(xml_->getNativeReader());
	return NodeTypeConvert(nTempType); 
}

std::wstring saxXmlLiteReader::value()
{
 	if (xml_.IsInit() == false) return _T("");
    
	return xml_->GetText3();

}

// <element attribute="123"></element> Ч false
// <element attribute="123"/> - true
bool saxXmlLiteReader::isEmptyElement()
{
	if (xml_.IsInit() == false) return false;

    return xml_->IsEmptyNode();    
}

bool saxXmlLiteReader::isEof()
{
 	if (xml_.IsInit() == false) return true;
	
	return false;
}

bool saxXmlLiteReader::moveToAttrFirst()
{
	if (xml_.IsInit() == false) return false;

    if (S_OK != xml_->MoveToFirstAttribute())
    {
        throw error(error::internalErr);    
    }

    return true;
}

bool saxXmlLiteReader::moveToAttrNext()
{
	if (xml_.IsInit() == false) return false;

    if (S_OK !=xml_->MoveToNextAttribute())
    {
        throw error(error::internalErr);    
    }

    return true;
}

void saxXmlLiteReader::moveToAttrOwner()
{
	if (xml_.IsInit() == false) return;

	if (S_OK != xml_->MoveToElement())
    {
        throw error(error::internalErr);    
    }
}

bool saxXmlLiteReader::attrDefault()
{
	if (xml_.IsInit() == false) return false;
	
    return (FALSE != xmlTextReaderIsDefault(xml_->getNativeReader()));
}

NodeType saxXmlLiteReader::next()
{
	int nCurDepth = xml_->GetDepth();
	
	if (xml_->ReadNextSiblingNode(nCurDepth))
	{
		XmlUtils::XmlNodeType nTempType = (XmlUtils::XmlNodeType)xmlTextReaderNodeType(xml_->getNativeReader());
		return NodeTypeConvert(nTempType);
	}
	else 
		return typeNone;

	
}
//unsigned int saxXmlLiteReader::readChunk(wchar_t * const Buffer, unsigned int Size)
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

sax_ptr create_sax_xmllite(const wchar_t * FileName)
{
    return sax_ptr( new saxXmlLiteReader(FileName) );
}

//sax_ptr create_sax_xmllite( shared_ptr< std::istream >::Type istreamPtr)
//{
//     return sax_ptr( new saxXmlLiteReader(istreamPtr) );    
//}
//
//sax_ptr create_sax_xmllite( std::istream & istreamVal )
//{
//     return sax_ptr( new saxXmlLiteReader(istreamVal) );    
//}

}
}
