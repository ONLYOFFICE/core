/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include <string>

#include "../../../OOXML/Base/Nullable.h"

#include "../../../DesktopEditor/xml/include/xmlutils.h"

#include "sax.h"

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
		case XmlUtils::XmlNodeType_SIGNIFICANT_WHITESPACE:
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

    return XmlUtils::GetNameNoNS(xml_->GetName());
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

    return xml_->GetNamespacePrefix();
}

NodeType saxXmlLiteReader::nodeType()
{
	if (xml_.IsInit() == false) return typeNone;	

    XmlUtils::XmlNodeType nTempType = xml_->GetNodeType();
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
	
    return xml_->IsDefaultAttribute();
}

NodeType saxXmlLiteReader::next(int Depth)
{
	if (xml_->ReadNextSiblingNode2(Depth))
	{
        XmlUtils::XmlNodeType nTempType = xml_->GetNodeType();
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
