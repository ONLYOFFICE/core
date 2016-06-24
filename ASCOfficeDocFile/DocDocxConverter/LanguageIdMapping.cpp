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

#include "LanguageIdMapping.h"
#include "../../Common/MS-LCID.h"

namespace DocFileFormat
{
	LanguageIdMapping::LanguageIdMapping (XmlUtils::CXmlWriter* pWriter, LanguageType type) : PropertiesMapping (pWriter)
	{
		_type = type;
	}

	LanguageIdMapping::LanguageIdMapping (XMLTools::XMLElement<wchar_t>* parentElement, LanguageType type) : PropertiesMapping(NULL)
	{
		_parent = parentElement;
		_type = type;
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
			wstring langcode = getLanguageCode( dynamic_cast<LanguageId*>( lid ) );

			XMLTools::XMLAttribute<wchar_t>* att = NULL;

			switch ( _type )
			{
			case Default:
				{
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:val" ), langcode.c_str() );
				}
				break;

			case EastAsian:
				{                    
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:eastAsia" ), langcode.c_str() );
				}
				break;

			case Complex:
				{
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:bidi" ), langcode.c_str() );
				}
				break;

			default:
				{    
					att = new XMLTools::XMLAttribute<wchar_t>( _T( "w:val" ), langcode.c_str() );
				}
				break;
			}

			if (m_pXmlWriter)
			{
				// !!!TODO!!!
				m_pXmlWriter->WriteString( att->GetXMLString().c_str() );
			}
			else if ( _parent != NULL )
			{
				_parent->AppendAttribute( *att );
			}

			RELEASEOBJECT( att );
		}
	}

	wstring LanguageIdMapping::getLanguageCode( LanguageId* lid )
	{
		int intLCID				= lid->Code;
		std::wstring strLCID	= msLCID2wstring(intLCID);

		if (strLCID.empty()) strLCID = L"en-US";

		return strLCID;
	}
}