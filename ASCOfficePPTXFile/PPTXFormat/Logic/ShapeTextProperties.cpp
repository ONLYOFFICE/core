//#include "./stdafx.h"

#include "ShapeTextProperties.h"
#include "Fills/SolidFill.h"
#include "Fills/GradFill.h"
#include "../Slide.h"
#include "../SlideLayout.h"
#include "../SlideMaster.h"

namespace PPTX
{
	namespace Logic
	{
        AVSINLINE WORD GetTextAnchorFromStr(const CString& str)
		{
			if (str == _T("t"))		return 0;
			if (str == _T("ctr"))	return 1;
			if (str == _T("b"))		return 2;
			return 0;
		}

        AVSINLINE WORD GetTextAlignFromStr(const CString& str)
		{
			if (str == _T("l"))			return 0;
			if (str == _T("ctr"))		return 1;
			if (str == _T("r"))			return 2;
			if (str == _T("just"))		return 3;
			if (str == _T("dist"))		return 4;
			if (str == _T("thaiDist"))	return 5;
			if (str == _T("justLow"))	return 6;
			return 0;
		}

        AVSINLINE WORD GetFontAlignFromStr(const CString& str)
		{
			if (str == _T("auto"))	return 0;
			if (str == _T("base"))	return 0;
			if (str == _T("t"))		return 1;
			if (str == _T("ctr"))	return 2;
			if (str == _T("b"))		return 3;
			return 0;
		}

        AVSINLINE int GetIntegerFromHex(const CString& string)
		{
			return XmlUtils::GetInteger(string);
		}

		CShapeTextProperties::CShapeTextProperties()
		{
			m_lTextType				= -1;
			m_bIsFontRefFromSlide	= false;
			m_lMasterTextSize		= -1;

			m_bIsSlideShape			= false;
			m_masters				= NULL;

			m_pFile					= NULL;
		}

		CShapeTextProperties::~CShapeTextProperties()
		{
		}

		void CShapeTextProperties::FillTextType(const LONG& lTextMasterType)
		{
			m_lTextType = lTextMasterType;
		}
		void CShapeTextProperties::FillFromTextBody(const nullable<TxBody>& Src, const nullable<TextParagraphPr>* bodyStyles)
		{
			if (Src.IsInit())
			{
				Src->bodyPr.Merge(bodyPr);
				if (Src->lstStyle.IsInit())
				{
					for(int i = 0; i < 10; i++)
					{
						if(Src->lstStyle->levels[i].IsInit())
						{
							if (NULL == bodyStyles)
							{
								// не фик мучиться здесь с темами. Это ж не в слайде.
								// А в слайде все будет круто
								Src->lstStyle->levels[i]->Merge(m_body[i]);
							}
							else
							{
								Src->lstStyle->levels[i]->Merge(m_levels[i]);
							}
						}
					}
				}
			}
			if (NULL != bodyStyles)
			{
				m_bIsSlideShape = true;
				for (int i = 0; i < 10; ++i)
				{
					m_body[i] = bodyStyles[i];
				}
			}
		}
		void CShapeTextProperties::FillFontRef(const FontRef& Src, const bool& bIsSlideProperty)
		{
			m_FontStyle = Src;
		}
		void CShapeTextProperties::FillMasterFontSize(int size)
		{
			m_lMasterTextSize = size;
		}

		nullable_base<WORD> CShapeTextProperties::GetParagraphAlgn(int level, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<WORD> prop;

			if (pParagraph.is_init())
				if (pParagraph->algn.is_init())
					prop = GetTextAlignFromStr(pParagraph->algn->get());

			return prop;
		}
		nullable_base<LONG>	CShapeTextProperties::GetParagraphLeftMargin(int level, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<LONG> prop;

			if (pParagraph.is_init())
				if (pParagraph->marL.is_init())
					prop = (LONG)pParagraph->marL.get();

			return prop;
		}
		nullable_base<LONG> CShapeTextProperties::GetParagraphIndent(int level, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<LONG> prop;

			if (pParagraph.is_init())
				if (pParagraph->indent.is_init())
					prop = pParagraph->indent.get();

			return prop;
		}
		nullable_base<LONG> CShapeTextProperties::GetParagraphDefTabSz(int level, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<LONG> prop;

			if (pParagraph.is_init())
				if (pParagraph->defTabSz.is_init())
					prop = (LONG)pParagraph->defTabSz.get();

			return prop;
		}
		nullable_base<WORD> CShapeTextProperties::GetParagraphFontAlgn(int level, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<WORD> prop;

			if (pParagraph.is_init())
				if (pParagraph->fontAlgn.is_init())
					prop = (WORD)GetFontAlignFromStr(pParagraph->fontAlgn->get());

			return prop;
		}
		nullable_base<WORD> CShapeTextProperties::GetParagraphLatinLnBrk(int level, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<WORD> prop;

			if (pParagraph.is_init())
				if (pParagraph->latinLnBrk.is_init())
				{
					bool bWrap = pParagraph->latinLnBrk.get();
					WORD lWrap = 0;
					if (bWrap)
					{
						lWrap &= 0xFD;
						lWrap |= 0x01;
					}
					else
					{
						lWrap &= 0xFE;
						lWrap |= 0x02;
					}
					prop = lWrap;
				}

			return prop;
		}
		nullable_base<WORD> CShapeTextProperties::GetParagraphRtl(int level, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<WORD> prop;

			if (pParagraph.is_init())
				if (pParagraph->rtl.is_init())
				{
					WORD lVal = 0;
					if (pParagraph->rtl.get())
						lVal = 1;

					prop = lVal;
				}

			return prop;
		}
		nullable_base<LONG> CShapeTextProperties::GetParagraphLnSpc(int level, const nullable<TextParagraphPr>& pParagraph, const double& LnSpcReduction)const
		{
			nullable_base<LONG> prop;

			if (pParagraph.is_init())
				if (pParagraph->lnSpc.is_init())
				{
					int space = pParagraph->lnSpc->GetVal();
					if (space < 0)
						prop = (LONG)(space * 127 * (1 - LnSpcReduction));// - 13200;
					else
						prop = (LONG)(space * (1 - LnSpcReduction));
				}

			return prop;
		}
		nullable_base<LONG> CShapeTextProperties::GetParagraphSpcAft(int level, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<LONG> prop;

			if (pParagraph.is_init())
				if (pParagraph->spcAft.is_init())
				{
					int space = pParagraph->spcAft->GetVal();
					if (space < 0)
						prop = (LONG)(space * 127);// - 13200;
					else
						prop = (LONG)space;
				}

			return prop;
		}
		nullable_base<LONG> CShapeTextProperties::GetParagraphSpcBef(int level, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<LONG> prop;

			if (pParagraph.is_init())
				if (pParagraph->spcBef.is_init())
				{
					int space = pParagraph->spcBef->GetVal();
					if (space < 0)
						prop = (LONG)(space * 127);// - 13200;
					else
						prop = (LONG)space;
				}

			return prop;
		}

		nullable_base<BOOL>	CShapeTextProperties::GetHasBullet(int level, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<BOOL> prop;

			if (pParagraph.is_init())
				if (pParagraph->ParagraphBullet.is_init())
				{
					prop = (BOOL)pParagraph->ParagraphBullet.has_bullet();
					return prop;
				}

			if (m_body[level].is_init())
				if (m_body[level]->ParagraphBullet.is_init())
				{
					prop = (BOOL)m_body[level]->ParagraphBullet.has_bullet();
					return prop;
				}

			if (m_levels[level].is_init())
				if (m_levels[level]->ParagraphBullet.is_init())
				{
					prop = (BOOL)m_levels[level]->ParagraphBullet.has_bullet();
					return prop;
				}
				
			return prop;
		}

		nullable_base<BOOL> CShapeTextProperties::GetRunBold(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<BOOL> prop;

			if (pRun.is_init())
				if (pRun->b.is_init())
				{
					prop = (BOOL)pRun->b.get();
					return prop;
				}
			if (pParagraph.is_init())
				if (pParagraph->defRPr.is_init())
					if (pParagraph->defRPr->b.is_init())
					{
						 prop = (BOOL)pParagraph->defRPr->b.get();
					}

			return prop;
		}
		nullable_base<BOOL> CShapeTextProperties::GetRunItalic(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<BOOL> prop;

			if (pRun.is_init())
				if (pRun->i.is_init())
				{
					prop = (BOOL)pRun->i.get();
					return prop;
				}
			if (pParagraph.is_init())
				if (pParagraph->defRPr.is_init())
					if (pParagraph->defRPr->i.is_init())
					{
						 prop = (BOOL)pParagraph->defRPr->i.get();
					}

			return prop;
		}
		nullable_base<BOOL> CShapeTextProperties::GetRunUnderline(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<BOOL> prop;

			if (pRun.is_init())
				if (pRun->u.is_init())
				{
					prop = (_T("none") != pRun->u->get()) ? TRUE : FALSE;
					return prop;
				}
			if (pParagraph.is_init())
				if (pParagraph->defRPr.is_init())
					if (pParagraph->defRPr->u.is_init())
					{
						prop = (_T("none") != pParagraph->defRPr->u->get()) ? TRUE : FALSE;
						return prop;
					}

			return prop;
		}
		nullable_base<BOOL> CShapeTextProperties::GetRunStrike(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<BOOL> prop;

			if (pRun.is_init())
				if (pRun->strike.is_init())
				{
					prop = (_T("noStrike") != pRun->strike->get()) ? TRUE : FALSE;
					return prop;
				}
			if (pParagraph.is_init())
				if (pParagraph->defRPr.is_init())
					if (pParagraph->defRPr->strike.is_init())
						prop = (_T("noStrike") != pParagraph->defRPr->strike->get()) ? TRUE : FALSE;

			return prop;
		}
		nullable_base<WORD> CShapeTextProperties::GetRunCap(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<WORD> prop;

			if (pRun.is_init())
				if (pRun->cap.is_init())
				{
					CString str = pRun->cap->get();
					if (_T("all") == str)
						prop = (WORD)1;
					else if (_T("small") == str)
						prop = (WORD)2;
					else
						prop = (WORD)0;

					return prop;
				}
			if(pParagraph.is_init())
				if(pParagraph->defRPr.is_init())
					if(pParagraph->defRPr->cap.is_init())
					{
						CString str = pParagraph->defRPr->cap->get();
						if (_T("all") == str)
							prop = (WORD)1;
						else if (_T("small") == str)
							prop = (WORD)2;
						else
							prop = (WORD)0;
					}

			return prop;
		}
		nullable_base<double> CShapeTextProperties::GetRunBaseline(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<double> prop;

			if (pRun.is_init())
				if (pRun->baseline.is_init())
				{
					prop = (double)pRun->baseline.get() / 1000;
					return prop;
				}
			if (pParagraph.is_init())
				if (pParagraph->defRPr.is_init())
					if (pParagraph->defRPr->baseline.is_init())
						prop = (double)pParagraph->defRPr->baseline.get() / 1000;

			return prop;
		}
		nullable_base<WORD> CShapeTextProperties::GetRunSize(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph, const double& FontScale)const
		{
			nullable_base<WORD> prop;

			if (pRun.is_init())
				if (pRun->sz.is_init())
				{
					int size = pRun->sz.get();
					prop = (WORD)((size * FontScale + 50) / 100);
					return prop;
				}
			if (pParagraph.is_init())
				if (pParagraph->defRPr.is_init())
					if (pParagraph->defRPr->sz.is_init())
					{
						int size = pParagraph->defRPr->sz.get();
						prop = (WORD)((size * FontScale + 50) / 100);
						return prop;
					}

			if (-1 != m_lMasterTextSize)
			{
				bool bIsSetUp = false;

				if (m_levels[level].is_init())
					if (m_levels[level]->defRPr.is_init())
						if (m_levels[level]->defRPr->sz.is_init())
							bIsSetUp = true;

				if (!bIsSetUp)
				{
					if (m_body[level].is_init())
						if (m_body[level]->defRPr.is_init())
							if (m_body[level]->defRPr->sz.is_init())
								bIsSetUp = true;
				}

				if (!bIsSetUp)
					prop = (WORD)((double)(m_lMasterTextSize * FontScale + 50) / 100);
			}

			return prop;
		}

		nullable_base<CFontInfo> CShapeTextProperties::GetRunFont(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const
		{
			nullable_base<CFontInfo> prop;

			if ((pRun.is_init()) && (pRun->latin.is_init()))
			{
				CFontInfo oInfo;
				
				oInfo.strFontName		= pRun->latin->typeface;
				oInfo.strPanose			= pRun->latin->panose.get_value_or(_T(""));
				oInfo.strPitchFamily	= pRun->latin->pitchFamily.get_value_or(_T(""));
				oInfo.Charset			= XmlUtils::GetInteger(pRun->latin->charset.get_value_or(_T("0")));

				oInfo.FontRef			= -1;
				if (oInfo.strFontName == _T("+mj-lt"))
				{
					oInfo.strFontName = MajorLatin.typeface;
					oInfo.FontRef = 0;
				}
				else if (oInfo.strFontName == _T("+mn-lt"))
				{
					oInfo.strFontName = MinorLatin.typeface;
					oInfo.FontRef = 1;
				}
				else if (oInfo.strFontName == _T(""))
				{
					oInfo.strFontName = MinorLatin.typeface;
					oInfo.FontRef = 1;
				}

				prop = oInfo;
			}
			else if ((pParagraph.is_init()) && (pParagraph->defRPr.is_init()) && (pParagraph->defRPr->latin.is_init()))
			{
				CFontInfo oInfo;
				
				oInfo.strFontName		= pParagraph->defRPr->latin->typeface;
				oInfo.strPanose			= pParagraph->defRPr->latin->panose.get_value_or(_T(""));
				oInfo.strPitchFamily	= pParagraph->defRPr->latin->pitchFamily.get_value_or(_T(""));
				oInfo.Charset			= XmlUtils::GetInteger(pParagraph->defRPr->latin->charset.get_value_or(_T("0")));

				oInfo.FontRef			= -1;
				if (oInfo.strFontName == _T("+mj-lt"))
				{
					oInfo.strFontName = MajorLatin.typeface;
					oInfo.FontRef = 0;
				}
				else if (oInfo.strFontName == _T("+mn-lt"))
				{
					oInfo.strFontName = MinorLatin.typeface;
					oInfo.FontRef = 1;
				}
				else if (oInfo.strFontName == _T(""))
				{
					oInfo.strFontName = MinorLatin.typeface;
					oInfo.FontRef = 1;
				}
				prop = oInfo;
			}

			return prop;
		}
		PPTX::Logic::UniColor	CShapeTextProperties::GetColor(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const
		{
			PPTX::Logic::UniColor prop;

			if (pRun.is_init())
			{
				if (pRun->Fill.is<SolidFill>())
				{
					prop = pRun->Fill.as<SolidFill>().Color;
					return prop;
				}
				if (pRun->Fill.is<GradFill>())
				{
					prop = pRun->Fill.as<GradFill>().GetFrontColor();
					return prop;
				}
			}

			if ((pParagraph.is_init()) && (pParagraph->defRPr.is_init()))
			{
				if (pParagraph->defRPr->Fill.is<SolidFill>())
				{
					prop = pParagraph->defRPr->Fill.as<SolidFill>().Color;
					return prop;
				}
				if (pParagraph->defRPr->Fill.is<GradFill>())
				{
					prop = pParagraph->defRPr->Fill.as<GradFill>().GetFrontColor();
					return prop;
				}
			}

			return prop;
		}
		PPTX::Logic::UniColor	CShapeTextProperties::GetColorBullet(int level, const nullable<TextParagraphPr>& pParagraph)const
		{
			PPTX::Logic::UniColor prop;

			if (pParagraph.is_init())
			{
				if (pParagraph->buColor.is_init())
				{
					if (pParagraph->buColor.is<Logic::BuClr>())
						prop = pParagraph->buColor.as<Logic::BuClr>().Color;
				}
			}

			return prop;
		}

		DWORD CShapeTextProperties::GetHyperlinkRGBA()const
		{
			if(bodyPr.parentFileIs<Slide>())
				return bodyPr.parentFileAs<Slide>().GetRGBAFromMap(_T("hlink"));
			else if(bodyPr.parentFileIs<SlideLayout>())
				return bodyPr.parentFileAs<SlideLayout>().GetRGBAFromMap(_T("hlink"));
			else if(bodyPr.parentFileIs<SlideMaster>())
				return bodyPr.parentFileAs<SlideMaster>().GetRGBAFromMap(_T("hlink"));
			else return 0;
		}
		DWORD CShapeTextProperties::GetHyperlinkARGB()const
		{
			if(bodyPr.parentFileIs<Slide>())
				return bodyPr.parentFileAs<Slide>().GetARGBFromMap(_T("hlink"));
			else if(bodyPr.parentFileIs<SlideLayout>())
				return bodyPr.parentFileAs<SlideLayout>().GetARGBFromMap(_T("hlink"));
			else if(bodyPr.parentFileIs<SlideMaster>())
				return bodyPr.parentFileAs<SlideMaster>().GetARGBFromMap(_T("hlink"));
			else return 0;
		}
		DWORD CShapeTextProperties::GetHyperlinkBGRA()const
		{
			if(bodyPr.parentFileIs<Slide>())
				return bodyPr.parentFileAs<Slide>().GetBGRAFromMap(_T("hlink"));
			else if(bodyPr.parentFileIs<SlideLayout>())
				return bodyPr.parentFileAs<SlideLayout>().GetBGRAFromMap(_T("hlink"));
			else if(bodyPr.parentFileIs<SlideMaster>())
				return bodyPr.parentFileAs<SlideMaster>().GetBGRAFromMap(_T("hlink"));
			else return 0;
		}
		DWORD CShapeTextProperties::GetHyperlinkABGR()const
		{
			if(bodyPr.parentFileIs<Slide>())
				return bodyPr.parentFileAs<Slide>().GetABGRFromMap(_T("hlink"));
			else if(bodyPr.parentFileIs<SlideLayout>())
				return bodyPr.parentFileAs<SlideLayout>().GetABGRFromMap(_T("hlink"));
			else if(bodyPr.parentFileIs<SlideMaster>())
				return bodyPr.parentFileAs<SlideMaster>().GetABGRFromMap(_T("hlink"));
			else return 0;
		}

		void CShapeTextProperties::SetParentFilePointer(const WrapperFile* pFile)
		{
			bodyPr.SetParentFilePointer(pFile);

			m_pFile = pFile;

			//for(int i = 0; i < 10; i ++)
			//{
			//	m_levels[i]->SetParentFilePointer(pFile);
			//	m_body[i]->SetParentFilePointer(pFile);
			//}

			if(m_FontStyle.is_init())
				m_FontStyle->SetParentFilePointer(pFile);
		}
	} // namespace Logic
} // namespace PPTX
