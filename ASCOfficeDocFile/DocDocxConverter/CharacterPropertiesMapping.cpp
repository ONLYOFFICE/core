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

#include "CharacterPropertiesMapping.h"

namespace DocFileFormat
{
	CharacterPropertiesMapping::CharacterPropertiesMapping( XmlUtils::CXmlWriter* writer, WordDocument* doc, RevisionData* rev, ParagraphPropertyExceptions* currentPapx, bool styleChpx, bool isRunStyleNeeded)
		: PropertiesMapping( writer ), _isRunStyleNeeded(isRunStyleNeeded), _isOwnRPr(true), _isRTL(false)
	{
		_doc			= doc;
		_rPr			= new XMLTools::XMLElement<wchar_t>( _T( "w:rPr" ) );
		_revisionData	= rev;
		_currentPapx	= currentPapx;
		_styleChpx		= styleChpx;
		_currentIstd	= USHRT_MAX;
		_webHidden		= false;
	}

	CharacterPropertiesMapping::CharacterPropertiesMapping( XMLTools::XMLElement<wchar_t>* rPr, WordDocument* doc, RevisionData* rev, ParagraphPropertyExceptions* currentPapx, bool styleChpx, bool isRunStyleNeeded )
		: PropertiesMapping( NULL ), _isRunStyleNeeded(isRunStyleNeeded), _isOwnRPr(false), _isRTL(false)   
	{
		_doc			= doc;
		_rPr			= rPr;
		_revisionData	= rev;
		_currentPapx	= currentPapx;
		_styleChpx		= styleChpx;
		_currentIstd	= USHRT_MAX;
		_webHidden		= false;
	}

	CharacterPropertiesMapping::~CharacterPropertiesMapping()
	{
		if (_isOwnRPr)
		{
			RELEASEOBJECT(_rPr);
		}
	}
}

namespace DocFileFormat
{
	void CharacterPropertiesMapping::Apply( IVisitable* chpx )
	{
		//convert the normal SPRMS
		convertSprms( dynamic_cast<CharacterPropertyExceptions*>( chpx )->grpprl, _rPr );

		// apend revision changes
		if (_revisionData->Type == Changed)
		{
			XMLTools::XMLElement<wchar_t> rPrChange( _T( "w:rPrChange" ) );

			//todooo date - _revisionData->Dttm.Convert( new DateMapping( rPrChange ) );

			WideString* author_str = static_cast<WideString*>( _doc->RevisionAuthorTable->operator []( _revisionData->Isbt ));
			
			XMLTools::XMLAttribute<wchar_t> author( _T( "w:author" ), FormatUtils::XmlEncode(*author_str).c_str());
			rPrChange.AppendAttribute( author );

			//convert revision stack
			convertSprms( _revisionData->Changes, &rPrChange );

			_rPr->AppendChild( rPrChange );
		}

		//write properties
		if ( ( m_pXmlWriter != NULL ) && ( ( _rPr->GetChildCount() > 0 ) || ( _rPr->GetAttributeCount() > 0 ) ) )
		{
			m_pXmlWriter->WriteString( _rPr->GetXMLString().c_str() );
		}
	}

	/*========================================================================================================*/

	bool CharacterPropertiesMapping::CheckIsSymbolFont()
	{
		//Todo сделать определение симольного шрифта через fontManager
		//Заглушка под Google Docs, они пишут bullet в Arial
		if (-1 != m_sAsciiFont.find (_T("Arial")) && -1 != m_sEastAsiaFont.find (_T("Arial")) && -1 != m_shAnsiFont.find (_T("Arial")))
			return false;

		return true;
	}

	/*========================================================================================================*/

	void CharacterPropertiesMapping::convertSprms( std::list<SinglePropertyModifier>* sprms, XMLTools::XMLElement<wchar_t>* parent )
	{
		XMLTools::XMLElement<wchar_t>	* rFonts	= new XMLTools::XMLElement<wchar_t>		( _T( "w:rFonts" ) );
		XMLTools::XMLElement<wchar_t>	* color		= new XMLTools::XMLElement<wchar_t>		( _T( "w:color" ) );
		XMLTools::XMLAttribute<wchar_t>	* colorVal	= new XMLTools::XMLAttribute<wchar_t>	( _T( "w:val" ) );
		XMLTools::XMLElement<wchar_t>	* lang		= new XMLTools::XMLElement<wchar_t>		( _T( "w:lang" ) );

		if (_webHidden)
		{
			XMLTools::XMLElement<wchar_t>	* webHidden		= new XMLTools::XMLElement<wchar_t>	( _T( "w:webHidden" ) );
			parent->AppendChild( *webHidden );
			RELEASEOBJECT( webHidden );
		}
		std::list<SinglePropertyModifier>::iterator end = sprms->end();
		for (std::list<SinglePropertyModifier>::iterator iter = sprms->begin(); iter != end; ++iter)
		{
			int nProperty = 0; //for unknown test

			switch ( (int)( iter->OpCode ) )
			{
			case sprmOldCIstd :
			case sprmCIstd :	//	style id 
				{
					if (_isRunStyleNeeded && !_webHidden)
					{
						_currentIstd = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
						if (_currentIstd < _doc->Styles->Styles->size())
						{
							appendValueElement( parent, _T( "rStyle" ), StyleSheetMapping::MakeStyleId( _doc->Styles->Styles->at( _currentIstd ) ).c_str(), true );
						}
					}
				}break;	

			case sprmCFBiDi :
				appendFlagElement( parent, *iter, _T( "rtl" ), true );
				_isRTL = true;
				break;

			case sprmOldCFBold :
			case sprmCFBold :
				appendFlagElement( parent, *iter, _T( "b" ), true );
				break;

			case sprmCFBoldBi :
				appendFlagElement( parent, *iter, _T( "bCs" ), true );
				break;

			case sprmOldCFCaps :
			case sprmCFCaps :
				appendFlagElement( parent, *iter, _T( "caps" ), true );
				break;

			case sprmCFComplexScripts :
				appendFlagElement( parent, *iter, _T( "cs" ), true );
				break;

			case sprmCFDStrike :
				appendFlagElement( parent, *iter, _T( "dstrike" ), true );
				break;

			case sprmCFEmboss :
				appendFlagElement( parent, *iter, _T( "emboss" ), true );
				break;

			case sprmCFImprint :
				appendFlagElement( parent, *iter, _T( "imprint" ), true );
				break;

			case sprmOldCFItalic :
			case sprmCFItalic :
				appendFlagElement( parent, *iter, _T( "i" ), true );
				break;

			case sprmCFItalicBi:
				appendFlagElement( parent, *iter, _T( "iCs" ), true );
				break;

			case 0x0875:
				appendFlagElement( parent, *iter, _T( "noProof" ), true );
				break;

			case sprmOldCFOutline:
			case sprmCFOutline:
				appendFlagElement( parent, *iter, _T( "outline" ), true );
				break;

			case sprmOldCFShadow:
			case sprmCFShadow:
				appendFlagElement( parent, *iter, _T( "shadow" ), true );
				break;

			case sprmOldCFSmallCaps:
			case sprmCFSmallCaps:
				appendFlagElement( parent, *iter, _T( "smallCaps" ), true );
				break;

			case sprmCFSpecVanish:
				appendFlagElement( parent, *iter, _T( "specVanish" ), true );
				break;

			case sprmOldCFStrike:
			case sprmCFStrike:
				appendFlagElement( parent, *iter, _T( "strike" ), true );
				break;

			case sprmOldCFVanish:
			case sprmCFVanish:
				appendFlagElement( parent, *iter, _T( "vanish" ), true );
				break;

			case 0x0811:
				appendFlagElement( parent, *iter, _T( "webHidden" ), true );
				break;

			case sprmOldCIss:
			case sprmCIss:
				appendValueElement( parent, _T( "vertAlign" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &SuperscriptIndex[0][0], 3, 12 ).c_str(), true );
				break;	

			case sprmCRgLid0_80:
			case sprmCRgLid0:
				{	//latin					
					LanguageId langid( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );

					LanguageIdMapping* langIDMapping = new LanguageIdMapping( lang, Default );

					langid.Convert( langIDMapping );

					RELEASEOBJECT( langIDMapping );
				}break;	

			case sprmOldCLid:
			case sprmCRgLid1_80:
			case sprmCRgLid1:
				{	//east asia				
					LanguageId langid( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );

					LanguageIdMapping* langIDMapping = new LanguageIdMapping( lang, EastAsian );

					langid.Convert( langIDMapping );

					RELEASEOBJECT( langIDMapping );
				}break;

			case sprmCLidBi:
				{					
					LanguageId langid( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );

					LanguageIdMapping* langIDMapping = new LanguageIdMapping( lang, Complex );

					langid.Convert( langIDMapping );

					RELEASEOBJECT( langIDMapping );
				}break;	

			case sprmCBrc80:
			case sprmCBrc:
				{  //borders
					XMLTools::XMLElement<wchar_t> bdr( _T( "w:bdr" ) );
					BorderCode bc( iter->Arguments, iter->argumentsSize );
					appendBorderAttributes( &bc, &bdr );
					parent->AppendChild( bdr );
				}break;		

			case sprmCShd80:
			case sprmCShd:
				{  //shading
					ShadingDescriptor desc( iter->Arguments, iter->argumentsSize );

					appendShading( parent, desc );
				}break;		

			case sprmOldCIco:
			case sprmCIco:
			case sprmCIcoBi:
				{//color
					colorVal->SetValue( FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::ColorIdentifier[0][0], 17, 12 ).c_str() );
				}break;

			case sprmCCv:
				{
                    CString rgbColor;

                    rgbColor.Format( _T( "%02x%02x%02x" ), /*R*/iter->Arguments[0], /*G*/iter->Arguments[1], /*B*/iter->Arguments[2] );

                    colorVal->SetValue( rgbColor.GetString() );
				}break;	

			case sprmCHighlight:
				{
					appendValueElement( parent, _T( "highlight" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::ColorIdentifier[0][0], 17, 12 ).c_str(), true );
				}break;	

			case sprmOldCDxaSpace:
			case sprmCDxaSpace:
				{
					appendValueElement( parent, _T( "spacing" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
				}break;

			case sprmCFtcBi :
				{//default from FontTable
					SHORT nIndex	=	FormatUtils::BytesToUInt16 (iter->Arguments, 0, iter->argumentsSize);
					if( nIndex < _doc->FontTable->Data.size() )
					{
						FontFamilyName* ffn = static_cast<FontFamilyName*>( _doc->FontTable->operator [] ( nIndex ) );
						if (ffn)
							m_sDefaultFont = ffn->xszFtn;
					}
				}break;

			case sprmCHpsBi :
				{
					appendValueElement( parent, _T( "szCs" ), 
						FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );						
				}
				break;
// Font Size in points (2~3276) default 20-half-points
			case sprmOldCHps :
				{
					appendValueElement (parent, _T( "sz" ), 
						FormatUtils::IntToWideString (FormatUtils::BytesToUChar (iter->Arguments, 0, iter->argumentsSize) ).c_str(),
						true );
				}break;
			case sprmCHps : 
				{  
					appendValueElement (parent, _T( "sz" ), 
						FormatUtils::IntToWideString (FormatUtils::BytesToUInt16 (iter->Arguments, 0, iter->argumentsSize) ).c_str(), true );						
				}break;

			case sprmCMajority :
				{	//for complex props
				}break;

			case sprmOldCHpsPos: 
				{	// The vertical position, in half-points, of text relative to the normal position. (MUST be between -3168 and 3168)
					short nVertPos = FormatUtils::BytesToUChar(iter->Arguments, 0, iter->argumentsSize);
					appendValueElement (parent, _T("position"), nVertPos, true);
				}break;
			case sprmCHpsPos: 
				{	// The vertical position, in half-points, of text relative to the normal position. (MUST be between -3168 and 3168)
					short nVertPos = FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize);
					appendValueElement (parent, _T("position"), nVertPos, true);
				}break;

			case sprmOldCHpsKern:
			case sprmCHpsKern:
				{
					appendValueElement( parent, _T( "kern" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
				}break;

			case sprmOldCFtc:
			case sprmCRgFtc0:
				{	// font family
					int nIndex = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
					
					if( nIndex < _doc->FontTable->Data.size() )
					{
						XMLTools::XMLAttribute<wchar_t>* ascii = new XMLTools::XMLAttribute<wchar_t>( _T( "w:ascii" ) );
						FontFamilyName* ffn = static_cast<FontFamilyName*>( _doc->FontTable->operator [] ( nIndex ) );
						m_sAsciiFont = ffn->xszFtn;
						ascii->SetValue( FormatUtils::XmlEncode(m_sAsciiFont).c_str() );
						rFonts->AppendAttribute( *ascii );
						RELEASEOBJECT( ascii );
					}
				}break;

			case sprmCRgFtc1:
				{
					int nIndex = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
					if( nIndex >= 0 && nIndex < _doc->FontTable->Data.size() )
					{
						XMLTools::XMLAttribute<wchar_t>* eastAsia = new XMLTools::XMLAttribute<wchar_t>( _T( "w:eastAsia" ) );
						FontFamilyName* ffn = static_cast<FontFamilyName*>( _doc->FontTable->operator [] ( nIndex ) );
						m_sEastAsiaFont = ffn->xszFtn;
						eastAsia->SetValue( FormatUtils::XmlEncode(m_sEastAsiaFont).c_str() );
						rFonts->AppendAttribute( *eastAsia );
						RELEASEOBJECT( eastAsia );
					}
				}
				break;

			case sprmCRgFtc2:
				{
					int nIndex = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
					if( nIndex>=0 && nIndex < _doc->FontTable->Data.size() )
					{
						XMLTools::XMLAttribute<wchar_t>* ansi = new XMLTools::XMLAttribute<wchar_t>( _T( "w:hAnsi" ) );
						FontFamilyName* ffn = static_cast<FontFamilyName*>( _doc->FontTable->operator [] ( nIndex ) );
						m_shAnsiFont = ffn->xszFtn;
						ansi->SetValue( FormatUtils::XmlEncode(m_shAnsiFont).c_str() );
						rFonts->AppendAttribute( *ansi );
						RELEASEOBJECT( ansi );
					}
				}break;			

			case sprmOldCKul:
			case sprmCKul:
				{	//Underlining
					appendValueElement( parent, _T( "u" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::UnderlineCode[0][0], 56, 16 ).c_str(), true );
				}
				break;		

			case sprmCCharScale:
				{	//char width
					appendValueElement( parent, _T( "w" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
				}break;	
							
			case sprmCSfxText:
				{	//animation
					appendValueElement( parent, _T( "effect" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::TextAnimation[0][0], 7, 16 ).c_str(), true );
				}break;	

			case sprmCIdctHint:
				{
					switch(iter->Arguments[0])
					{
					case 0:		break;	// default
					case 1:		break;	// eastAsia
					case 2:		break;	// cs
					case 0xFF:	break;	//No ST_Hint equivalent
					}

				}break;

			case sprmCPbiIBullet:
				{
					int nIndex = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
					if (nIndex >=0)
					{
						std::map<int, int>::iterator it = _doc->PictureBulletsCPsMap.find(nIndex);
						if (it != _doc->PictureBulletsCPsMap.end())
						{
							//добавить
						}
					}
				}break;

			case sprmCPbiGrf:
				{
					//used picture bullet
					int val = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );

				}break;

			case sprmCRsidProp:
			case sprmCRsidText:
				break;

			default:
				if (iter->argumentsSize == 2)
				{
					nProperty = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
				}else
					if (iter->argumentsSize == 1)
				{
					nProperty = FormatUtils::BytesToUChar( iter->Arguments, 0, iter->argumentsSize );
				}
				break;
			}
		}

		if (!m_sDefaultFont.empty() && m_sAsciiFont.empty() && m_sEastAsiaFont.empty() && m_shAnsiFont.empty())
		{//????
			XMLTools::XMLAttribute<wchar_t>* ascii = new XMLTools::XMLAttribute<wchar_t>( _T( "w:ascii" ) );
			ascii->SetValue( FormatUtils::XmlEncode(m_sDefaultFont).c_str() );
			//rFonts->AppendAttribute( *ascii );
			RELEASEOBJECT( ascii );
		}

		//apend lang
		if ( lang->GetAttributeCount() > 0 )
		{
			parent->AppendChild( *lang );
		}

		//append fonts
		if ( rFonts->GetAttributeCount() > 0 )
		{
			parent->AppendChild( *rFonts );
		}

		//append color
		if ( colorVal->GetValue() != _T( "" ) )
		{
			color->AppendAttribute( *colorVal );
			parent->AppendChild( *color );
		}

		RELEASEOBJECT( lang );
		RELEASEOBJECT( colorVal );
		RELEASEOBJECT( color );
		RELEASEOBJECT( rFonts );
	}

	/*========================================================================================================*/

	/// CHPX flags are special flags because the can be 0,1,128 and 129,
	/// so this method overrides the appendFlagElement method.
	void CharacterPropertiesMapping::appendFlagElement( XMLTools::XMLElement<wchar_t>* node, const SinglePropertyModifier& sprm, const wchar_t* elementName, bool unique )
	{
		unsigned char flag = sprm.Arguments[0];

		if( flag != 128 )
		{
			XMLTools::XMLElement<wchar_t>* ele = new XMLTools::XMLElement<wchar_t>( _T( "w" ), elementName );
			XMLTools::XMLAttribute<wchar_t>* val = new XMLTools::XMLAttribute<wchar_t>( _T( "w:val" ) );

			if ( unique )
			{
				node->RemoveChild( *ele );
			}

			if ( flag == 0 )
			{
				val->SetValue( _T( "false" ) );
				ele->AppendAttribute( *val );
				node->AppendChild( *ele );
			}
			else if (flag == 1)
			{
				//dont append attribute val
				//no attribute means true
				node->AppendChild( *ele );
			}
			else if( flag == 129 )
			{
				//Invert the value of the style

				//determine the style id of the current style
				unsigned short styleId = 0;

				if ( _currentIstd != USHRT_MAX )
				{
					styleId = _currentIstd;
				}
				else if( _currentPapx != NULL )
				{
					styleId = _currentPapx->istd;
				}

				//this chpx is the chpx of a style, 
				//don't use the id of the chpx or the papx, use the baseOn style
				if ( _styleChpx )
				{
					StyleSheetDescription* thisStyle = _doc->Styles->Styles->at( styleId );
					styleId = (unsigned short)thisStyle->istdBase;
				}

				//build the style hierarchy
				_hierarchy = buildHierarchy( _doc->Styles, styleId );

				//apply the toggle values to get the real value of the style
				bool stylesVal = applyToggleHierachy( sprm ); 

				//invert it
				if ( stylesVal )
				{
					val->SetValue( _T( "false" ) );
					ele->AppendAttribute( *val );
				}

				node->AppendChild( *ele );
			}

			RELEASEOBJECT(ele);
			RELEASEOBJECT(val);
		}
	}

	/*========================================================================================================*/

	std::list<CharacterPropertyExceptions*> CharacterPropertiesMapping::buildHierarchy( const StyleSheet* styleSheet, unsigned short istdStart )
	{
		std::list<CharacterPropertyExceptions*> hierarchy;
		unsigned int istd = (unsigned int)istdStart;
		bool goOn = true;

		if ( ( styleSheet != NULL ) && ( styleSheet->Styles != NULL ) )
		{
			while ( goOn )
			{
				try
				{
					if ( istd < styleSheet->Styles->size() )
					{
						CharacterPropertyExceptions* baseChpx = styleSheet->Styles->at( istd )->chpx;

						if ( baseChpx != NULL )
						{
							hierarchy.push_back( baseChpx );
							istd = (unsigned int)styleSheet->Styles->at( istd )->istdBase;
						}
						else
						{
							goOn = false;
						}
					}
					else
					{
						goOn = false;
					}
				}
				catch (...)
				{
					goOn = false;
				}
			}
		}

		return hierarchy;
	}

	/*========================================================================================================*/

	bool CharacterPropertiesMapping::applyToggleHierachy( const SinglePropertyModifier& sprm )
	{
		bool ret = false;

		std::list<CharacterPropertyExceptions*>::const_iterator end = _hierarchy.end();
		for (std::list<CharacterPropertyExceptions*>::const_iterator iter = _hierarchy.begin(); iter != end; ++iter)        
		{
			std::list<SinglePropertyModifier>::const_iterator end_grpprl = (*iter)->grpprl->end();
			for (std::list<SinglePropertyModifier>::const_iterator grpprlIter = (*iter)->grpprl->begin(); grpprlIter != end_grpprl; ++grpprlIter)	 
			{
				if (grpprlIter->OpCode == sprm.OpCode)
				{
					unsigned char ancient = grpprlIter->Arguments[0];
					ret = toogleValue(ret, ancient);
					break;
				}
			}
		}

		return ret;
	}

	/*========================================================================================================*/

	bool CharacterPropertiesMapping::toogleValue(bool currentValue, unsigned char toggle)
	{
		if ( toggle == 1 )
		{
			return true;
		}
		else if ( toggle == 129 )
		{
			//invert the current value
			if ( currentValue )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if ( toggle == 128 )
		{
			//use the current value
			return currentValue;
		}
		else
		{
			return false;
		}
	}
}
