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

#include "../Common/XmlTools.h"
#include "Global.h"
#include "BorderCode.h"
#include "RGBColor.h"
#include "ShadingDescriptor.h"
#include "SinglePropertyModifier.h"

#include "../../Common/DocxFormat/Source/XML/xmlutils.h"


#include "../Win32/ASCOfficeCriticalSection.h" //for linux make inside

namespace DocFileFormat
{
	class PropertiesMapping
	{
	public:
		PropertiesMapping( XmlUtils::CXmlWriter* writer );

	protected:
		static void init();
		void appendFlagAttribute( XMLTools::XMLElement<wchar_t>* node, const SinglePropertyModifier& sprm, const wchar_t* attributeName );
		virtual void appendFlagElement( XMLTools::XMLElement<wchar_t>* node, const SinglePropertyModifier& sprm, const wchar_t* elementName, bool unique );
		void appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, const wchar_t* attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, int attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, short attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, unsigned short attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, unsigned char attributeValue );
		void appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, const wchar_t* elementValue, bool unique );
		void appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, short elementValue, bool unique );
		void appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, unsigned short elementValue, bool unique );
		void appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, unsigned char elementValue, bool unique );
		void appendBorderAttributes( BorderCode* brc, XMLTools::XMLElement<wchar_t>* border );
		void appendShading( XMLTools::XMLElement<wchar_t>* parent, const ShadingDescriptor& desc );
		wstring getBorderType( unsigned char type );
		wstring getShadingPattern( const ShadingDescriptor& shd );
		void appendDxaElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, const wchar_t* elementValue, bool unique );
		void addOrSetBorder( XMLTools::XMLElement<wchar_t>* pBdr, const XMLTools::XMLElement<wchar_t>* border );

	protected:

		XmlUtils::CXmlWriter* m_pXmlWriter;
		static map<unsigned char, wstring> brcTypeMap;
		
		static ASCOfficeCriticalSection brcTypeMapLock;
	};
}
