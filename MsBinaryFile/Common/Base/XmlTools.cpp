/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "XmlTools.h"

namespace XMLTools
{
	/*========================================================================================================
	class XMLAttribute
	========================================================================================================*/

	XMLAttribute::XMLAttribute()
	{
	}
	XMLAttribute::XMLAttribute( const std::wstring & name ) : m_Name(name)
	{
	}
	XMLAttribute::XMLAttribute( const std::wstring & name, const std::wstring & value ) :	m_Name(name), m_Value(value)
	{
	}
	XMLAttribute::~XMLAttribute()
	{
	}
	void XMLAttribute::SetValue( const std::wstring & value )
	{
		m_Value = std::wstring( value );
	}
	std::wstring XMLAttribute::GetName() const
	{
		return m_Name;
	}
	std::wstring XMLAttribute::GetValue() const
	{
		return m_Value;
	}
	std::wstring XMLAttribute::GetXMLString()
		{
			std::wstring xmlString( L"" );

			xmlString += m_Name;
			xmlString += std::wstring( L"=\"" );
			xmlString += m_Value;
			xmlString += std::wstring( L"\"" );

			return xmlString;
		}

	/*========================================================================================================
	class XMLElement
	========================================================================================================*/

	XMLElement::XMLElement() {}
	XMLElement::XMLElement( const std::wstring & name ) : m_Name(name)
	{
	}
	XMLElement::XMLElement( const std::wstring & prefix, const std::wstring & localName ) :
		m_Name( std::wstring( prefix ) + std::wstring( L":" ) + std::wstring( localName ) ), m_ElementText( L"" )
	{
	}
	XMLElement::~XMLElement() {}
	void XMLElement::AppendText( const std::wstring & text )
	{
		m_ElementText = std::wstring( text );
	}
	void XMLElement::AppendTextSymbol( const wchar_t symbol )
	{
		m_ElementText += std::wstring( &symbol );
	}
	void XMLElement::AppendAttribute( const XMLAttribute& attribute )
	{
		AttributeValuePair p( attribute.GetName(), attribute.GetValue() );

		m_AttributeMap.insert( p );
	}
	void XMLElement::AppendAttribute( const std::wstring & name, const std::wstring & value )
	{
		AttributeValuePair p(  name , value  );

		m_AttributeMap.insert( p );
	}
	void XMLElement::AppendChild( const XMLElement& element, bool uniq)
	{
		if (m_ChildMap.find(element.GetName()) != m_ChildMap.end())
		{
			if (uniq) return;
		}
		else
		{
			m_ChildMap.insert(m_ChildMap.end(), std::pair<std::wstring, int>(element.GetName(), 0));
		}
		m_Elements.push_back( element );
	}
	void XMLElement::AppendChild( XMLElementPtr element, bool uniq)
	{
		if (!element) return;

		if (m_ChildMap.find(element->GetName()) != m_ChildMap.end())
		{
			if (uniq) return;
		}
		else
		{
			m_ChildMap.insert(m_ChildMap.end(), std::pair<std::wstring, int>(element->GetName(), 0));
		}
		m_Elements.push_back( *element.get() );
	}
	void XMLElement::RemoveChild( const XMLElement& element )
	{
		m_Elements.remove( element );
	}
	bool XMLElement::FindChild( const XMLElement& element )
	{
		bool result = false;

		for ( ElementsIterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++ )
		{
			if ( *iter == element )
			{
				result = true;

				break;
			}
		}

		return result;
	}
	bool XMLElement::FindChildByName( const std::wstring & elementName ) const
	{
		bool result = false;

		for ( ElementsIteratorConst iter = m_Elements.begin(); iter != m_Elements.end(); iter++ )
		{
			if ( iter->m_Name == std::wstring( elementName ) )
			{
				result = true;

				break;
			}
		}

		return result;
	}
	bool XMLElement::RemoveChildByName( const std::wstring& elementName )
	{
		bool result = false;

		for ( ElementsIterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++ )
		{
			if ( iter->m_Name == elementName )
			{
				m_Elements.erase( iter );

				result = true;

				break;
			}
		}

		return result;
	}
	bool XMLElement::operator == ( const XMLElement& element ) const
	{
		bool result = false;

		result = ( m_Name == element.m_Name );

		if ( m_AttributeMap.size() != element.m_AttributeMap.size() )
		{
			result = false;
		}
		else
		{

			AttMapIteratorConst  thisIter = m_AttributeMap.begin();
			AttMapIteratorConst elementIter = element.m_AttributeMap.begin();

			for ( ; thisIter != m_AttributeMap.end(); thisIter++, elementIter++ )
			{
				if ( ( thisIter->first != elementIter->first ) || ( thisIter->second != elementIter->second ) )
				{
					result = false;
				}
			}
		}

		if ( m_Elements.size() != element.m_Elements.size() )
		{
			result = false;
		}
		else
		{
			ElementsIteratorConst  thisIter    = m_Elements.begin();
			ElementsIteratorConst  elementIter = element.m_Elements.begin();

			for ( ; thisIter != m_Elements.end(); thisIter++, elementIter++ )
			{
				if ( !( (*thisIter) == (*elementIter) ) )
				{
					result = false;
				}
			}
		}

		return result;
	}
	std::wstring XMLElement::GetName() const
	{
		return m_Name;
	}
	std::wstring XMLElement::GetXMLString()
	{
		std::wstring xmlString( L"");

		bool bIsNameExists = ( m_Name != std::wstring( L"") );
		bool bIsTextExists = ( m_ElementText != std::wstring( L"") );

		if ( bIsNameExists )
		{
			xmlString += std::wstring( L"<" ) + m_Name;
		}

		if ( ( bIsNameExists ) && ( m_AttributeMap.size() > 0 ) )
		{
			for ( AttMapIterator iter = m_AttributeMap.begin(); iter != m_AttributeMap.end(); iter++ )
			{
				xmlString += std::wstring( L" " );
				xmlString += iter->first;
				xmlString += std::wstring( L"=\"" );
				xmlString += iter->second;
				xmlString += std::wstring( L"\"" );
			}
		}

		if ( ( m_Elements.size() > 0 ) || ( bIsTextExists ) )
		{
			if ( bIsNameExists )
			{
				xmlString += std::wstring( L">" );
			}

			for ( ElementsIterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++ )
			{
				xmlString += iter->GetXMLString();
			}

			if ( bIsTextExists )
			{
				xmlString += m_ElementText;
			}

			if ( bIsNameExists )
			{
				xmlString += std::wstring( L"</" );
				xmlString += m_Name;
				xmlString += std::wstring( L">" );
			}
		}
		else
		{
			if ( bIsNameExists )
			{
				xmlString += std::wstring( L"/>" );
			}
		}

		return xmlString;
	}
	unsigned int XMLElement::GetAttributeCount() const
	{
		return (unsigned int)m_AttributeMap.size();
	}
	unsigned int XMLElement::GetChildCount() const
	{
		return (unsigned int)m_Elements.size();
	}

	CStringXmlWriter::CStringXmlWriter(){}
	std::wstring CStringXmlWriter::GetXmlString()
	{
		return m_str;
	}
	void CStringXmlWriter::SetXmlString(const std::wstring& strValue)
	{
		m_str = strValue;
	}
	void CStringXmlWriter::Clear()
	{
		m_str.clear();
	}
	bool CStringXmlWriter::SaveToFile(const std::wstring& strFilePath, bool bEncodingToUTF8)
	{
		NSFile::CFileBinary file;
		if (!file.CreateFileW(strFilePath)) return false;

		if (bEncodingToUTF8)
			file.WriteStringUTF8(m_str);
		else
		{
			std::string s(m_str.begin(), m_str.end());
			file.WriteFile((unsigned char*)s.c_str(), (DWORD)s.length());
		}
		file.CloseFile();
		return true;
	}
	void CStringXmlWriter::WriteString(const std::wstring & strValue)
	{
		m_str += strValue;
	}
	void CStringXmlWriter::WriteInteger(int Value, int Base)
	{
		m_str += std::to_wstring(Value);
	}
	void CStringXmlWriter::WriteDouble(double Value)
	{
		m_str += std::to_wstring(Value);
	}
	void CStringXmlWriter::WriteBoolean(bool Value)
	{
		if (Value)
			m_str += L"true";
		else
			m_str += L"false";
	}
	void CStringXmlWriter::WriteNodeBegin(const std::wstring& strNodeName, bool bAttributed)
	{
		m_str += L"<" + strNodeName;

		if (!bAttributed)
			m_str += L">";
	}
	void CStringXmlWriter::WriteNodeEnd(const std::wstring& strNodeName, bool bEmptyNode, bool bEndNode)
	{
		if (bEmptyNode)
		{
			if (bEndNode)
				m_str += L"/>";
			else
				m_str += L">";
		}
		else
			m_str += L"</" + strNodeName + L">";
	}
	void CStringXmlWriter::WriteNode(const std::wstring& strNodeName, const std::wstring& strNodeValue)
	{
		if (strNodeValue.length() == 0)
			m_str += L"<" + strNodeName + L"/>";
		else
			m_str += L"<" + strNodeName + L">" + strNodeValue + L"</" + strNodeName + L">";
	}
	void CStringXmlWriter::WriteNode(const std::wstring& strNodeName, int nValue, int nBase, const std::wstring& strTextBeforeValue, const std::wstring& strTextAfterValue)
	{
		WriteNodeBegin(strNodeName);
			WriteString(strTextBeforeValue);
			WriteInteger(nValue, nBase);
			WriteString(strTextAfterValue);
		WriteNodeEnd(strNodeName);
	}
	void CStringXmlWriter::WriteNode(const std::wstring& strNodeName, double dValue)
	{
		WriteNodeBegin(strNodeName);
			WriteDouble(dValue);
		WriteNodeEnd(strNodeName);
	}
	void CStringXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const std::wstring& strAttributeValue)
	{
		m_str += L" " + strAttributeName + L"=\"" + strAttributeValue + L"\"";
	}
	void CStringXmlWriter::WriteAttribute(const std::wstring& strAttributeName, int nValue, int nBase, const std::wstring& strTextBeforeValue, const std::wstring& strTextAfterValue)
	{
		WriteString(L" " + strAttributeName + L"=");
		WriteString(L"\"");
		WriteString(strTextBeforeValue);
		WriteInteger(nValue, nBase);
		WriteString(strTextAfterValue);
		WriteString(L"\"");
	}
	void CStringXmlWriter::WriteAttribute(const std::wstring& strAttributeName, double dValue)
	{
		WriteString(L" " + strAttributeName + L"=");
		WriteString(L"\"");
		WriteDouble(dValue);
		WriteString(L"\"");
	}
}
