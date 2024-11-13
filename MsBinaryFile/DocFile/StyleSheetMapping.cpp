/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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


#include "StyleSheetMapping.h"

namespace DocFileFormat
{
    std::map<std::wstring, std::wstring>	  StyleSheetMapping::m_mapStyleId;
    NSCriticalSection::CRITICAL_SECTION_SMART StyleSheetMapping::m_mapStyleIdLock;

	StyleSheetMapping::StyleSheetMapping( ConversionContext* ctx ) : AbstractOpenXmlMapping( new XMLTools::CStringXmlWriter() )
	{
		_ctx = ctx;
		m_document = _ctx->_doc;

		for (size_t i = 0; i < 157; ++i)
			mapCheckReservedNames.insert(std::make_pair(StyleIdentifierMap[i], 0));
	}

	/*========================================================================================================*/

	StyleSheetMapping::~StyleSheetMapping()
	{
		RELEASEOBJECT( m_pXmlWriter );
	}

	/*========================================================================================================*/

	void StyleSheetMapping::Apply( IVisitable* visited )
	{
		StyleSheet* sheet = static_cast<StyleSheet*>( visited );

		this->_ctx->_docx->RegisterStyleSheet();

		//start the document
        m_pXmlWriter->WriteNodeBegin( L"w:styles", TRUE );

		//write namespaces
        m_pXmlWriter->WriteAttribute( L"xmlns:w", OpenXmlNamespaces::WordprocessingML );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

		//document defaults
        m_pXmlWriter->WriteNodeBegin( L"w:docDefaults" );
		writeRunDefaults( sheet );
		writeParagraphDefaults( sheet );
        m_pXmlWriter->WriteNodeEnd( L"w:docDefaults" );

		//write the default styles
		if ( (sheet->Styles->size() < 12) || (sheet->Styles->at( 11 ) == NULL ))
		{
			//NormalTable
			writeNormalTableStyle();
		}
		int count_styles = 0;
		for ( size_t i = 0; i < sheet->Styles->size(); ++i)
		{
			StyleSheetDescription* style = sheet->Styles->at(i);
			if (!style) continue;

			count_styles++;
			
			if (count_styles > 4080) // todooo - skip/remap equal styles
				break;

			m_pXmlWriter->WriteNodeBegin(L"w:style", true);

			m_pXmlWriter->WriteAttribute(L"w:type", FormatUtils::MapValueToWideString(style->stk, &StyleKindMap[0][0], 5, 10));

			//!!!TODO: There is NO default styles in DOC file. So, we can't choose one of them!!!
			/*if ( ( style->sti != Null ) && ( style->sti != User ) )
			{
			//it's a default style
			m_pXmlWriter->WriteAttribute( L"w:default", L"1" );
			}*/

			m_pXmlWriter->WriteAttribute(L"w:styleId", FormatUtils::XmlEncode(MakeStyleId(style)));
			m_pXmlWriter->WriteNodeEnd(L"", true, false);

			std::wstring sAliases;
			std::wstring sName = FormatUtils::XmlEncode(getStyleName(style), true);
			m_pXmlWriter->WriteNodeBegin(L"w:name", true);
			if (style->sti == StyleIdentifier::Normal)
				m_pXmlWriter->WriteAttribute(L"w:val", L"Normal");
			else
			{
				std::size_t pos = sName.find(L",");
				if (pos != std::wstring::npos)
				{
					sAliases = sName.substr(pos + 1);
					sName = sName.substr(0, pos);
				}
				m_pXmlWriter->WriteAttribute(L"w:val", sName);
			}
			m_pXmlWriter->WriteNodeEnd(L"", true);

			if (false == sAliases.empty())
			{
				m_pXmlWriter->WriteNodeBegin(L"w:aliases", true);
				m_pXmlWriter->WriteAttribute(L"w:val", sAliases);
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}
			else
			if (style->sti == StyleIdentifier::Normal) // ??? < sti < 159
			{
				m_pXmlWriter->WriteNodeBegin(L"w:aliases", true);
				m_pXmlWriter->WriteAttribute(L"w:val", sName);
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}

			if ((style->istdBase != 4095) && (style->istdBase < sheet->Styles->size()))
			{
				m_pXmlWriter->WriteNodeBegin(L"w:basedOn", true);
				m_pXmlWriter->WriteAttribute(L"w:val", FormatUtils::XmlEncode(MakeStyleId(sheet->Styles->at(style->istdBase))));
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}

			if (style->istdNext < sheet->Styles->size())
			{
				m_pXmlWriter->WriteNodeBegin(L"w:next", true);
				m_pXmlWriter->WriteAttribute(L"w:val", FormatUtils::XmlEncode(MakeStyleId(sheet->Styles->at(style->istdNext))));
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}

			if (style->istdLink < sheet->Styles->size())
			{
				m_pXmlWriter->WriteNodeBegin(L"w:link", true);
				m_pXmlWriter->WriteAttribute(L"w:val", FormatUtils::XmlEncode(MakeStyleId(sheet->Styles->at(style->istdLink))));
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}
			if (style->fAutoRedef)
			{
				m_pXmlWriter->WriteNodeBegin(L"w:autoRedefine", true);
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}
			if (style->fHidden)
			{
				m_pXmlWriter->WriteNodeBegin(L"w:hidden", true);
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}
			if (style->uiPriority > 0)
			{
				m_pXmlWriter->WriteNodeBegin(L"w:uiPriority", true);
				m_pXmlWriter->WriteAttribute(L"w:val", std::to_wstring(style->uiPriority));
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}
			if (style->fQFormat)
			{
				m_pXmlWriter->WriteNodeBegin(L"w:qFormat", true);
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}
			if (style->fSemiHidden)
			{
				m_pXmlWriter->WriteNodeBegin(L"w:semiHidden", true);
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}
			if (style->fUnhideWhenUsed)
			{
				m_pXmlWriter->WriteNodeBegin(L"w:unhideWhenUsed", true);
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}
			if (style->fLocked)
			{
				m_pXmlWriter->WriteNodeBegin(L"w:locked", true);
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}
			if (style->rsid > 0)
			{
				m_pXmlWriter->WriteNodeBegin(L"w:rsid", true);
				m_pXmlWriter->WriteAttribute(L"w:val", FormatUtils::IntToFormattedWideString(style->rsid, L"%08X"));
				m_pXmlWriter->WriteNodeEnd(L"", true);
			}
			//write paragraph properties
			if (style->papx != NULL)
			{
				bool isBidi = false;
				ParagraphPropertiesMapping* ppMappingnew = new ParagraphPropertiesMapping(m_pXmlWriter, _ctx, m_document, NULL, isBidi, false);
				style->papx->Convert(ppMappingnew);
				RELEASEOBJECT(ppMappingnew);
			}

			//write character properties
			if (style->chpx != NULL)
			{
				RevisionData rev;
				rev.Type = NoRevision;
				CharacterPropertiesMapping* cpMapping = new CharacterPropertiesMapping(m_pXmlWriter, m_document, &rev, style->papx, true, false);
				style->chpx->Convert(cpMapping);
				RELEASEOBJECT(cpMapping);
			}
			//write table properties
			if (style->tapx != NULL)
			{
				std::vector<short> tableGrid;
				TablePropertiesMapping* tpMapping = new TablePropertiesMapping(m_pXmlWriter, sheet, &tableGrid, false);
				style->tapx->Convert(tpMapping);
				RELEASEOBJECT(tpMapping);
			}
			m_pXmlWriter->WriteNodeEnd(L"w:style");
		}

        m_pXmlWriter->WriteNodeEnd( L"w:styles" );

		this->_ctx->_docx->StyleSheetXML = std::wstring( m_pXmlWriter->GetXmlString() );
	}

	/*========================================================================================================*/

	/// Generates a style id for custom style names or returns the build-in identifier for build-in styles.
	std::wstring StyleSheetMapping::MakeStyleId( StyleSheetDescription* std )
	{
		std::wstring ret;

		if ( std != NULL )
		{
			if ( ( std->sti != User ) && ( std->sti != Null ) )
			{
				//use the identifier
				if ( std->sti < 159 )
				{
					ret = std::wstring( StyleIdentifierMap[std->sti] );
				}
				else
				{
					ret = FormatUtils::IntToWideString( std->sti );
				}
			}
			else
			{
				//if no identifier is set, use the unique id.
				std::map<std::wstring, std::wstring>::const_iterator findResult = m_mapStyleId.find(std->xstzName);
				if( findResult != m_mapStyleId.end() )
				{
					ret = findResult->second;
				}
				else
				{
					//genarate new
					m_mapStyleIdLock.Enter();
					int nIndex = m_mapStyleId.size();
                    ret = L"UserStyle_" + FormatUtils::IntToWideString( nIndex );
					std::pair< std::wstring, std::wstring > p( std->xstzName, ret);
					m_mapStyleId.insert(p);
					m_mapStyleIdLock.Leave();
				}
			}
		}

		return ret;
	}

	/*========================================================================================================*/

	void StyleSheetMapping::writeRunDefaults( StyleSheet* sheet )
	{
		if ((sheet->stshi) && (!sheet->stshi->rgftcStandardChpStsh.empty()))
		{
			m_pXmlWriter->WriteNodeBegin( L"w:rPrDefault" );
			m_pXmlWriter->WriteNodeBegin( L"w:rPr" );

			//write default fonts
			m_pXmlWriter->WriteNodeBegin( L"w:rFonts", TRUE );

			FontFamilyName* ffnAscii = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( sheet->stshi->rgftcStandardChpStsh[0] ) );
			if (ffnAscii)
				m_pXmlWriter->WriteAttribute( L"w:ascii", FormatUtils::XmlEncode(ffnAscii->xszFtn, true));

			FontFamilyName* ffnAsia = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( sheet->stshi->rgftcStandardChpStsh[1] ) );
			if (ffnAsia)
				m_pXmlWriter->WriteAttribute( L"w:eastAsia", FormatUtils::XmlEncode(ffnAsia->xszFtn, true));

			FontFamilyName* ffnAnsi = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( sheet->stshi->rgftcStandardChpStsh[2] ) );
			if (ffnAnsi)
				m_pXmlWriter->WriteAttribute( L"w:hAnsi", FormatUtils::XmlEncode(ffnAnsi->xszFtn, true));

			FontFamilyName* ffnComplex = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( sheet->stshi->rgftcStandardChpStsh[3] ) );
			if (ffnComplex)
				m_pXmlWriter->WriteAttribute( L"w:cs", FormatUtils::XmlEncode(ffnComplex->xszFtn, true));

			m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
			m_pXmlWriter->WriteNodeEnd( L"w:rFonts" );

			std::wstring langcode = m_document->m_lcidConverter.get_wstring(m_document->FIB->m_FibBase.lid);

			m_pXmlWriter->WriteNodeBegin( L"w:lang", TRUE );
			m_pXmlWriter->WriteAttribute( L"w:val", langcode);
			m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
			m_pXmlWriter->WriteNodeEnd( L"w:lang" );

			m_pXmlWriter->WriteNodeEnd( L"w:rPr" );
			m_pXmlWriter->WriteNodeEnd( L"w:rPrDefault" );
		}
	}

	/*========================================================================================================*/

	void StyleSheetMapping::writeParagraphDefaults( StyleSheet* sheet )
	{
		//if there is no pPrDefault, Word will not used the default paragraph settings.
		//writing an empty pPrDefault will cause Word to load the default paragraph settings.
        m_pXmlWriter->WriteNodeBegin( L"w:pPrDefault" );

        m_pXmlWriter->WriteNodeEnd( L"w:pPrDefault" );
	}

	/*========================================================================================================*/

	/// Chooses the correct style name.
	/// Word 2007 needs the identifier instead of the stylename for translating it into the UI language.
	std::wstring StyleSheetMapping::getStyleName( StyleSheetDescription* std )
	{
		std::wstring id;
		std::wstring name;

		if ( std != NULL )
		{
			name =  std->xstzName;

			if ( ( std->sti != User ) && ( std->sti != Null ) )
			{
				//use the identifier
				if ( std->sti < 159 )
				{
					id = std::wstring( StyleIdentifierMap[std->sti] );
				}
				else
				{
					id = FormatUtils::IntToWideString( std->sti );
				}
			}
			else
			{
				std::map<std::wstring, std::wstring>::const_iterator findResult = m_mapStyleId.find(name);
				if (mapCheckReservedNames.find(name) != mapCheckReservedNames.end())
				{
					name += L"1";
				}
				if( findResult != m_mapStyleId.end() )
				{
					id = findResult->second;
				}
				else
				{
					//genarate new
					m_mapStyleIdLock.Enter();
					int nIndex = m_mapStyleId.size();
                    id = L"UserStyle_" + FormatUtils::IntToWideString( nIndex );
					std::pair< std::wstring, std::wstring > p( name, id);
					m_mapStyleId.insert(p);
					m_mapStyleIdLock.Leave();
				}
			}
		}
		if (name.empty())
			name = id;

		return name;
	}

	/*========================================================================================================*/

	/// Writes the "NormalTable" default style
	void StyleSheetMapping::writeNormalTableStyle()
	{
        m_pXmlWriter->WriteNodeBegin( L"w:style", TRUE );

        m_pXmlWriter->WriteAttribute( L"w:type", L"table" );
        m_pXmlWriter->WriteAttribute( L"w:default", L"1" );
        m_pXmlWriter->WriteAttribute( L"w:styleId", L"TableNormal" );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

        m_pXmlWriter->WriteNodeBegin( L"w:name", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:val", L"Normal Table" );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

        m_pXmlWriter->WriteNodeBegin( L"w:uiPriority", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:val", L"99" );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

        m_pXmlWriter->WriteNodeBegin( L"w:semiHidden", TRUE );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

        m_pXmlWriter->WriteNodeBegin( L"w:unhideWhenUsed", TRUE );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

        m_pXmlWriter->WriteNodeBegin( L"w:qFormat", TRUE );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

        m_pXmlWriter->WriteNodeBegin( L"w:tblPr" );

        m_pXmlWriter->WriteNodeBegin( L"w:tblInd", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:w", L"0" );
        m_pXmlWriter->WriteAttribute( L"w:type", L"dxa" );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

        m_pXmlWriter->WriteNodeBegin( L"w:tblCellMar" );

        m_pXmlWriter->WriteNodeBegin( L"w:top", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:w", L"0" );
        m_pXmlWriter->WriteAttribute( L"w:type", L"dxa" );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

        m_pXmlWriter->WriteNodeBegin( L"w:left", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:w", L"108" );
        m_pXmlWriter->WriteAttribute( L"w:type", L"dxa" );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

        m_pXmlWriter->WriteNodeBegin( L"w:bottom", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:w", L"0" );
        m_pXmlWriter->WriteAttribute( L"w:type", L"dxa" );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

        m_pXmlWriter->WriteNodeBegin( L"w:right", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:w", L"108" );
        m_pXmlWriter->WriteAttribute( L"w:type", L"dxa" );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

        m_pXmlWriter->WriteNodeEnd( L"w:tblCellMar" );
        m_pXmlWriter->WriteNodeEnd( L"w:tblPr" );
        m_pXmlWriter->WriteNodeEnd( L"w:style" );
	}
}
