/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
//
//#if defined(_WIN32) || defined(_WIN64)
//#else
//    #include "../../DesktopEditor/common/ASCVariant.h"
//    #include "../../Common/DocxFormat/Source/Base/ASCString.h"
//#endif

#include "../../DesktopEditor/Common/File.h"

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

	template <class T> class XMLAttribute
	{
	private:
		std::basic_string<T> m_Name;
		std::basic_string<T> m_Value;

	public:

		XMLAttribute()
		{

		}

		/*========================================================================================================*/

		XMLAttribute( const T* name ) : m_Name(name)
		{
		}

		/*========================================================================================================*/

		XMLAttribute( const T* name, const T* value ) :	m_Name(name), m_Value(value)
		{
		}

		/*========================================================================================================*/

		~XMLAttribute()
		{
		}

		/*========================================================================================================*/

		void SetValue( const T* value )
		{
			m_Value = std::basic_string<T>( value );
		}

		/*========================================================================================================*/

		std::basic_string<T> GetName() const
		{
			return m_Name;
		}

		/*========================================================================================================*/

		std::basic_string<T> GetValue() const
		{
			return m_Value;
		}

		/*========================================================================================================*/

		std::basic_string<T> GetXMLString()
		{
			std::basic_string<T> xmlString( L"" );

			xmlString += m_Name;
			xmlString += std::basic_string<T>( L"=\"" );
			xmlString += m_Value;
			xmlString += std::basic_string<T>( L"\"" );

			return xmlString;
		}
	};

	/*========================================================================================================
	class XMLElement
    ========================================================================================================*/

	template <class T> class XMLElement
	{
		typedef std::pair< std::basic_string<T>, std::basic_string<T> > AttributeValuePair;

	private:
		std::basic_string<T>									m_Name;
		std::basic_string<T>									m_ElementText;
		std::map<std::basic_string<T>, std::basic_string<T>>	m_AttributeMap;
        std::list<XMLElement<T>>								m_Elements;

        typedef typename std::list<XMLElement<T>>::iterator          ElementsIterator;
        typedef typename std::list<XMLElement<T>>::const_iterator    ElementsIteratorConst;

        typedef typename std::map<std::basic_string<T>, std::basic_string<T>>::iterator            AttMapIterator;
        typedef typename std::map<std::basic_string<T>, std::basic_string<T>>::const_iterator      AttMapIteratorConst;

    public:

		XMLElement()
		{

		}

		/*========================================================================================================*/

		XMLElement( const T* name ) : m_Name(name)
		{

		}

		/*========================================================================================================*/

		XMLElement( const T* prefix, const T* localName ) : m_Name( std::basic_string<T>( prefix ) + std::basic_string<T>( L":" ) + std::basic_string<T>( localName ) ), m_ElementText( L"" )
		{

		}

		/*========================================================================================================*/

		~XMLElement()
		{

		}

		/*========================================================================================================*/

		void AppendText( const T* text )
		{
			m_ElementText = std::basic_string<T>( text );
		}

		/*========================================================================================================*/

		void AppendTextSymbol( const T symbol )
		{
			m_ElementText += std::basic_string<T>( &symbol );
		}

		/*========================================================================================================*/

		void AppendAttribute( const XMLAttribute<T>& attribute )
		{
			AttributeValuePair p( attribute.GetName(), attribute.GetValue() );

			m_AttributeMap.insert( p );
		}

		/*========================================================================================================*/

		void AppendAttribute( const T* name, const T* value )
		{
			AttributeValuePair p( std::basic_string<T>( const_cast<T*>( name ) ), std::basic_string<T>( const_cast<T*>( value ) ) );

			m_AttributeMap.insert( p );
		}

		/*========================================================================================================*/

		void AppendChild( const XMLElement<T>& element )
		{
			m_Elements.push_back( element );
		}

		/*========================================================================================================*/

		void RemoveChild( const XMLElement<T>& element )
		{
			m_Elements.remove( element );
		}

		/*========================================================================================================*/

		bool FindChild( const XMLElement<T>& element )
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

		/*========================================================================================================*/

		bool FindChildByName( const T* elementName ) const
		{
			bool result = false;

            for ( ElementsIterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++ )
			{
				if ( iter->m_Name == std::basic_string<T>( elementName ) )
				{
					result = true;

					break;
				}
			}

			return result;
		}

		/*========================================================================================================*/

		bool RemoveChildByName( const std::basic_string<T>& elementName )
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

		/*========================================================================================================*/

		bool operator == ( const XMLElement<T>& element ) const
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
				ElementsIteratorConst        thisIter    = m_Elements.begin();
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

        std::basic_string<T> GetName() const
		{
			return m_Name;
		}

		/*========================================================================================================*/

        std::basic_string<T> GetXMLString()
		{
			std::basic_string<T> xmlString( L"");

            bool bIsNameExists = ( m_Name != std::basic_string<T>( L"") );
            bool bIsTextExists = ( m_ElementText != std::basic_string<T>( L"") );

			if ( bIsNameExists )
			{
                xmlString += std::basic_string<T>( L"<" ) + m_Name;
			}

            if ( ( bIsNameExists ) && ( m_AttributeMap.size() > 0 ) )
			{
                for ( AttMapIterator iter = m_AttributeMap.begin(); iter != m_AttributeMap.end(); iter++ )
				{
					xmlString += std::basic_string<T>( L" " );
					xmlString += iter->first;
					xmlString += std::basic_string<T>( L"=\"" );
					xmlString += iter->second;
					xmlString += std::basic_string<T>( L"\"" );
				}
			}

			if ( ( m_Elements.size() > 0 ) || ( bIsTextExists ) )
			{
				if ( bIsNameExists )
				{
					xmlString += std::basic_string<T>( L">" );
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
                    xmlString += std::basic_string<T>( L"</" );
					xmlString += m_Name;
					xmlString += std::basic_string<T>( L">" );
				}
			}
			else
			{
				if ( bIsNameExists )
				{
					xmlString += std::basic_string<T>( L"/>" );
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
		bool SaveToFile(const std::wstring& strFilePath, bool bEncodingToUTF8 = false)
		{
			NSFile::CFileBinary file;
			if (!file.CreateFileW(strFilePath)) return false;

			if (bEncodingToUTF8)
				file.WriteStringUTF8(m_str);
			else
			{
				std::string s(m_str.begin(), m_str.end());
				file.WriteFile((unsigned char*)s.c_str(), s.length());
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
			wchar_t str[33]={};
            _itow(Value, str, Base);
			m_str += str;
		}
		void WriteDouble(double Value)
		{
			int *dec = NULL, *sign = NULL;
			char *str = _fcvt( Value , 4, dec, sign);

			if (str)
			{
				std::string sA(str);
				delete []str;	
				m_str += std::wstring(sA.begin(), sA.end());
			}
			else
			{
				m_str += L"0";
			}
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
