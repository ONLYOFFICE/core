#include "stdafx.h"
#include "CharacterPropertiesMapping.h"

namespace DocFileFormat
{
	CharacterPropertiesMapping::CharacterPropertiesMapping( XmlUtils::CXmlWriter* writer, WordDocument* doc, RevisionData* rev, ParagraphPropertyExceptions* currentPapx, bool styleChpx, bool isRunStyleNeeded) : PropertiesMapping( writer ), _isRunStyleNeeded(isRunStyleNeeded), _isOwnRPr(true)
	{
		this->_doc = doc;
		this->_rPr = new XMLTools::XMLElement<WCHAR>( _T( "w:rPr" ) );
		this->_revisionData = rev;
		this->_currentPapx = currentPapx;
		this->_styleChpx = styleChpx;
		this->_currentIstd = USHRT_MAX;
	}

	CharacterPropertiesMapping::CharacterPropertiesMapping( XMLTools::XMLElement<WCHAR>* rPr, WordDocument* doc, RevisionData* rev, ParagraphPropertyExceptions* currentPapx, bool styleChpx, bool isRunStyleNeeded ): PropertiesMapping( NULL ), _isRunStyleNeeded(isRunStyleNeeded), _isOwnRPr(false)    
	{
		this->_doc = doc;
		this->_rPr = rPr;
		this->_revisionData = rev;
		this->_currentPapx = currentPapx;
		this->_styleChpx = styleChpx;
		this->_currentIstd = USHRT_MAX;
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
		convertSprms( dynamic_cast<CharacterPropertyExceptions*>( chpx )->grpprl, this->_rPr );

		// apend revision changes
		if (_revisionData->Type == Changed)
		{
			XMLTools::XMLElement<WCHAR> rPrChange( _T( "w:rPrChange" ) );

			//!!!TODO!!!
			//date
			//_revisionData->Dttm.Convert( new DateMapping( rPrChange ) );

			//author
			XMLTools::XMLAttribute<WCHAR> author( _T( "w:author" ), static_cast<WideString*>( this->_doc->RevisionAuthorTable->operator []( _revisionData->Isbt ) )->c_str() );
			rPrChange.AppendAttribute( author );

			//convert revision stack
			convertSprms( this->_revisionData->Changes, &rPrChange );

			this->_rPr->AppendChild( rPrChange );
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
		if (-1 != this->m_sAsciiFont.find (_T("Arial")) && -1 != this->m_sEastAsiaFont.find (_T("Arial")) && -1 != this->m_shAnsiFont.find (_T("Arial")))
			return false;

		return true;
	}

	/*========================================================================================================*/

	void CharacterPropertiesMapping::convertSprms( list<SinglePropertyModifier>* sprms, XMLTools::XMLElement<WCHAR>* parent )
	{
		XMLTools::XMLElement<WCHAR>* rFonts = new XMLTools::XMLElement<WCHAR>( _T( "w:rFonts" ) );
		XMLTools::XMLElement<WCHAR>* color = new XMLTools::XMLElement<WCHAR>( _T( "w:color" ) );
		XMLTools::XMLAttribute<WCHAR>* colorVal = new XMLTools::XMLAttribute<WCHAR>( _T( "w:val" ) );
		XMLTools::XMLElement<WCHAR>* lang = new XMLTools::XMLElement<WCHAR>( _T( "w:lang" ) );

		// флаг наличия стиля для баги - http://bugzserver/show_bug.cgi?id=13353 	TODO : найти в чем создан такой документ
		BOOL haveStyle	=	FALSE;	
		
		std::list<SinglePropertyModifier>::iterator end = sprms->end();
		for (std::list<SinglePropertyModifier>::iterator iter = sprms->begin(); iter != end; ++iter)
		{
			switch ( (int)( iter->OpCode ) )
			{				
			case 0x4A30 :	//	style id 
				{
					if (_isRunStyleNeeded)
					{
						_currentIstd = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
						appendValueElement( parent, _T( "rStyle" ), StyleSheetMapping::MakeStyleId( this->_doc->Styles->Styles->at( _currentIstd ) ).c_str(), true );

						haveStyle	=	TRUE;
					}
				}
				break;

				
			case 0x085A :	//	Element flags
				appendFlagElement( parent, *iter, _T( "rtl" ), true );
				break;

			case 0x0835 :
				appendFlagElement( parent, *iter, _T( "b" ), true );
				break;

			case 0x085C :
				appendFlagElement( parent, *iter, _T( "bCs" ), true );
				break;

			case 0x083B :
				appendFlagElement( parent, *iter, _T( "caps" ), true );
				break;

			case 0x0882 :
				appendFlagElement( parent, *iter, _T( "cs" ), true );
				break;

			case 0x2A53 :
				appendFlagElement( parent, *iter, _T( "dstrike" ), true );
				break;

			case 0x0858 :
				appendFlagElement( parent, *iter, _T( "emboss" ), true );
				break;

			case 0x0854 :
				appendFlagElement( parent, *iter, _T( "imprint" ), true );
				break;

			case 0x0836 :
				appendFlagElement( parent, *iter, _T( "i" ), true );
				break;

			case 0x085D:
				appendFlagElement( parent, *iter, _T( "iCs" ), true );
				break;

			case 0x0875:
				appendFlagElement( parent, *iter, _T( "noProof" ), true );
				break;

			case 0x0838:
				appendFlagElement( parent, *iter, _T( "outline" ), true );
				break;

			case 0x0839:
				appendFlagElement( parent, *iter, _T( "shadow" ), true );
				break;

			case 0x083A:
				appendFlagElement( parent, *iter, _T( "smallCaps" ), true );
				break;

			case 0x0818:
				appendFlagElement( parent, *iter, _T( "specVanish" ), true );
				break;

			case 0x0837:
				appendFlagElement( parent, *iter, _T( "strike" ), true );
				break;

			case 0x083C:
				appendFlagElement( parent, *iter, _T( "vanish" ), true );
				break;

			case 0x0811:
				appendFlagElement( parent, *iter, _T( "webHidden" ), true );
				break;

			case 0x2A48:
				appendValueElement( parent, _T( "vertAlign" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &SuperscriptIndex[0][0], 3, 12 ).c_str(), true );
				break;

				//language
			case 0x486D:
			case 0x4873:
				{
					//latin
					LanguageId langid( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );

					LanguageIdMapping* langIDMapping = new LanguageIdMapping( lang, Default );

					langid.Convert( langIDMapping );

					RELEASEOBJECT( langIDMapping );
				}
				break;

			case 0x486E:
			case 0x4874:
				{
					//east asia
					LanguageId langid( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );

					LanguageIdMapping* langIDMapping = new LanguageIdMapping( lang, EastAsian );

					langid.Convert( langIDMapping );

					RELEASEOBJECT( langIDMapping );
				}
				break;

			case 0x485F:
				{
					//bidi
					LanguageId langid( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );

					LanguageIdMapping* langIDMapping = new LanguageIdMapping( lang, Complex );

					langid.Convert( langIDMapping );

					RELEASEOBJECT( langIDMapping );
				}
				break;

				//borders
			case 0x6865:
			case 0xCA72:
				{  
					XMLTools::XMLElement<WCHAR> bdr( _T( "w:bdr" ) );
					BorderCode bc( iter->Arguments, iter->argumentsSize );
					appendBorderAttributes( &bc, &bdr );
					parent->AppendChild( bdr );
				}
				break;

				//shading
			case 0x4866:
			case 0xCA71:
				{  
					ShadingDescriptor desc( iter->Arguments, iter->argumentsSize );

					appendShading( parent, desc );
				}
				break;

				//color
			case 0x2A42:
			case 0x4A60:
				colorVal->SetValue( FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::ColorIdentifier[0][0], 17, 12 ).c_str() );
				break;

			case 0x6870:
				{
					WCHAR rgbColor[7];

					swprintf_s( rgbColor, _T( "%02x%02x%02x" ), /*R*/iter->Arguments[0], /*G*/iter->Arguments[1], /*B*/iter->Arguments[2] );

					colorVal->SetValue( rgbColor );
				}
				break;

				//highlightning
			case 0x2A0C:
				appendValueElement( parent, _T( "highlight" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::ColorIdentifier[0][0], 17, 12 ).c_str(), true );
				break;

				//spacing
			case 0x8840:
				{
					appendValueElement( parent, _T( "spacing" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
				}
				break;

			case sprmCFtcBi :
				{
					//SHORT fontIndex	=	FormatUtils::BytesToUInt16 (iter->Arguments, 0, iter->argumentsSize);
					//ATLTRACE ( _T("fontIndex : %d\n"), fontIndex);
				}
				break;

			case sprmCHpsBi :
				{
					//SHORT fontSize	=	FormatUtils::BytesToUInt16 (iter->Arguments, 0, iter->argumentsSize);
					//ATLTRACE ( _T("CHpsBi : %d\n"), fontSize);

					if (FALSE == haveStyle)
					{
						appendValueElement( parent, _T( "szCs" ), 
							FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(),
							true );
					}
				}
				break;


			case sprmCHps : // Font Size in points (2~3276) default 20-half-points
				{  
					//SHORT fontSize	=	FormatUtils::BytesToUInt16 (iter->Arguments, 0, iter->argumentsSize);
					//ATLTRACE ( _T("CHps : %d\n"), fontSize);
					
					if (FALSE == haveStyle)
					{					
						appendValueElement (parent, _T( "sz" ), 
							FormatUtils::IntToWideString (FormatUtils::BytesToUInt16 (iter->Arguments, 0, iter->argumentsSize) ).c_str(),
							true );
					}
				}
				break;

			case sprmCHpsPos: // The vertical position, in half-points, of text relative to the normal position. (MUST be between -3168 and 3168)
				{
					short nVertPos = FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize);
					appendValueElement (parent, _T("position"), nVertPos, true);
				}
				break;

			case sprmCHpsKern:
				{
					appendValueElement( parent, _T( "kern" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
				}
				break;

			case sprmCRgFtc0:	//	font family
				{
					int nIndex = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
					if( nIndex < _doc->FontTable->cData )
					{
						XMLTools::XMLAttribute<WCHAR>* ascii = new XMLTools::XMLAttribute<WCHAR>( _T( "w:ascii" ) );
						FontFamilyName* ffn = static_cast<FontFamilyName*>( _doc->FontTable->operator [] ( nIndex ) );
						this->m_sAsciiFont = ffn->xszFtn;
						ascii->SetValue( FormatUtils::XmlEncode(m_sAsciiFont).c_str() );
						rFonts->AppendAttribute( *ascii );
						RELEASEOBJECT( ascii );
					}
				}
				break;

			case 0x4A50:
				{
					int nIndex = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
					if( nIndex>=0 && nIndex < _doc->FontTable->cData )
					{
						XMLTools::XMLAttribute<WCHAR>* eastAsia = new XMLTools::XMLAttribute<WCHAR>( _T( "w:eastAsia" ) );
						FontFamilyName* ffn = static_cast<FontFamilyName*>( _doc->FontTable->operator [] ( nIndex ) );
						this->m_sEastAsiaFont = ffn->xszFtn;
						eastAsia->SetValue( FormatUtils::XmlEncode(this->m_sEastAsiaFont).c_str() );
						rFonts->AppendAttribute( *eastAsia );
						RELEASEOBJECT( eastAsia );
					}
				}
				break;

			case 0x4A51:
				{
					int nIndex = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
					if( nIndex>=0 && nIndex < _doc->FontTable->cData )
					{
						XMLTools::XMLAttribute<WCHAR>* ansi = new XMLTools::XMLAttribute<WCHAR>( _T( "w:hAnsi" ) );
						FontFamilyName* ffn = static_cast<FontFamilyName*>( _doc->FontTable->operator [] ( nIndex ) );
						this->m_shAnsiFont = ffn->xszFtn;
						ansi->SetValue( FormatUtils::XmlEncode(this->m_shAnsiFont).c_str() );
						rFonts->AppendAttribute( *ansi );
						RELEASEOBJECT( ansi );
					}
				}
				break;

				//Underlining
			case 0x2A3E:
				{
					appendValueElement( parent, _T( "u" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::UnderlineCode[0][0], 56, 16 ).c_str(), true );
				}
				break;

				//char width
			case 0x4852:
				{
					appendValueElement( parent, _T( "w" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
				}
				break;

				//animation
			case 0x2859:
				{
					appendValueElement( parent, _T( "effect" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::TextAnimation[0][0], 7, 16 ).c_str(), true );
				}
				break;
							
			default:
#ifdef _DEBUG 
				// ATLTRACE (_T("CharacterPropertiesMapping - UNKNOWN SPRM : 0x%x\n"), iter->OpCode);
#endif
				break;
			}
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
	void CharacterPropertiesMapping::appendFlagElement( XMLTools::XMLElement<WCHAR>* node, const SinglePropertyModifier& sprm, const WCHAR* elementName, bool unique )
	{
		byte flag = sprm.Arguments[0];

		if( flag != 128 )
		{
			XMLTools::XMLElement<WCHAR>* ele = new XMLTools::XMLElement<WCHAR>( _T( "w" ), elementName );
			XMLTools::XMLAttribute<WCHAR>* val = new XMLTools::XMLAttribute<WCHAR>( _T( "w:val" ) );

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
					StyleSheetDescription* thisStyle = this->_doc->Styles->Styles->at( styleId );
					styleId = (unsigned short)thisStyle->istdBase;
				}

				//build the style hierarchy
				this->_hierarchy = buildHierarchy( this->_doc->Styles, styleId );

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

	list<CharacterPropertyExceptions*> CharacterPropertiesMapping::buildHierarchy( const StyleSheet* styleSheet, unsigned short istdStart )
	{
		list<CharacterPropertyExceptions*> hierarchy;
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
		for (std::list<CharacterPropertyExceptions*>::const_iterator iter = this->_hierarchy.begin(); iter != end; ++iter)        
		{
			std::list<SinglePropertyModifier>::const_iterator end_grpprl = (*iter)->grpprl->end();
			for (std::list<SinglePropertyModifier>::const_iterator grpprlIter = (*iter)->grpprl->begin(); grpprlIter != end_grpprl; ++grpprlIter)	 
			{
				if (grpprlIter->OpCode == sprm.OpCode)
				{
					byte ancient = grpprlIter->Arguments[0];
					ret = toogleValue(ret, ancient);
					break;
				}
			}
		}

		return ret;
	}

	/*========================================================================================================*/

	bool CharacterPropertiesMapping::toogleValue(bool currentValue, byte toggle)
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