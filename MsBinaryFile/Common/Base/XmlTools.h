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
#include <unordered_map>
#include <list>

#include <boost/shared_ptr.hpp>

#include "../../../DesktopEditor/common/File.h"

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

		XMLAttribute();
		XMLAttribute( const std::wstring & name );
		XMLAttribute( const std::wstring & name, const std::wstring & value );
		~XMLAttribute();

		void SetValue( const std::wstring & value );
		std::wstring GetName() const;

		/*========================================================================================================*/

		std::wstring GetValue() const;

		/*========================================================================================================*/

		std::wstring GetXMLString();
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
        std::unordered_map<std::wstring, std::wstring>	m_AttributeMap;
        std::unordered_map<std::wstring, int>			m_ChildMap; //for uniq
        std::list<XMLElement>                           m_Elements;

        typedef std::list<XMLElement>::iterator          ElementsIterator;
        typedef std::list<XMLElement>::const_iterator    ElementsIteratorConst;

        typedef std::unordered_map<std::wstring, std::wstring>::iterator            AttMapIterator;
        typedef std::unordered_map<std::wstring, std::wstring>::const_iterator      AttMapIteratorConst;

    public:
		XMLElement();
		XMLElement( const std::wstring & name );
		XMLElement( const std::wstring & prefix, const std::wstring & localName );
		~XMLElement();

		void AppendText( const std::wstring & text );
		void AppendTextSymbol( const wchar_t symbol );

		void AppendAttribute( const XMLAttribute& attribute );
		void AppendAttribute( const std::wstring & name, const std::wstring & value );

		void AppendChild( const XMLElement& element, bool uniq = false);
		void AppendChild( XMLElementPtr element, bool uniq = false);

		void RemoveChild( const XMLElement& element );

		bool FindChild( const XMLElement& element );
		bool FindChildByName( const std::wstring & elementName ) const;

		bool RemoveChildByName( const std::wstring& elementName );

		bool operator == ( const XMLElement& element ) const;

		/*========================================================================================================*/

		std::wstring GetName() const;

		/*========================================================================================================*/

		std::wstring GetXMLString();

		/*========================================================================================================*/

		unsigned int GetAttributeCount() const;

		/*========================================================================================================*/

		unsigned int GetChildCount() const;
	};

	class CStringXmlWriter
	{
		std::wstring m_str;
	
	public:
		CStringXmlWriter();
		std::wstring GetXmlString();

		void SetXmlString(const std::wstring& strValue);
		void Clear();
		bool SaveToFile(const std::wstring& strFilePath, bool bEncodingToUTF8 = false);

		void WriteString(const std::wstring & strValue);
		void WriteInteger(int Value, int Base = 10);
		void WriteDouble(double Value);
		void WriteBoolean(bool Value);

		void WriteNodeBegin(const std::wstring& strNodeName, bool bAttributed = false);
		void WriteNodeEnd(const std::wstring& strNodeName, bool bEmptyNode = false, bool bEndNode = true);

		void WriteNode(const std::wstring& strNodeName, const std::wstring& strNodeValue);
		void WriteNode(const std::wstring& strNodeName, int nValue, int nBase = 10, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = L"");
		void WriteNode(const std::wstring& strNodeName, double dValue);

		void WriteAttribute(const std::wstring& strAttributeName, const std::wstring& strAttributeValue);
		void WriteAttribute(const std::wstring& strAttributeName, int nValue, int nBase = 10, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = L"");
		void WriteAttribute(const std::wstring& strAttributeName, double dValue);
	};
}
