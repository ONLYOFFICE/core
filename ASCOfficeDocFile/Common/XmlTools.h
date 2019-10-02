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
#pragma once

#include <string>
#include <map>
#include <list>

#include <boost/shared_ptr.hpp>

#include "../../DesktopEditor/common/File.h"

inline static std::wstring ReplaceString(std::wstring subject, const std::wstring& search, const std::wstring& replace) 
{
	size_t pos = 0;

	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}

	return subject;
}
//----------------------------------------------------------------
namespace XMLTools
{
	/*========================================================================================================
	class XMLAttribute
	========================================================================================================*/

    class XMLAttribute
	{
	private:
        std::wstring m_Name;
        std::wstring m_Value;

	public:

		XMLAttribute()
		{

		}
        XMLAttribute( const std::wstring & name ) : m_Name(name)
		{
		}
        XMLAttribute( const std::wstring & name, const std::wstring & value ) :	m_Name(name), m_Value(value)
		{
        }
        ~XMLAttribute()
		{
		}
        void SetValue( const std::wstring & value )
		{
            m_Value = std::wstring( value );
		}
        std::wstring GetName() const
		{
			return m_Name;
		}
		/*========================================================================================================*/

        std::wstring GetValue() const
		{
			return m_Value;
		}

		/*========================================================================================================*/

        std::wstring GetXMLString()
		{
            std::wstring xmlString( L"" );

			xmlString += m_Name;
            xmlString += std::wstring( L"=\"" );
			xmlString += m_Value;
            xmlString += std::wstring( L"\"" );

			return xmlString;
		}
	};

	/*========================================================================================================
	class XMLElement
    ========================================================================================================*/

	class XMLElement;
	typedef class boost::shared_ptr<XMLElement> XMLElementPtr;

	class XMLElement
	{
        typedef std::pair< std::wstring, std::wstring> AttributeValuePair;

	private:
        std::wstring									m_Name;
        std::wstring									m_ElementText;
        std::map<std::wstring, std::wstring>            m_AttributeMap;
        std::map<std::wstring, int>						m_ChildMap; //for uniq
        std::list<XMLElement>                           m_Elements;

        typedef std::list<XMLElement>::iterator          ElementsIterator;
        typedef std::list<XMLElement>::const_iterator    ElementsIteratorConst;

        typedef std::map<std::wstring, std::wstring>::iterator            AttMapIterator;
        typedef std::map<std::wstring, std::wstring>::const_iterator      AttMapIteratorConst;

    public:

        XMLElement() {}

        XMLElement( const std::wstring & name ) : m_Name(name)
		{

		}
        XMLElement( const std::wstring & prefix, const std::wstring & localName ) :
            m_Name( std::wstring( prefix ) + std::wstring( L":" ) + std::wstring( localName ) ), m_ElementText( L"" )
		{

		}
        ~XMLElement() {}

        void AppendText( const std::wstring & text )
		{
            m_ElementText = std::wstring( text );
		}

        void AppendTextSymbol( const wchar_t symbol )
		{
            m_ElementText += std::wstring( &symbol );
		}

        void AppendAttribute( const XMLAttribute& attribute )
		{
			AttributeValuePair p( attribute.GetName(), attribute.GetValue() );

			m_AttributeMap.insert( p );
		}

        void AppendAttribute( const std::wstring & name, const std::wstring & value )
		{
            AttributeValuePair p(  name , value  );

			m_AttributeMap.insert( p );
		}

        void AppendChild( const XMLElement& element, bool uniq = false)
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
        void AppendChild( XMLElementPtr element, bool uniq = false)
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
        void RemoveChild( const XMLElement& element )
		{
			m_Elements.remove( element );
		}

        bool FindChild( const XMLElement& element )
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

        bool FindChildByName( const std::wstring & elementName ) const
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

        bool RemoveChildByName( const std::wstring& elementName )
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

        bool operator == ( const XMLElement& element ) const
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

		/*========================================================================================================*/

        std::wstring GetName() const
		{
			return m_Name;
		}

		/*========================================================================================================*/

        std::wstring GetXMLString()
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

		/*========================================================================================================*/

		unsigned int GetAttributeCount() const
		{
			return (unsigned int)m_AttributeMap.size();
		}

		/*========================================================================================================*/

		unsigned int GetChildCount() const
		{
			return (unsigned int)m_Elements.size();
		}
	};

	class CStringXmlWriter
	{
		std::wstring m_str;
	
	public:
		CStringXmlWriter(){}
		std::wstring GetXmlString()
		{
			return m_str;
		}
		void SetXmlString(const std::wstring& strValue)
		{
			m_str = strValue;
		}
		void Clear()
		{
			m_str.clear();
		}
		bool SaveToFile(const std::wstring& strFilePath, bool bEncodingToUTF8 = false)
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

		void WriteString(const std::wstring & strValue)
		{
			m_str += strValue;
		}

		void WriteInteger(int Value, int Base = 10)
		{
            m_str += std::to_wstring(Value);
		}

		void WriteDouble(double Value)
		{
            m_str += std::to_wstring(Value);
        }
		void WriteBoolean(bool Value)
		{
			if (Value)
				m_str += L"true";
			else
				m_str += L"false";
		}		
		void WriteNodeBegin(const std::wstring& strNodeName, bool bAttributed = false)
		{
			m_str += L"<" + strNodeName;

			if (!bAttributed)
				m_str += L">";
		}
		void WriteNodeEnd(const std::wstring& strNodeName, bool bEmptyNode = false, bool bEndNode = true)
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
		void WriteNode(const std::wstring& strNodeName, const std::wstring& strNodeValue)
		{
			if (strNodeValue.length() == 0)
				m_str += L"<" + strNodeName + L"/>";
			else
				m_str += L"<" + strNodeName + L">" + strNodeValue + L"</" + strNodeName + L">";
		}
		void WriteNode(const std::wstring& strNodeName, int nValue, int nBase = 10, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = L"")
		{
			WriteNodeBegin(strNodeName);
				WriteString(strTextBeforeValue);
				WriteInteger(nValue, nBase);
				WriteString(strTextAfterValue);
			WriteNodeEnd(strNodeName);
		}
		void WriteNode(const std::wstring& strNodeName, double dValue)
		{
			WriteNodeBegin(strNodeName);
				WriteDouble(dValue);
			WriteNodeEnd(strNodeName);
		}
		void WriteAttribute(const std::wstring& strAttributeName, const std::wstring& strAttributeValue)
		{
			m_str += L" " + strAttributeName + L"=\"" + strAttributeValue + L"\"";
		}
		void WriteAttribute(const std::wstring& strAttributeName, int nValue, int nBase = 10, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = L"")
		{
			WriteString(L" " + strAttributeName + L"=");
			WriteString(L"\"");
			WriteString(strTextBeforeValue);
			WriteInteger(nValue, nBase);
			WriteString(strTextAfterValue);
			WriteString(L"\"");
		}
		void WriteAttribute(const std::wstring& strAttributeName, double dValue)
		{
			WriteString(L" " + strAttributeName + L"=");
			WriteString(L"\"");
			WriteDouble(dValue);
			WriteString(L"\"");
		}
	};


}
