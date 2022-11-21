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

#include "../Common/Base/XmlTools.h"

#include "Global.h"
#include "BorderCode.h"
#include "ShadingDescriptor.h"
#include "SinglePropertyModifier.h"

#include "../../DesktopEditor/graphics/TemporaryCS.h"

namespace DocFileFormat
{
	class PropertiesMapping
	{
	public:
		PropertiesMapping( XMLTools::CStringXmlWriter* writer );

	protected:
		static void init();
        virtual void appendFlagElement( XMLTools::XMLElement* node, const SinglePropertyModifier& sprm, const std::wstring & elementName, bool unique );
        void appendFlagAttribute	( XMLTools::XMLElement* node,	const SinglePropertyModifier& sprm, const std::wstring & attributeName );

        void appendValueAttribute	( XMLTools::XMLElement* node,	const std::wstring & attributeName, const std::wstring & attributeValue );
        void appendValueAttribute	( XMLTools::XMLElement* node,	const std::wstring & attributeName, int attributeValue );
        void appendValueAttribute	( XMLTools::XMLElement* node,	const std::wstring & ttributeName,	short attributeValue );
        void appendValueAttribute	( XMLTools::XMLElement* node,	const std::wstring & attributeName, unsigned short attributeValue );
        void appendValueAttribute	( XMLTools::XMLElement* node,	const std::wstring & attributeName, unsigned char attributeValue );
        void appendValueElement		( XMLTools::XMLElement* node,	const std::wstring & elementName,	const std::wstring & elementValue, bool unique );
        void appendValueElement		( XMLTools::XMLElement* node,	const std::wstring & elementName,	short elementValue, bool unique );
        void appendValueElement		( XMLTools::XMLElement* node,	const std::wstring & elementName,	unsigned short elementValue, bool unique );
        void appendValueElement		( XMLTools::XMLElement* node,	const std::wstring & elementName,	unsigned char elementValue, bool unique );
        void appendShading			( XMLTools::XMLElement* parent, const ShadingDescriptor& desc );
        void appendDxaElement		( XMLTools::XMLElement* node,	const std::wstring & elementName, const std::wstring & elementValue, bool unique );
        void addOrSetBorder			( XMLTools::XMLElement* pBdr,	const XMLTools::XMLElement* border );
       
		void appendBorderAttributes	( BorderCode* brc, XMLTools::XMLElement* border );

		std::wstring getBorderType		( unsigned char type );
		std::wstring getShadingPattern	( const ShadingDescriptor& shd );
	protected:

		XMLTools::CStringXmlWriter* m_pXmlWriter;

        static std::map<unsigned char, std::wstring>	 brcTypeMap;
        static NSCriticalSection::CRITICAL_SECTION_SMART brcTypeMapLock;
	};
}
