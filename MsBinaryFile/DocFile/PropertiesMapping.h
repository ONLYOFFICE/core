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
