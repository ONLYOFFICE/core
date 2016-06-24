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


#include "StyleSheetMapping.h"

namespace DocFileFormat
{
	map<std::wstring, std::wstring> StyleSheetMapping::m_mapStyleId;
	ASCOfficeCriticalSection StyleSheetMapping::m_mapStyleIdLock;

	StyleSheetMapping::StyleSheetMapping( ConversionContext* ctx ) : AbstractOpenXmlMapping( new XmlUtils::CXmlWriter() )
	{
		_ctx = ctx;
		m_document = _ctx->_doc;
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
		m_pXmlWriter->WriteNodeBegin( _T( "?xml version=\"1.0\" encoding=\"UTF-8\"?" ) );    
		m_pXmlWriter->WriteNodeBegin( _T( "w:styles" ), TRUE );

		//write namespaces
		m_pXmlWriter->WriteAttribute( _T( "xmlns:w" ), OpenXmlNamespaces::WordprocessingML );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

		//document defaults
		m_pXmlWriter->WriteNodeBegin( _T( "w:docDefaults" ) );    
		writeRunDefaults( sheet );
		writeParagraphDefaults( sheet );
		m_pXmlWriter->WriteNodeEnd( _T( "w:docDefaults" ) );

		//write the default styles
		if ( (sheet->Styles->size() < 12) || (sheet->Styles->at( 11 ) == NULL ))
		{
			//NormalTable
			writeNormalTableStyle();
		}

		for ( vector<StyleSheetDescription*>::iterator iter = sheet->Styles->begin(); iter != sheet->Styles->end(); iter++ )
		{
			if ( *iter != NULL )
			{
				m_pXmlWriter->WriteNodeBegin( _T( "w:style" ), TRUE );  

				m_pXmlWriter->WriteAttribute( _T( "w:type" ), FormatUtils::MapValueToWideString( (*iter)->stk, &StyleKindMap[0][0], 5, 10 ).c_str() );

				//!!!TODO: There is NO default styles in DOC file. So, we can't choose one of them!!!
				/*if ( ( (*iter)->sti != Null ) && ( (*iter)->sti != User ) )
				{
				//it's a default style
				m_pXmlWriter->WriteAttribute( _T( "w:default" ), _T( "1" ) );
				}*/

				m_pXmlWriter->WriteAttribute( _T( "w:styleId" ), FormatUtils::XmlEncode(MakeStyleId( *iter )).c_str() );
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

				// <w:name val="" />
				m_pXmlWriter->WriteNodeBegin( _T( "w:name" ), TRUE );  
				m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::XmlEncode(getStyleName( *iter ), true ).c_str());
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

				// <w:basedOn val="" />
				if ( ( (*iter)->istdBase != 4095 ) && ( (*iter)->istdBase < sheet->Styles->size() ) )
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:basedOn" ), TRUE );  
					m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::XmlEncode(MakeStyleId( sheet->Styles->at( (*iter)->istdBase ) )).c_str() );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
				}

				// <w:next val="" />
				if ( (*iter)->istdNext < sheet->Styles->size() )
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:next" ), TRUE );  
					m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::XmlEncode(MakeStyleId( sheet->Styles->at( (*iter)->istdNext ) )).c_str() );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
				}

				// <w:link val="" />
				if ( (*iter)->istdLink < sheet->Styles->size() )
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:link" ), TRUE );  
					m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::XmlEncode(MakeStyleId( sheet->Styles->at( (*iter)->istdLink ) )).c_str() );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
				}

				// <w:locked/>
				if ( (*iter)->fLocked )
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:locked" ) );  
					m_pXmlWriter->WriteNodeEnd( _T( "w:locked" ) );
				}

				// <w:hidden/>
				if ( (*iter)->fHidden )
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:hidden" ) );  
					m_pXmlWriter->WriteNodeEnd( _T( "w:hidden" ) );
				}

				// <w:semiHidden/>
				if ( (*iter)->fSemiHidden )
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:semiHidden" ) );  
					m_pXmlWriter->WriteNodeEnd( _T( "w:semiHidden" ) );
				}

				//write paragraph properties
				if ( (*iter)->papx != NULL )
				{
					bool isBidi = false;
					ParagraphPropertiesMapping* ppMappingnew = new ParagraphPropertiesMapping (m_pXmlWriter, _ctx, m_document, NULL, isBidi, false);
					(*iter)->papx->Convert( ppMappingnew );
					RELEASEOBJECT( ppMappingnew );
				}

				//write character properties
				if ( (*iter)->chpx != NULL )
				{
					RevisionData rev;
					rev.Type = NoRevision;
					CharacterPropertiesMapping* cpMapping = new CharacterPropertiesMapping (m_pXmlWriter, m_document, &rev, (*iter)->papx, true, false);
					(*iter)->chpx->Convert( cpMapping );
					RELEASEOBJECT( cpMapping );
				}

				//write table properties
				if ( (*iter)->tapx != NULL )
				{
					vector<short> tableGrid;
					TablePropertiesMapping* tpMapping = new TablePropertiesMapping (m_pXmlWriter, sheet, &tableGrid, false);
					(*iter)->tapx->Convert( tpMapping );
					RELEASEOBJECT( tpMapping );
				}

				m_pXmlWriter->WriteNodeEnd( _T( "w:style" ) );
			}
		}

		m_pXmlWriter->WriteNodeEnd( _T( "w:styles" ) );

		this->_ctx->_docx->StyleSheetXML = wstring( m_pXmlWriter->GetXmlString() );
	}

	/*========================================================================================================*/

	/// Generates a style id for custom style names or returns the build-in identifier for build-in styles.
	wstring StyleSheetMapping::MakeStyleId( StyleSheetDescription* std )
	{
		wstring ret;

		if ( std != NULL )
		{
			if ( ( std->sti != User ) && ( std->sti != Null ) )
			{
				//use the identifier
				if ( std->sti < 159 )
				{
					ret = wstring( StyleIdentifierMap[std->sti] );
				}
				else
				{
					ret = FormatUtils::IntToWideString( std->sti );
				}
			}
			else
			{
				//if no identifier is set, use the unique id.
				map<std::wstring, std::wstring>::const_iterator findResult = m_mapStyleId.find(std->xstzName);
				if( findResult != m_mapStyleId.end() )
				{
					ret = findResult->second;
				}
				else
				{
					//genarate new
					m_mapStyleIdLock.Enter();
					int nIndex = m_mapStyleId.size();
					ret = _T("UserStyle_") + FormatUtils::IntToWideString( nIndex );
					pair< std::wstring, std::wstring > p( std->xstzName, ret);
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
		m_pXmlWriter->WriteNodeBegin( _T( "w:rPrDefault" ) );
		m_pXmlWriter->WriteNodeBegin( _T( "w:rPr" ) );

		//write default fonts
		m_pXmlWriter->WriteNodeBegin( _T( "w:rFonts" ), TRUE );

		FontFamilyName* ffnAscii = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( sheet->stshi->rgftcStandardChpStsh[0] ) );
		if (ffnAscii)
			m_pXmlWriter->WriteAttribute( _T( "w:ascii" ), FormatUtils::XmlEncode(ffnAscii->xszFtn).c_str() );

		FontFamilyName* ffnAsia = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( sheet->stshi->rgftcStandardChpStsh[1] ) );
		if (ffnAsia)
			m_pXmlWriter->WriteAttribute( _T( "w:eastAsia" ), FormatUtils::XmlEncode(ffnAsia->xszFtn).c_str() );

		FontFamilyName* ffnAnsi = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( sheet->stshi->rgftcStandardChpStsh[2] ) );
		if (ffnAnsi)
			m_pXmlWriter->WriteAttribute( _T( "w:hAnsi" ), FormatUtils::XmlEncode(ffnAnsi->xszFtn).c_str() );

		FontFamilyName* ffnComplex = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( sheet->stshi->rgftcStandardChpStsh[3] ) );
		if (ffnComplex)
			m_pXmlWriter->WriteAttribute( _T( "w:cs" ), FormatUtils::XmlEncode(ffnComplex->xszFtn).c_str() );

		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );
		m_pXmlWriter->WriteNodeEnd( _T( "w:rFonts" ) );

		LanguageId langid(this->m_document->FIB->m_FibBase.lid);
		wstring langcode = LanguageIdMapping::getLanguageCode( &langid );

		m_pXmlWriter->WriteNodeBegin( _T( "w:lang" ), TRUE );
		m_pXmlWriter->WriteAttribute( _T( "w:val" ), langcode.c_str() );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );
		m_pXmlWriter->WriteNodeEnd( _T( "w:lang" ) );

		m_pXmlWriter->WriteNodeEnd( _T( "w:rPr" ) );
		m_pXmlWriter->WriteNodeEnd( _T( "w:rPrDefault" ) );
	}

	/*========================================================================================================*/

	void StyleSheetMapping::writeParagraphDefaults( StyleSheet* sheet )
	{
		//if there is no pPrDefault, Word will not used the default paragraph settings.
		//writing an empty pPrDefault will cause Word to load the default paragraph settings.
		m_pXmlWriter->WriteNodeBegin( _T( "w:pPrDefault" ) );

		m_pXmlWriter->WriteNodeEnd( _T( "w:pPrDefault" ) );
	}

	/*========================================================================================================*/

	/// Chooses the correct style name.
	/// Word 2007 needs the identifier instead of the stylename for translating it into the UI language.
	wstring StyleSheetMapping::getStyleName( StyleSheetDescription* std )
	{
		wstring id;
		wstring name;

		if ( std != NULL )
		{
			name =  std->xstzName;

			if ( ( std->sti != User ) && ( std->sti != Null ) )
			{
				//use the identifier
				if ( std->sti < 159 )
				{
					id = wstring( StyleIdentifierMap[std->sti] );
				}
				else
				{
					id = FormatUtils::IntToWideString( std->sti );
				}
			}
			else
			{
				//if no identifier is set, use the unique id.
				map<std::wstring, std::wstring>::const_iterator findResult = m_mapStyleId.find(name);
				if( findResult != m_mapStyleId.end() )
				{
					id		= findResult->second;
				}
				else
				{
					//genarate new
					m_mapStyleIdLock.Enter();
					int nIndex = m_mapStyleId.size();
					id = _T("UserStyle_") + FormatUtils::IntToWideString( nIndex );
					pair< std::wstring, std::wstring > p( name, id);
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
		m_pXmlWriter->WriteNodeBegin( _T( "w:style" ), TRUE );

		m_pXmlWriter->WriteAttribute( _T( "w:type" ), _T( "table" ) );
		m_pXmlWriter->WriteAttribute( _T( "w:default" ), _T( "1" ) );
		m_pXmlWriter->WriteAttribute( _T( "w:styleId" ), _T( "TableNormal" ) );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

		m_pXmlWriter->WriteNodeBegin( _T( "w:name" ), TRUE );
		m_pXmlWriter->WriteAttribute( _T( "w:val" ), _T( "Normal Table" ) );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

		m_pXmlWriter->WriteNodeBegin( _T( "w:uiPriority" ), TRUE );
		m_pXmlWriter->WriteAttribute( _T( "w:val" ), _T( "99" ) );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

		m_pXmlWriter->WriteNodeBegin( _T( "w:semiHidden" ), TRUE );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

		m_pXmlWriter->WriteNodeBegin( _T( "w:unhideWhenUsed" ), TRUE );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

		m_pXmlWriter->WriteNodeBegin( _T( "w:qFormat" ), TRUE );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

		m_pXmlWriter->WriteNodeBegin( _T( "w:tblPr" ) );

		m_pXmlWriter->WriteNodeBegin( _T( "w:tblInd" ), TRUE );
		m_pXmlWriter->WriteAttribute( _T( "w:w" ), _T( "0" ) );
		m_pXmlWriter->WriteAttribute( _T( "w:type" ), _T( "dxa" ) );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

		m_pXmlWriter->WriteNodeBegin( _T( "w:tblCellMar" ) );

		m_pXmlWriter->WriteNodeBegin( _T( "w:top" ), TRUE );
		m_pXmlWriter->WriteAttribute( _T( "w:w" ), _T( "0" ) );
		m_pXmlWriter->WriteAttribute( _T( "w:type" ), _T( "dxa" ) );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

		m_pXmlWriter->WriteNodeBegin( _T( "w:left" ), TRUE );
		m_pXmlWriter->WriteAttribute( _T( "w:w" ), _T( "108" ) );
		m_pXmlWriter->WriteAttribute( _T( "w:type" ), _T( "dxa" ) );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

		m_pXmlWriter->WriteNodeBegin( _T( "w:bottom" ), TRUE );
		m_pXmlWriter->WriteAttribute( _T( "w:w" ), _T( "0" ) );
		m_pXmlWriter->WriteAttribute( _T( "w:type" ), _T( "dxa" ) );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

		m_pXmlWriter->WriteNodeBegin( _T( "w:right" ), TRUE );
		m_pXmlWriter->WriteAttribute( _T( "w:w" ), _T( "108" ) );
		m_pXmlWriter->WriteAttribute( _T( "w:type" ), _T( "dxa" ) );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

		m_pXmlWriter->WriteNodeEnd( _T( "w:tblCellMar" ) ); 
		m_pXmlWriter->WriteNodeEnd( _T( "w:tblPr" ) );  
		m_pXmlWriter->WriteNodeEnd( _T( "w:style" ) );  
	}
}