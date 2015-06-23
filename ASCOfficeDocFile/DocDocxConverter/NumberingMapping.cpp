
#include "NumberingMapping.h"

namespace DocFileFormat
{
	NumberingMapping::NumberingMapping(ConversionContext* context) : AbstractOpenXmlMapping(new XmlUtils::CXmlWriter()), m_context(context), m_document(NULL), m_xmldocument(NULL)
	{
		if (m_context)
		{
			m_document		=	m_context->GetDocument();
			m_xmldocument	=	m_context->GetXmlDocument();
		}
	}

	NumberingMapping::~NumberingMapping()
	{
		RELEASEOBJECT(m_pXmlWriter);
	}
}

namespace DocFileFormat
{
	void NumberingMapping::Apply(IVisitable* visited)
	{
		if ((NULL == m_document) || (NULL == m_xmldocument))
			return;

		ListTable* rglst = static_cast<ListTable*>(visited);

		if ((rglst != NULL) && (!rglst->listData.empty()))
		{
			m_xmldocument->RegisterNumbering();

			//start the document
			m_pXmlWriter->WriteNodeBegin( _T( "?xml version=\"1.0\" encoding=\"UTF-8\"?" ) ); 
			m_pXmlWriter->WriteNodeBegin( _T( "w:numbering" ), TRUE );

			//write namespaces
			m_pXmlWriter->WriteAttribute( _T( "xmlns:w" ), OpenXmlNamespaces::WordprocessingML );
			m_pXmlWriter->WriteAttribute( _T( "xmlns:v" ), OpenXmlNamespaces::VectorML );
			m_pXmlWriter->WriteAttribute( _T( "xmlns:o" ), OpenXmlNamespaces::Office );
			m_pXmlWriter->WriteAttribute( _T( "xmlns:w10" ), OpenXmlNamespaces::OfficeWord );
			m_pXmlWriter->WriteAttribute( _T( "xmlns:r" ), OpenXmlNamespaces::Relationships );

			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

			PictureBulletsMapping();

			int i = 0;

			for (list<ListData*>::iterator iter = rglst->listData.begin(); iter != rglst->listData.end(); ++iter, ++i)
			{
				//start abstractNum
				m_pXmlWriter->WriteNodeBegin( _T( "w:abstractNum" ), TRUE );
				m_pXmlWriter->WriteAttribute( _T( "w:abstractNumId" ), FormatUtils::IntToWideString( i ).c_str() );
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

				//nsid
				m_pXmlWriter->WriteNodeBegin( _T( "w:nsid" ), TRUE );
				m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::IntToFormattedWideString( (*iter)->lsid, _T( "%08x" ) ).c_str() );
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

				//multiLevelType
				m_pXmlWriter->WriteNodeBegin( _T( "w:multiLevelType" ), TRUE );

				if ( (*iter)->fHybrid )
				{
					m_pXmlWriter->WriteAttribute( _T( "w:val" ), _T( "hybridMultilevel" ) );
				}
				else if ( (*iter)->fSimpleList )
				{
					m_pXmlWriter->WriteAttribute( _T( "w:val" ), _T( "singleLevel" ) );
				}
				else
				{
					m_pXmlWriter->WriteAttribute( _T( "w:val" ), _T( "multilevel" ) );
				}

				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

				//template
				m_pXmlWriter->WriteNodeBegin( _T( "w:tmpl" ), TRUE );
				m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::IntToFormattedWideString( (*iter)->tplc, _T( "%08x" ) ).c_str());
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

				// writes the levels
				size_t length = (*iter)->rglvl->size();

				for (size_t j = 0; j < length; ++j)
				{
					ListLevel* lvl = (*iter)->rglvl->at(j);
					LevelMapping(lvl, j, (*iter)->rgistd[j]);
				}

				//end abstractNum
				m_pXmlWriter->WriteNodeEnd( _T( "w:abstractNum" ) );
			}

			//write the overrides
			for (unsigned int i = 0; i < m_document->listFormatOverrideTable->size(); ++i)
			{
				ListFormatOverride* lfo = m_document->listFormatOverrideTable->at(i);

				//start num
				m_pXmlWriter->WriteNodeBegin( _T( "w:num" ), TRUE );
				m_pXmlWriter->WriteAttribute( _T( "w:numId" ), FormatUtils::IntToWideString(i + 1).c_str());
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

				int index = FindIndexbyId( rglst->listData, lfo->lsid );

				m_pXmlWriter->WriteNodeBegin( _T( "w:abstractNumId" ), TRUE );
				m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::IntToWideString( index ).c_str() );
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

				for (vector<ListFormatOverrideLevel*>::const_iterator iter = lfo->rgLfoLvl.begin(); iter != lfo->rgLfoLvl.end(); ++iter)
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:lvlOverride" ), TRUE );

					m_pXmlWriter->WriteAttribute( _T( "w:ilvl" ), FormatUtils::IntToWideString( (*iter)->ilvl ).c_str() );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

					if ( ( (*iter)->fStartAt ) && ( !(*iter)->fFormatting ) )
					{
						m_pXmlWriter->WriteNodeBegin( _T( "w:startOverride" ), TRUE );
						m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::IntToWideString( (*iter)->iStartAt ).c_str() );
						m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
					}

					LevelMapping((*iter)->lvl, 0, ListData::ISTD_NIL);

					m_pXmlWriter->WriteNodeEnd(_T("w:lvlOverride"));
				}

				m_pXmlWriter->WriteNodeEnd(_T( "w:num"));
			}

			m_pXmlWriter->WriteNodeEnd(_T("w:numbering"));

			m_xmldocument->NumberingXML = std::wstring(m_pXmlWriter->GetXmlString());
		}
	}

	int NumberingMapping::FindIndexbyId(const list<ListData*>& listData, int id)
	{
		int ret = -1;
		int i = 0;

		for (list<ListData*>::const_iterator iter = listData.begin(); iter != listData.end(); ++iter, ++i)
		{
			if ((*iter)->lsid == id)
			{
				ret = i;
				break;
			}
		}

		return ret;
	}

	// Converts the number text of the binary format to the number text of OOXML.
	// OOXML uses different placeholders for the numbers.

	std::wstring NumberingMapping::GetLvlText(const ListLevel* lvl, bool bIsSymbol) const
	{
		std::wstring ret;

		if (lvl != NULL)
		{
			if (lvl->nfc == 0x17)
			{
				if (!lvl->xst.empty())
				{
					wchar_t xchBullet = lvl->xst[0];

					// В символьном шрифте обрезать надо, в других случаях - нет
					if (true == bIsSymbol && (xchBullet & 0xF000) != 0)
					{
						xchBullet &= 0x0FFF;
					}

					if (!FormatUtils::IsControlSymbol(xchBullet))
					{
						ret.push_back(lvl->xst[0]);
						ret.push_back(L'\0');
					}
				}
			}
			else
			{
				ret = lvl->xst;

				std::wstring::const_iterator result		=	lvl->xst.begin();
				std::wstring::const_iterator newResult	=	lvl->xst.begin();

				newResult								=	find_if(lvl->xst.begin(), lvl->xst.end(), &NumberingMapping::IsPlaceholder);
				ret										=	wstring(lvl->xst.begin(), newResult);
				result									=	newResult;

				while (result != lvl->xst.end())
				{
					newResult = find_if((result + 1), lvl->xst.end(), &NumberingMapping::IsPlaceholder);

					ret += _T( "%" );
					ret += FormatUtils::IntToWideString(*result + 1);
					ret += wstring((result + 1), newResult);
					result = newResult;
				}
			}
		}

		return ret;
	}

	bool NumberingMapping::IsPlaceholder(wchar_t symbol)
	{
		if ((symbol == (wchar_t)0x0000) || (symbol == (wchar_t)0x0001) || (symbol == (wchar_t)0x0002) || 
			(symbol == (wchar_t)0x0003) || (symbol == (wchar_t)0x0004) || (symbol == (wchar_t)0x0005) || 
			(symbol == (wchar_t)0x0006) || (symbol == (wchar_t)0x0007) || (symbol == (wchar_t)0x0008))
		{
			return true;  
		}

		return false;
	}

	// Converts the number format code of the binary format.
	std::wstring NumberingMapping::GetNumberFormatWideString(int nfc)
	{
		switch ( nfc )
		{
		case 0:
			return std::wstring( _T( "decimal" ) );
		case 1:
			return std::wstring( _T( "upperRoman" ) );
		case 2:
			return std::wstring( _T( "lowerRoman" ) );
		case 3:
			return std::wstring( _T( "upperLetter" ) );
		case 4:
			return std::wstring( _T( "lowerLetter" ) );
		case 5:
			return std::wstring( _T( "ordinal" ) );
		case 6:
			return std::wstring( _T( "cardinalText" ) );
		case 7:
			return std::wstring( _T( "ordinalText" ) );
		case 8:
			return std::wstring( _T( "hex" ) );
		case 9:
			return std::wstring( _T( "chicago" ) );
		case 10:
			return std::wstring( _T( "ideographDigital" ) );
		case 11:
			return std::wstring( _T( "japaneseCounting" ) );
		case 12:
			return std::wstring( _T( "aiueo" ) );
		case 13:
			return std::wstring( _T( "iroha" ) );
		case 14:
			return std::wstring( _T( "decimalFullWidth" ) );
		case 15:
			return std::wstring( _T( "decimalHalfWidth" ) );
		case 16:
			return std::wstring( _T( "japaneseLegal" ) );
		case 17:
			return std::wstring( _T( "japaneseDigitalTenThousand" ) );
		case 18:
			return std::wstring( _T( "decimalEnclosedCircle" ) );
		case 19:
			return std::wstring( _T( "decimalFullWidth2" ) );
		case 20:
			return std::wstring( _T( "aiueoFullWidth" ) );
		case 21:
			return std::wstring( _T( "irohaFullWidth" ) );
		case 22:
			return std::wstring( _T( "decimalZero" ) );
		case 23:
			return std::wstring( _T( "bullet" ) );
		case 24:
			return std::wstring( _T( "ganada" ) );
		case 25:
			return std::wstring( _T( "chosung" ) );
		case 26:
			return std::wstring( _T( "decimalEnclosedFullstop" ) );
		case 27:
			return std::wstring( _T( "decimalEnclosedParen" ) );
		case 28:
			return std::wstring( _T( "decimalEnclosedCircleChinese" ) );
		case 29:
			return std::wstring( _T( "ideographEnclosedCircle" ) );
		case 30:
			return std::wstring( _T( "ideographTraditional" ) );
		case 31:
			return std::wstring( _T( "ideographZodiac" ) );
		case 32:
			return std::wstring( _T( "ideographZodiacTraditional" ) );
		case 33:
			return std::wstring( _T( "taiwaneseCounting" ) );
		case 34:
			return std::wstring( _T( "ideographLegalTraditional" ) );
		case 35:
			return std::wstring( _T( "taiwaneseCountingThousand" ) );
		case 36:
			return std::wstring( _T( "taiwaneseDigital" ) );
		case 37:
			return std::wstring( _T( "chineseCounting" ) );
		case 38:
			return std::wstring( _T( "chineseLegalSimplified" ) );
		case 39:
			return std::wstring( _T( "chineseCountingThousand" ) );
		case 40:
			return std::wstring( _T( "koreanDigital" ) );
		case 41:
			return std::wstring( _T( "koreanCounting" ) );
		case 42:
			return std::wstring( _T( "koreanLegal" ) );
		case 43:
			return std::wstring( _T( "koreanDigital2" ) );
		case 44:
			return std::wstring( _T( "vietnameseCounting" ) );
		case 45:
			return std::wstring( _T( "russianLower" ) );
		case 46:
			return std::wstring( _T( "russianUpper" ) );
		case 47:
			return std::wstring( _T( "none" ) );
		case 48:
			return std::wstring( _T( "numberInDash" ) );
		case 49:
			return std::wstring( _T( "hebrew1" ) );
		case 50:
			return std::wstring( _T( "hebrew2" ) );
		case 51:
			return std::wstring( _T( "arabicAlpha" ) );
		case 52:
			return std::wstring( _T( "arabicAbjad" ) );
		case 53:
			return std::wstring( _T( "hindiVowels" ) );
		case 54:
			return std::wstring( _T( "hindiConsonants" ) );
		case 55:
			return std::wstring( _T( "hindiNumbers" ) );
		case 56:
			return std::wstring( _T( "hindiCounting" ) );
		case 57:
			return std::wstring( _T( "thaiLetters" ) );
		case 58:
			return std::wstring( _T( "thaiNumbers" ) );
		case 59:
			return std::wstring( _T( "thaiCounting" ) );
		default:
			return std::wstring( _T( "decimal" ) );
		}
	}

	void NumberingMapping::LevelMapping(const ListLevel* lvl, unsigned int level, short styleIndex)
	{
		if (lvl)
		{
			XmlUtils::CXmlWriter oWriterTemp;	//Временный writer,что не нарушать последовательность записи

			//rPr

			RevisionData rev(lvl->grpprlChpx);
			CharacterPropertiesMapping cpMapping(&oWriterTemp, m_document, &rev, lvl->grpprlPapx, false);
			lvl->grpprlChpx->Convert(&cpMapping);

			// Проверяем шрифт

			m_pXmlWriter->WriteNodeBegin( _T( "w:lvl" ), TRUE );
			m_pXmlWriter->WriteAttribute( _T( "w:ilvl" ), FormatUtils::IntToWideString(level).c_str());
			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

			// starts at

			m_pXmlWriter->WriteNodeBegin( _T( "w:start" ), TRUE );
			m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::IntToWideString(lvl->iStartAt).c_str());
			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

			// number format

			m_pXmlWriter->WriteNodeBegin( _T( "w:numFmt" ), TRUE );
			m_pXmlWriter->WriteAttribute( _T( "w:val" ), GetNumberFormatWideString(lvl->nfc).c_str());
			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

			// suffix

			m_pXmlWriter->WriteNodeBegin( _T( "w:suff" ), TRUE );
			m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::MapValueToWideString(lvl->ixchFollow, &FollowingCharMap[0][0], 3, 8).c_str());
			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

			// style
			// The style id is used for a reverse reference. 
			// It can happen that the reference points to the wrong style.

			if (styleIndex != ListData::ISTD_NIL)
			{
				m_pXmlWriter->WriteNodeBegin( _T( "w:pStyle" ), TRUE );
				m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::XmlEncode(StyleSheetMapping::MakeStyleId(m_document->Styles->Styles->at(styleIndex))).c_str());
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
			}

			// Number level text

			m_pXmlWriter->WriteNodeBegin(_T("w:lvlText"), TRUE);
			m_pXmlWriter->WriteAttribute(_T("w:val"), GetLvlText(lvl, cpMapping.CheckIsSymbolFont()).c_str());
			m_pXmlWriter->WriteNodeEnd(_T( ""), TRUE);

			WriteLevelPictureBullet(lvl->grpprlChpx);

			// legacy

			if (lvl->fWord6)
			{
				m_pXmlWriter->WriteNodeBegin( _T( "w:legacy" ), TRUE );
				m_pXmlWriter->WriteAttribute( _T( "w:legacy" ), _T( "1" ) );
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
			}

			// jc

			m_pXmlWriter->WriteNodeBegin( _T( "w:lvlJc" ), TRUE );
			m_pXmlWriter->WriteAttribute( _T( "w:val" ), FormatUtils::MapValueToWideString(lvl->jc, &LevelJustificationMap[0][0], 3, 7).c_str());
			m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

			// pPr
			ParagraphPropertiesMapping oppMapping(m_pXmlWriter, m_context, m_document, NULL);
			lvl->grpprlPapx->Convert(&oppMapping);

			// пишем rPr

			m_pXmlWriter->WriteString(oWriterTemp.GetXmlString());

			m_pXmlWriter->WriteNodeEnd(_T("w:lvl"));
		}
	}

	void NumberingMapping::PictureBulletsMapping()
	{
		for (map<int, int>::const_iterator iter = m_document->PictureBulletsCPsMap.begin(); iter != m_document->PictureBulletsCPsMap.end(); ++iter)
		{
			int fc		=	m_document->FindFileCharPos(iter->second);
			int fcEnd	=	m_document->FindFileCharPos(iter->second + 1);

			list<CharacterPropertyExceptions*>* chpxs = m_document->GetCharacterPropertyExceptions(fc, fcEnd);

			if ((chpxs != NULL) && (!chpxs->empty()))
			{
				PictureDescriptor pict(chpxs->front(), m_document->DataStream);

				if ((pict.mfp.mm > 98) && (pict.shapeContainer != NULL))
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:numPicBullet" ), TRUE );
					m_pXmlWriter->WriteAttribute( _T( "w:numPicBulletId" ), FormatUtils::IntToWideString( iter->first ).c_str() );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );
					m_pXmlWriter->WriteNodeBegin( _T( "w:pict" ) );

					if (pict.blipStoreEntry != NULL)
					{
						VMLPictureMapping oPicture(m_context, m_pXmlWriter, false, this, true);
						pict.Convert(&oPicture);
					}
					else
					{
						VMLShapeMapping oShape (m_context, m_pXmlWriter, NULL, &pict, this,true);
						pict.shapeContainer->Convert(&oShape);
					}

					m_pXmlWriter->WriteNodeEnd(_T("w:pict"));
					m_pXmlWriter->WriteNodeEnd(_T("w:numPicBullet"));
				}  
			}

			RELEASEOBJECT( chpxs );
		}
	}

	void NumberingMapping::WriteLevelPictureBullet(const CharacterPropertyExceptions* grpprlChpx)
	{
		if (grpprlChpx)
		{
			unsigned int cp = 0;
			bool isPictureBullet = false;

			for (list<SinglePropertyModifier>::const_iterator iter = grpprlChpx->grpprl->begin(); iter != grpprlChpx->grpprl->end(); ++iter)
			{
				if ((int)(iter->OpCode) == sprmCPbiIBullet)
				{
					cp = FormatUtils::BytesToUInt32(iter->Arguments, 0, iter->argumentsSize);
				}

				if ((int)(iter->OpCode) == sprmCPbiGrf)
				{
					isPictureBullet = FormatUtils::BitmaskToBool(FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize), 0x1);  
				}
			}

			if (isPictureBullet)
			{
				m_pXmlWriter->WriteNodeBegin(_T("w:lvlPicBulletId"), TRUE);
				m_pXmlWriter->WriteAttribute(_T("w:val"), FormatUtils::IntToWideString(cp).c_str());
				m_pXmlWriter->WriteNodeEnd(_T(""), TRUE);  
			}
		}
	}
}