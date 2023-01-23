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
#ifndef _BUILD_XMLWRITER_CROSSPLATFORM_H_
#define _BUILD_XMLWRITER_CROSSPLATFORM_H_

#include <vector>
#include <list>
#include <map>

#ifdef _MSC_VER
   #pragma warning (disable: 4100 4189)
#endif

#include "../../common/StringBuilder.h"

#ifndef XML_UNUSED
#define XML_UNUSED( arg )  ( (arg) = (arg) )
#endif

namespace XmlUtils
{
	class KERNEL_DECL CXmlWriter
	{
	private:
		std::wstring m_str;
	
	public:

		CXmlWriter();
		
		std::wstring GetXmlString();
		void SetXmlString(const std::wstring& strValue);
		
		bool SaveToFile(const std::wstring& strFilePath/*, bool bEncodingToUTF8 = false*/);
		void WriteString(const std::wstring& strValue);
		void WriteInteger(int Value);
		void WriteDouble(double Value);
		void WriteBoolean(bool Value);
		void WriteNodeBegin(const std::wstring& strNodeName, bool bAttributed = false);
		void WriteNodeEnd(const std::wstring& strNodeName, bool bEmptyNode = false, bool bEndNode = true);
		void WriteNode(const std::wstring& strNodeName, const std::wstring& strNodeValue);
		void WriteNode(const std::wstring& strNodeName, int nValue, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = L"");
		void WriteNode(const std::wstring& strNodeName, double dValue);
		void WriteAttribute(const std::wstring& strAttributeName, const std::wstring& strAttributeValue);
		void WriteAttribute(const std::wstring& strAttributeName, int nValue, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = (L""));
		void WriteAttribute(const std::wstring& strAttributeName, double dValue);
	};
}

#endif // _BUILD_XMLWRITER_CROSSPLATFORM_H_
