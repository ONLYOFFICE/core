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

#include "FontTableMapping.h"

namespace DocFileFormat
{
	FontTableMapping::FontTableMapping( ConversionContext* ctx ): AbstractOpenXmlMapping( new XmlUtils::CXmlWriter() )
	{
		_ctx = ctx;
	}

	/*========================================================================================================*/

	FontTableMapping::~FontTableMapping()
	{
		RELEASEOBJECT (m_pXmlWriter);
	}

	/*========================================================================================================*/

	void FontTableMapping::Apply( IVisitable* visited )
	{
		StringTable<FontFamilyName>* table = static_cast<StringTable<FontFamilyName>*>( visited );

		this->_ctx->_docx->RegisterFontTable();

		m_pXmlWriter->WriteNodeBegin( _T( "?xml version=\"1.0\" encoding=\"UTF-8\"?" ) );
		m_pXmlWriter->WriteNodeBegin( _T( "w:fonts" ), TRUE );
		m_pXmlWriter->WriteAttribute( _T( "xmlns:w" ), OpenXmlNamespaces::WordprocessingML );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

		int sz_fonts = table->Data.size();

		for ( vector<ByteStructure*>::iterator iter = table->Data.begin(); iter != table->Data.end(); iter++ )
		{
			FontFamilyName* font = dynamic_cast<FontFamilyName*>( *iter );

			m_pXmlWriter->WriteNodeBegin( _T( "w:font" ), TRUE );
			m_pXmlWriter->WriteAttribute( _T( "w:name" ), FormatUtils::XmlEncode(font->xszFtn, true).c_str());
			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

			//alternative name
			if ( ( font->xszAlt != wstring( _T( "" ) ) ) && ( font->xszAlt.length() > 0 ) )
			{
				m_pXmlWriter->WriteNodeBegin( _T( "w:altName" ), TRUE );
				m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::XmlEncode(font->xszAlt, true).c_str() );
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );
				m_pXmlWriter->WriteNodeEnd( _T( "w:altName" ) );
			}

			//charset
			m_pXmlWriter->WriteNodeBegin( _T("w:charset" ), TRUE );
			m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::IntToFormattedWideString( font->chs, _T( "%02x" ) ).c_str() );
			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );
			m_pXmlWriter->WriteNodeEnd( _T( "w:charset" ) );

			//font family
			m_pXmlWriter->WriteNodeBegin( _T("w:family"), TRUE );
			m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::MapValueToWideString( font->ff, &FontFamily[0][0], 6, 11 ).c_str() );
			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );
			m_pXmlWriter->WriteNodeEnd( _T( "w:family" ) );

			//panose
			m_pXmlWriter->WriteNodeBegin( _T("w:panose1"), TRUE );

			wstring wstr( _T( "" ) );

			for ( unsigned int i = 0; i < font->panoseSize; i++ )
			{
				wstr += FormatUtils::IntToFormattedWideString( font->panose[i], _T( "%02x" ) );
			}

			m_pXmlWriter->WriteAttribute( _T( "w:val" ), wstr.c_str() );
			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );
			m_pXmlWriter->WriteNodeEnd( _T( "w:panose1" ) );

			//pitch
			m_pXmlWriter->WriteNodeBegin( _T("w:pitch"), TRUE );
			m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::MapValueToWideString( font->prq, &FontPitch[0][0], 3, 9 ).c_str() );
			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );
			m_pXmlWriter->WriteNodeEnd( _T( "w:pitch" ) );

			//truetype
			if ( !font->fTrueType )
			{
				m_pXmlWriter->WriteNodeBegin( _T("w:notTrueType"), TRUE );
				m_pXmlWriter->WriteAttribute( _T( "w:val" ), _T( "true" ) );
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );
				m_pXmlWriter->WriteNodeEnd( _T( "w:notTrueType" ) );
			}

			//font signature
			m_pXmlWriter->WriteNodeBegin( _T("w:sig"), TRUE );
			m_pXmlWriter->WriteAttribute( _T( "w:usb0" ), FormatUtils::IntToFormattedWideString( font->fs.UnicodeSubsetBitfield0, _T( "%08x" ) ).c_str() );
			m_pXmlWriter->WriteAttribute( _T( "w:usb1" ), FormatUtils::IntToFormattedWideString( font->fs.UnicodeSubsetBitfield1, _T( "%08x" ) ).c_str() );
			m_pXmlWriter->WriteAttribute( _T( "w:usb2" ), FormatUtils::IntToFormattedWideString( font->fs.UnicodeSubsetBitfield2, _T( "%08x" ) ).c_str() );
			m_pXmlWriter->WriteAttribute( _T( "w:usb3" ), FormatUtils::IntToFormattedWideString( font->fs.UnicodeSubsetBitfield3, _T( "%08x" ) ).c_str() );
			m_pXmlWriter->WriteAttribute( _T( "w:csb0" ), FormatUtils::IntToFormattedWideString( font->fs.CodePageBitfield0, _T( "%08x" ) ).c_str() );
			m_pXmlWriter->WriteAttribute( _T( "w:csb1" ), FormatUtils::IntToFormattedWideString( font->fs.CodePageBitfield1, _T( "%08x" ) ).c_str() );
			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );
			m_pXmlWriter->WriteNodeEnd( _T( "w:sig" ) );

			m_pXmlWriter->WriteNodeEnd( _T( "w:font" ) );
		}

		m_pXmlWriter->WriteNodeEnd( _T("w:fonts") );

		this->_ctx->_docx->FontTableXML = wstring( m_pXmlWriter->GetXmlString() );
	}
}