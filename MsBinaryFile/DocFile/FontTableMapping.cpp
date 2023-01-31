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

#include "FontTableMapping.h"

namespace DocFileFormat
{
	FontTableMapping::FontTableMapping( ConversionContext* ctx ) : AbstractOpenXmlMapping( new XMLTools::CStringXmlWriter() )
	{
		_ctx = ctx;
	}


	FontTableMapping::~FontTableMapping()
	{
		RELEASEOBJECT (m_pXmlWriter);
	}


	void FontTableMapping::Apply( IVisitable* visited )
	{
		StringTable<FontFamilyName>* table = static_cast<StringTable<FontFamilyName>*>( visited );

		_ctx->_docx->RegisterFontTable();

        m_pXmlWriter->WriteNodeBegin( L"w:fonts", TRUE );
        m_pXmlWriter->WriteAttribute( L"xmlns:w", OpenXmlNamespaces::WordprocessingML );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

		int sz_fonts = table->Data.size();
		int users_fonts = 0;

		for ( std::vector<ByteStructure*>::iterator iter = table->Data.begin(); iter != table->Data.end(); iter++ )
		{
			FontFamilyName* font = dynamic_cast<FontFamilyName*>( *iter );

            m_pXmlWriter->WriteNodeBegin( L"w:font", TRUE );

			std::wstring name_ = FormatUtils::XmlEncode(font->xszFtn, true);
			if (name_.empty()) 
				name_ = L"UserFont_" + std::to_wstring(++users_fonts);
            m_pXmlWriter->WriteAttribute( L"w:name", name_);
            m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

			//alternative name
            if ( ( font->xszAlt != std::wstring( L"" ) ) && ( font->xszAlt.length() > 0 ) )
			{
                m_pXmlWriter->WriteNodeBegin( L"w:altName", TRUE );
                m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::XmlEncode(font->xszAlt, true));
                m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
                m_pXmlWriter->WriteNodeEnd( L"w:altName" );
			}

			//charset
            m_pXmlWriter->WriteNodeBegin( L"w:charset", TRUE );
            m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::IntToFormattedWideString( font->chs, L"%02x" ));
            m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
            m_pXmlWriter->WriteNodeEnd( L"w:charset" );

			//font family
            m_pXmlWriter->WriteNodeBegin( L"w:family", TRUE );
            m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::MapValueToWideString( font->ff, &FontFamily[0][0], 6, 11 ));
            m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
            m_pXmlWriter->WriteNodeEnd( L"w:family" );

			//panose
            m_pXmlWriter->WriteNodeBegin( L"w:panose1", TRUE );

            std::wstring wstr( L"" );

			for ( unsigned int i = 0; i < font->panoseSize; i++ )
			{
                wstr += FormatUtils::IntToFormattedWideString( font->panose[i], L"%02x" );
			}

            m_pXmlWriter->WriteAttribute( L"w:val", wstr);
            m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
            m_pXmlWriter->WriteNodeEnd( L"w:panose1" );

			//pitch
            m_pXmlWriter->WriteNodeBegin( L"w:pitch", TRUE );
            m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::MapValueToWideString( font->prq, &FontPitch[0][0], 3, 9 ));
            m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
            m_pXmlWriter->WriteNodeEnd( L"w:pitch" );

			//truetype
			if ( !font->fTrueType )
			{
                m_pXmlWriter->WriteNodeBegin( L"w:notTrueType", TRUE );
                m_pXmlWriter->WriteAttribute( L"w:val", L"true" );
                m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
                m_pXmlWriter->WriteNodeEnd( L"w:notTrueType" );
			}

			//font signature
            m_pXmlWriter->WriteNodeBegin( L"w:sig", TRUE );
            m_pXmlWriter->WriteAttribute( L"w:usb0", FormatUtils::IntToFormattedWideString( font->fs.UnicodeSubsetBitfield0, L"%08X" ));
            m_pXmlWriter->WriteAttribute( L"w:usb1", FormatUtils::IntToFormattedWideString( font->fs.UnicodeSubsetBitfield1, L"%08X" ));
            m_pXmlWriter->WriteAttribute( L"w:usb2", FormatUtils::IntToFormattedWideString( font->fs.UnicodeSubsetBitfield2, L"%08X" ));
            m_pXmlWriter->WriteAttribute( L"w:usb3", FormatUtils::IntToFormattedWideString( font->fs.UnicodeSubsetBitfield3, L"%08X" ));
            m_pXmlWriter->WriteAttribute( L"w:csb0", FormatUtils::IntToFormattedWideString( font->fs.CodePageBitfield0, L"%08X" ));
            m_pXmlWriter->WriteAttribute( L"w:csb1", FormatUtils::IntToFormattedWideString( font->fs.CodePageBitfield1, L"%08X" ));
            m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
            m_pXmlWriter->WriteNodeEnd( L"w:sig" );

            m_pXmlWriter->WriteNodeEnd( L"w:font" );
		}

        m_pXmlWriter->WriteNodeEnd( L"w:fonts");

		_ctx->_docx->FontTableXML = m_pXmlWriter->GetXmlString() ;
	}
}
