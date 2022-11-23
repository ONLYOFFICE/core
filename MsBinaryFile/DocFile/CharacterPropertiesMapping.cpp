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

#include "CharacterPropertiesMapping.h"

namespace DocFileFormat
{
	CharacterPropertiesMapping::CharacterPropertiesMapping( XMLTools::CStringXmlWriter* writer, WordDocument* doc, RevisionData* rev, ParagraphPropertyExceptions* currentPapx, bool styleChpx, bool isRunStyleNeeded)
		: PropertiesMapping( writer ), _isRunStyleNeeded(isRunStyleNeeded), _isOwnRPr(true), _isRTL(false)
	{
		_doc			= doc;
        _rPr			= new XMLTools::XMLElement( L"w:rPr");
		_revisionData	= rev;
		_currentPapx	= currentPapx;
		_styleChpx		= styleChpx;
		_currentIstd	= USHRT_MAX;
		_webHidden		= false;
	}

    CharacterPropertiesMapping::CharacterPropertiesMapping( XMLTools::XMLElement* rPr, WordDocument* doc, RevisionData* rev, ParagraphPropertyExceptions* currentPapx, bool styleChpx, bool isRunStyleNeeded )
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
		if ((_revisionData) && (_revisionData->Type == Changed))
		{
            XMLTools::XMLElement rPrChange( L"w:rPrChange");

			//todooo date - _revisionData->Dttm.Convert( new DateMapping( rPrChange ) );

			WideString* author_str = static_cast<WideString*>( _doc->RevisionAuthorTable->operator []( _revisionData->Isbt ));
			
			if (author_str)
			{
				XMLTools::XMLAttribute author( L"w:author", FormatUtils::XmlEncode(*author_str));
				rPrChange.AppendAttribute( author );
			}
			XMLTools::XMLAttribute date( L"w:date", _revisionData->Dttm.getString());
			rPrChange.AppendAttribute( date );

			XMLTools::XMLElement rPr(L"w:rPr");
			//convert revision stack
			convertSprms( _revisionData->Changes, &rPr);

			rPrChange.AppendChild(rPr);

			_rPr->AppendChild( rPrChange );
		}

		//write properties
		if ( ( m_pXmlWriter != NULL ) && ( ( _rPr->GetChildCount() > 0 ) || ( _rPr->GetAttributeCount() > 0 ) ) )
		{
			m_pXmlWriter->WriteString( _rPr->GetXMLString());
		}
	}

	/*========================================================================================================*/

	bool CharacterPropertiesMapping::CheckIsSymbolFont()
	{
		//Todo сделать определение симольного шрифта через fontManager
		//Заглушка под Google Docs, они пишут bullet в Arial
        if (-1 != m_sAsciiFont.find (L"Arial") && -1 != m_sEastAsiaFont.find (L"Arial") && -1 != m_shAnsiFont.find (L"Arial"))
			return false;

		return true;
	}

	/*========================================================================================================*/

    void CharacterPropertiesMapping::convertSprms( std::list<SinglePropertyModifier>* sprms, XMLTools::XMLElement* parent )
	{
        XMLTools::XMLElement	* rFonts	= new XMLTools::XMLElement		( L"w:rFonts" );
        XMLTools::XMLElement	* color		= new XMLTools::XMLElement		( L"w:color" );
        XMLTools::XMLAttribute	* colorVal	= new XMLTools::XMLAttribute	( L"w:val" );
        XMLTools::XMLElement	* lang		= new XMLTools::XMLElement		( L"w:lang" );

		if (_webHidden)
		{
            XMLTools::XMLElement *webHidden = new XMLTools::XMLElement	( L"w:webHidden" );
			parent->AppendChild( *webHidden );
			RELEASEOBJECT( webHidden );
		}
		if ((sprms) && (!sprms->empty()))
		{
			std::list<SinglePropertyModifier>::iterator end = sprms->end();
			for (std::list<SinglePropertyModifier>::iterator iter = sprms->begin(); iter != end; ++iter)
			{
				int nProperty = 0; //for unknown test

				switch ((int)(iter->OpCode))
				{
					case sprmOldCIstd:
					case sprmCIstd:	//	style id 
					{
						if (_isRunStyleNeeded && !_webHidden)
						{
							_currentIstd = FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize);
							if (_currentIstd < _doc->Styles->Styles->size())
							{
								appendValueElement(parent, L"rStyle", StyleSheetMapping::MakeStyleId(_doc->Styles->Styles->at(_currentIstd)), true);
							}
						}
					}break;
					case sprmCFBiDi:
					{
						appendFlagElement(parent, *iter, L"rtl", true);
						_isRTL = true;
					}break;
					case sprmOldCFBold:
					case sprmCFBold:
					{
						appendFlagElement(parent, *iter, L"b", true);
					}break;
					case sprmCFBoldBi:
					{
						appendFlagElement(parent, *iter, L"bCs", true);
					}break;
					case sprmOldCFCaps:
					case sprmCFCaps:
					{
						appendFlagElement(parent, *iter, L"caps", true);
					}break;
					case sprmCFComplexScripts:
					{
						appendFlagElement(parent, *iter, L"cs", true);
					}break;
					case sprmCFDStrike:
					{
						appendFlagElement(parent, *iter, L"dstrike", true);
					}break;
					case sprmCFEmboss:
					{
						appendFlagElement(parent, *iter, L"emboss", true);
					}break;
					case sprmCFImprint:
					{
						appendFlagElement(parent, *iter, L"imprint", true);
					}break;
					case sprmOldCFItalic:
					case sprmCFItalic:
					{
						appendFlagElement(parent, *iter, L"i", true);
					}break;
					case sprmCFItalicBi:
					{
						appendFlagElement(parent, *iter, L"iCs", true);
					}break;
					case 0x0875:
					{
						appendFlagElement(parent, *iter, L"noProof", true);
					}break;
					case sprmOldCFOutline:
					case sprmCFOutline:
					{
						appendFlagElement(parent, *iter, L"outline", true);
					}break;
					case sprmOldCFShadow:
					case sprmCFShadow:
					{
						appendFlagElement(parent, *iter, L"shadow", true);
					}break;
					case sprmOldCFSmallCaps:
					case sprmCFSmallCaps:
					{
						appendFlagElement(parent, *iter, L"smallCaps", true);
					}break;
					case sprmCFSpecVanish:
					{
						appendFlagElement(parent, *iter, L"specVanish", true);
					}break;
					case sprmOldCFStrike:
					case sprmCFStrike:
					{
						appendFlagElement(parent, *iter, L"strike", true);
					}break;
					case sprmOldCFVanish:
					case sprmCFVanish:
					{
						appendFlagElement(parent, *iter, L"vanish", true);
					}break;
					case 0x0811:
					{
						appendFlagElement(parent, *iter, L"webHidden", true);
					}break;
					case sprmOldCIss:
					case sprmCIss:
					{
						if (iter->argumentsSize > 0 && iter->Arguments[0] < 3)	//Metaevan.doc
							appendValueElement(parent, L"vertAlign", FormatUtils::MapValueToWideString(iter->Arguments[0], &SuperscriptIndex[0][0], 3, 12), true);
					}break;
					case sprmCRgLid0_80:
					case sprmCRgLid0:
					{	//latin					
						LanguageId langid(FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize));

						LanguageIdMapping* langIDMapping = new LanguageIdMapping(lang, Default);

						langid.Convert(langIDMapping);

						RELEASEOBJECT(langIDMapping);
					}break;
					case sprmOldCLid:
					case sprmCRgLid1_80:
					case sprmCRgLid1:
					{	//east asia				
						LanguageId langid(FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize));

						LanguageIdMapping* langIDMapping = new LanguageIdMapping(lang, EastAsian);

						langid.Convert(langIDMapping);

						RELEASEOBJECT(langIDMapping);
					}break;
					case sprmCLidBi:
					{
						LanguageId langid(FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize));

						LanguageIdMapping* langIDMapping = new LanguageIdMapping(lang, Complex);

						langid.Convert(langIDMapping);

						RELEASEOBJECT(langIDMapping);
					}break;
					case sprmCBrc80:
					case sprmCBrc:
					{  //borders
						XMLTools::XMLElement bdr(L"w:bdr");
						BorderCode bc(iter->Arguments, iter->argumentsSize);
						appendBorderAttributes(&bc, &bdr);
						parent->AppendChild(bdr);
					}break;
					case sprmCShd80:
					case sprmCShd:
					{  //shading
						ShadingDescriptor desc(iter->Arguments, iter->argumentsSize);

						appendShading(parent, desc);
					}break;
					case sprmOldCIco:
					case sprmCIco:
					case sprmCIcoBi:
					{//color
						colorVal->SetValue(FormatUtils::MapValueToWideString(iter->Arguments[0], &Global::ColorIdentifier[0][0], 17, 12));
					}break;
					case sprmCCv:
					{
						std::wstringstream sstream;

						sstream << boost::wformat(L"%02x%02x%02x") % iter->Arguments[0] % /*G*/iter->Arguments[1] % /*B*/iter->Arguments[2];
						colorVal->SetValue(sstream.str());
					}break;
					case sprmCOldHighlight:
					{
						appendValueElement(parent, L"highlight", FormatUtils::MapValueToWideString(iter->Arguments[1], &Global::ColorNameIdentifier[0][0], 17, 12), true);
					}break;
					case sprmCHighlight:
					{
						appendValueElement(parent, L"highlight", FormatUtils::MapValueToWideString(iter->Arguments[0], &Global::ColorNameIdentifier[0][0], 17, 12), true);
					}break;
					case sprmOldCDxaSpace:
					case sprmCDxaSpace:
					{
						appendValueElement(parent, L"spacing", FormatUtils::IntToWideString(FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize)), true);
					}break;
					case sprmCFtcBi:
					{//default from FontTable
						size_t nIndex = FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize);
						if (nIndex < _doc->FontTable->Data.size())
						{
							FontFamilyName* ffn = static_cast<FontFamilyName*>(_doc->FontTable->operator [] (nIndex));
							if (ffn)
							{
								m_sCsFont = ffn->xszFtn;

								XMLTools::XMLAttribute* cs = new XMLTools::XMLAttribute(L"w:cs");
								cs->SetValue(FormatUtils::XmlEncode(m_sCsFont, true));
								rFonts->AppendAttribute(*cs);
								RELEASEOBJECT(cs);
							}
						}
					}break;
					case sprmCHpsBi:
					{
						appendValueElement(parent, L"szCs",
							FormatUtils::IntToWideString(FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize)), true);
					}
					break;
					// Font Size in points (2~3276) default 20-half-points
					case sprmOldCHps:
					{
						appendValueElement(parent, L"sz",
							FormatUtils::IntToWideString(FormatUtils::BytesToUChar(iter->Arguments, 0, iter->argumentsSize)),
							true);
					}break;
					case sprmCHps:
					{
						appendValueElement(parent, L"sz",
							FormatUtils::IntToWideString(FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize)), true);
					}break;
					case sprmCMajority:
					{	//for complex props
					}break;
					case sprmOldCHpsPos:
					{	// The vertical position, in half-points, of text relative to the normal position. (MUST be between -3168 and 3168)
						short nVertPos = FormatUtils::BytesToUChar(iter->Arguments, 0, iter->argumentsSize);
						appendValueElement(parent, L"position", nVertPos, true);
					}break;
					case sprmCHpsPos:
					{	// The vertical position, in half-points, of text relative to the normal position. (MUST be between -3168 and 3168)
						short nVertPos = FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize);
						appendValueElement(parent, L"position", nVertPos, true);
					}break;
					case sprmOldCHpsKern:
					case sprmCHpsKern:
					{
						appendValueElement(parent, L"kern", FormatUtils::IntToWideString(FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize)), true);
					}break;
					case sprmOldCFtc:
					case sprmCRgFtc0:
					{	// font family
						size_t nIndex = FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize);

						if (nIndex < _doc->FontTable->Data.size())
						{
							XMLTools::XMLAttribute* ascii = new XMLTools::XMLAttribute(L"w:ascii");
							FontFamilyName* ffn = static_cast<FontFamilyName*>(_doc->FontTable->operator [] (nIndex));
							m_sAsciiFont = ffn->xszFtn;
							ascii->SetValue(FormatUtils::XmlEncode(m_sAsciiFont, true));
							rFonts->AppendAttribute(*ascii);
							RELEASEOBJECT(ascii);
						}
					}break;
					case sprmCRgFtc1:
					{
						size_t nIndex = FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize);

						if (nIndex >= 0 && nIndex < _doc->FontTable->Data.size())
						{
							XMLTools::XMLAttribute* eastAsia = new XMLTools::XMLAttribute(L"w:eastAsia");
							FontFamilyName* ffn = static_cast<FontFamilyName*>(_doc->FontTable->operator [] (nIndex));
							m_sEastAsiaFont = ffn->xszFtn;
							eastAsia->SetValue(FormatUtils::XmlEncode(m_sEastAsiaFont));
							rFonts->AppendAttribute(*eastAsia);
							RELEASEOBJECT(eastAsia);
						}
					}
					break;
					case sprmCRgFtc2:
					{
						size_t nIndex = FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize);

						if (nIndex >= 0 && nIndex < _doc->FontTable->Data.size())
						{
							XMLTools::XMLAttribute* ansi = new XMLTools::XMLAttribute(L"w:hAnsi");
							FontFamilyName* ffn = static_cast<FontFamilyName*>(_doc->FontTable->operator [] (nIndex));
							m_shAnsiFont = ffn->xszFtn;
							ansi->SetValue(FormatUtils::XmlEncode(m_shAnsiFont));
							rFonts->AppendAttribute(*ansi);
							RELEASEOBJECT(ansi);
						}
					}break;
					case sprmOldCKul:
					case sprmCKul:
					{	//Underlining
						appendValueElement(parent, L"u", FormatUtils::MapValueToWideString(iter->Arguments[0], &Global::UnderlineCode[0][0], 56, 16), true);
					}
					break;
					case sprmCCharScale:
					{	//char width
						appendValueElement(parent, L"w", FormatUtils::IntToWideString(FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize)), true);
					}break;
					case sprmCSfxText:
					{	//animation
						appendValueElement(parent, L"effect", FormatUtils::MapValueToWideString(iter->Arguments[0], &Global::TextAnimation[0][0], 7, 16), true);
					}break;
					case sprmCIdctHint:
					{
						XMLTools::XMLAttribute hint(L"w:hint");
						switch (iter->Arguments[0])
						{
						case 1:
						{
							hint.SetValue(L"eastAsia");
							rFonts->AppendAttribute(hint);
						}break;
						case 2:
						{
							hint.SetValue(L"cs");
							rFonts->AppendAttribute(hint);
						}break;
						case 0: break;	// default
						case 0xFF:	break;	//No ST_Hint equivalent
						}
					}break;
					case sprmCPbiIBullet:
					{
						int nIndex = FormatUtils::BytesToInt32(iter->Arguments, 0, iter->argumentsSize);
						if (nIndex >= 0)
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
						int val = FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize);

					}break;
					case sprmCRsidProp:
					case sprmCRsidText:
						break;
					case sprmCFUsePgsuSettings:
					{
						appendFlagElement(parent, *iter, L"snapToGrid", true);
					}break;
					default:
					{
						switch (iter->argumentsSize)
						{
						case 1:
							nProperty = FormatUtils::BytesToUChar(iter->Arguments, 0, iter->argumentsSize);
							break;
						case 2:
							nProperty = FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize);
							break;
						case 4:
							nProperty = FormatUtils::BytesToInt32(iter->Arguments, 0, iter->argumentsSize);
							break;
						}
					}break;
				}
			}
		}
		if ( lang->GetAttributeCount() > 0 )
		{
			parent->AppendChild( *lang );
		}
		if ( rFonts->GetAttributeCount() > 0 )
		{
			parent->AppendChild( *rFonts );
		}
        if ( colorVal->GetValue() != L"")
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
    void CharacterPropertiesMapping::appendFlagElement( XMLTools::XMLElement* node, const SinglePropertyModifier& sprm, const wchar_t* elementName, bool unique )
	{
		unsigned char flag = sprm.Arguments[0];

		if( flag != 128 )
		{
            XMLTools::XMLElement* ele = new XMLTools::XMLElement( L"w", elementName );
            XMLTools::XMLAttribute* val = new XMLTools::XMLAttribute( L"w:val" );

			if ( unique )
			{
				node->RemoveChild( *ele );
			}

			if ( flag == 0 )
			{
                val->SetValue( L"0" );
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
                    val->SetValue( L"0" );
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
						StyleSheetDescription* style = styleSheet->Styles->at( istd );
						
						if (!style) 
							break;
						CharacterPropertyExceptions* baseChpx = style->chpx;

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
