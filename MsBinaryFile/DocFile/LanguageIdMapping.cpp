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

#include "LanguageIdMapping.h"

namespace DocFileFormat
{
	LanguageIdMapping::LanguageIdMapping (XMLTools::CStringXmlWriter* pWriter, LanguageType type, const std::wstring& langcode) 
		: PropertiesMapping (pWriter)
	{
		_type = type;
		_langcode = langcode;
		
		if (_langcode.empty()) _langcode = L"en-US";
	}

    LanguageIdMapping::LanguageIdMapping (XMLTools::XMLElement* parentElement, LanguageType type, const std::wstring& langcode) 
		: PropertiesMapping(NULL)
	{
		_parent = parentElement;
		_type = type;
		_langcode = langcode;
		
		if (_langcode.empty()) _langcode = L"en-US";
	}

	LanguageIdMapping::~LanguageIdMapping()
	{
	}
}

namespace DocFileFormat
{
	void LanguageIdMapping::Apply( IVisitable* lid )
	{
		if ( dynamic_cast<LanguageId*>( lid )->Code != Nothing )
		{
            XMLTools::XMLAttribute* att = NULL;

			switch ( _type )
			{
			case Default:
				{
                    att = new XMLTools::XMLAttribute( L"w:val", _langcode);
				}
				break;

			case EastAsian:
				{                    
                    att = new XMLTools::XMLAttribute( L"w:eastAsia", _langcode);
				}
				break;

			case Complex:
				{
                    att = new XMLTools::XMLAttribute( L"w:bidi", _langcode);
				}
				break;

			default:
				{    
                    att = new XMLTools::XMLAttribute( L"w:val", _langcode);
				}
				break;
			}

			if (m_pXmlWriter)
			{
				// !!!TODO!!!
				m_pXmlWriter->WriteString( att->GetXMLString());
			}
			else if ( _parent != NULL )
			{
				_parent->AppendAttribute( *att );
			}

			RELEASEOBJECT( att );
		}
	}
}
