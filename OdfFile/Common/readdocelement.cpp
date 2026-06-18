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

#include "readdocelement.h"
#include "xml/attributes.h"
#include "xml/sax.h"
 
namespace cpdoccore { 
namespace common {

bool read_doc_element::read_sax( xml::sax * Reader)
{
	const unsigned int currentDepth = Reader->depth();
   
	xml::attributes_wc_ptr attrb = xml::read_attributes( Reader );
    add_attributes( attrb );
    
    if (!attrb->check())
    {
        //std::wcerr << " -- attr check error (" << Reader->nodeQualifiedName() << ")\n";
    }

	//const std::wstring value = Reader->value();

	//if (value.length() > 0)
	//{
	//	add_text(value);
	//}

    xml::NodeType nodeType = Reader->nodeType();
	
	if (Reader->isEmptyElement())
	{
		return false;
	}

    while (true)
	{
		nodeType = Reader->next(currentDepth);
		if (nodeType == xml::typeEOF || nodeType == xml::typeNone ) break;
       
		if (Reader->depth() <= currentDepth + 1 && nodeType == xml::typeEndElement) break;
        
		switch( nodeType )
        {
			case xml::typeElement:
			{
				const std::wstring namespacePrefix	= Reader->namespacePrefix();
				const std::wstring localName		= Reader->nodeLocalName();
					
				add_child_element(Reader, namespacePrefix, localName);
			}
			break;
			case xml::typeWhitespace:
			{
				const std::wstring value = Reader->value();
				if (false == value.empty())
				{
					add_space(value);
				}
			}
			break;
			case xml::typeCDATA:
			case xml::typeText:
			{
				const std::wstring value = Reader->value();
				if (false == value.empty())
				{
					add_text(value);
				}
			}
			break;
        }
    }
    return true;
}

}
}
