/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

#include "../DocxFormat/WritingElement.h"
#include "../../../DesktopEditor/common/StringBuilder.h"

namespace OOX
{
namespace Spreadsheet
{

#define WritingStringCommon(start, func, val, end) \
		writer.WriteString(start); \
		writer.func(val); \
		writer.WriteString(end);
//#define WritingStringCommonDouble(start, val, end) \
//		writer.WriteString(start); \
//		writer.AddDouble(val, -1); \
//		writer.WriteString(end);
#define WritingStringCommonDouble(start, val, end) \
		writer.WriteString(start); \
		writer.WriteString(OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(val)); \
		writer.WriteString(end);


#define WritingStringValAttr(name, func, val) \
		WritingStringCommon(L"<" name L" val=\"", func, val, L"\"/>")
#define WritingStringValAttrString(name, val) \
		WritingStringValAttr(name, WriteString, val)
#define WritingStringValAttrEncodeXmlString(name, val) \
		WritingStringValAttr(name, WriteEncodeXmlString, val)
#define WritingStringValAttrInt(name, val) \
		WritingStringValAttr(name, AddInt, val)
#define WritingStringValAttrDouble(name, val) \
		WritingStringCommonDouble(L"<" name L" val=\"", val, L"\"/>")

#define WritingStringVal(name, func, val) \
		WritingStringCommon(L"<" name L">", func, val, L"</" name L">")
#define WritingStringValString(name, val) \
		WritingStringVal(name, WriteString, val)
#define WritingStringValEncodeXmlString(name, val) \
		WritingStringVal(name, WriteEncodeXmlString, val)
#define WritingStringValInt(name, val) \
		WritingStringVal(name, AddInt, val)
#define WritingStringValInt64(name, val) \
		WritingStringVal(name, AddInt64, val)
#define WritingStringValDouble(name, val) \
		WritingStringCommonDouble(L"<" name L">", val, L"</" name L">")

#define WritingStringAttr(name, func, val) \
		WritingStringCommon(L" " name L"=\"", func, val, L"\"")
#define WritingStringAttrString(name, val) \
		WritingStringAttr(name, WriteString, val)
#define WritingStringAttrEncodeXmlString(name, val) \
		WritingStringAttr(name, WriteEncodeXmlString, val)
#define WritingStringAttrInt(name, val) \
		WritingStringAttr(name, AddInt, val)
#define WritingStringAttrDouble(name, val) \
		WritingStringCommonDouble(L" " name L"=\"", val, L"\"")

#define WritingStringNullableAttr(name, func, elem, val) \
		if(elem.IsInit()) \
		{ \
			WritingStringAttr(name, func, val) \
		}
#define WritingStringNullableAttrString(name, elem, val) \
		WritingStringNullableAttr(name, WriteString, elem, val)
#define WritingStringNullableAttrEncodeXmlString(name, elem, val) \
		WritingStringNullableAttr(name, WriteEncodeXmlString, elem, val)
#define WritingStringNullableAttrInt(name, elem, val) \
		WritingStringNullableAttr(name, AddInt, elem, val)
#define WritingStringNullableAttrInt64(name, elem, val) \
		WritingStringNullableAttr(name, AddInt64, elem, val)
#define WritingStringNullableAttrDouble(name, elem, val) \
		if(elem.IsInit()) \
		{ \
			WritingStringAttrDouble(name, val) \
		}
#define WritingStringNullableAttrBool(name, elem) \
		WritingStringNullableAttrString(name, elem, elem->ToString3(SimpleTypes::onofftostring1))


	const double c_ag_Inch_to_MM	= 25.4;
	const double c_ag_1pxWidth		= 25.4 / 96;

	class WritingElement : public OOX::WritingElement
	{
	public:
        virtual EElementType    getType()   const
		{
			return OOX::et_x_Unknown;
		}
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const = 0;
	};

	template<typename ElemType = WritingElement>
	class WritingElementWithChilds : public WritingElement
	{
	public:
        std::vector<ElemType *>  m_arrItems;
		
		WritingElementWithChilds() {}
		virtual ~WritingElementWithChilds() 
		{
			ClearItems();
		}
		virtual void ClearItems()
		{
            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] ) delete m_arrItems[i];
            }
			m_arrItems.clear();
		}
	};
}
}
