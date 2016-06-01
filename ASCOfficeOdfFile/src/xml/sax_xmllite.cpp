#include <string>

#include "../../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../../../Common/DocxFormat/Source/XML/libxml2/libxml2.h"


#include "../../include/cpdoccore/xml/sax.h"

#include "sax_xmllite.h"

namespace cpdoccore {
namespace xml {

class saxXmlLiteReader : public sax
{
public:
    saxXmlLiteReader(const wchar_t * FileName);
    saxXmlLiteReader();
    
public:
    virtual unsigned int depth();

    virtual std::wstring nodeLocalName();
    
    virtual std::wstring nodeQualifiedName();

    virtual std::wstring namespacePrefix();

    virtual NodeType		nodeType();
    virtual std::wstring	value();
   
    virtual unsigned int	attrCount();
	virtual bool			attrDefault();

    virtual bool isEmptyElement();
    
    virtual bool moveToAttrFirst();
    virtual bool moveToAttrNext();
    virtual bool moveToAttrOwner();

    virtual NodeType next(int Depth);
    
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

	bool res;

	res = xml_->ReadNextNode();
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

NodeType saxXmlLiteReader::nodeType()
{
	if (xml_.IsInit() == false) return typeNone;	

	XmlUtils::XmlNodeType nTempType = (XmlUtils::XmlNodeType)xmlTextReaderNodeType(xml_->getNativeReader());
	return NodeTypeConvert(nTempType); 
}

std::wstring saxXmlLiteReader::value()
{
 	if (xml_.IsInit() == false) return _T("");
    
	return xml_->GetText();

}

bool saxXmlLiteReader::isEmptyElement()
{
	if (xml_.IsInit() == false) return false;

    return xml_->IsEmptyNode();    
}

bool saxXmlLiteReader::moveToAttrFirst()
{
	if (xml_.IsInit() == false) return false;

    return xml_->MoveToFirstAttribute();
}

bool saxXmlLiteReader::moveToAttrNext()
{
	if (xml_.IsInit() == false) return false;

    return xml_->MoveToNextAttribute();
}

bool saxXmlLiteReader::moveToAttrOwner()
{
	if (xml_.IsInit() == false) return false;

	return xml_->MoveToElement();
}

bool saxXmlLiteReader::attrDefault()
{
	if (xml_.IsInit() == false) return false;
	
    return (FALSE != xmlTextReaderIsDefault(xml_->getNativeReader()));
}

NodeType saxXmlLiteReader::next(int Depth)
{
	if (xml_->ReadNextSiblingNode2(Depth))
	{
		XmlUtils::XmlNodeType nTempType = (XmlUtils::XmlNodeType)xmlTextReaderNodeType(xml_->getNativeReader());
		return NodeTypeConvert(nTempType);
	}
	else 
		return typeNone;

	
}

sax_ptr create_sax_xmllite(const wchar_t * FileName)
{
    return sax_ptr( new saxXmlLiteReader(FileName) );
}

}
}
