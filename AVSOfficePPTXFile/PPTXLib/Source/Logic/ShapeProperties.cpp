#include "ShapeProperties.h"
#include "Fills/SolidFill.h"
#include "Fills/GradFill.h"
#include "../Slide.h"
#include "../SlideLayout.h"
#include "../SlideMaster.h"
#include <stdio.h>

namespace PPTX
{
	namespace Logic
	{

		int GetIntegerFromHex(const std::string& string)
		{
			int i = 0;
			sscanf(string.c_str(), "%x", &i);
			return i;
		}

		ShapeProperties::ShapeProperties()
		{
			for(int i = 0; i < 10; i++)
			{
				TextParagraphPr ppr;
				RunProperties rpr;
				ppr.defRPr = rpr;

				levels[i] = ppr;
				masters[i] = ppr;
			}
		}

		ShapeProperties::~ShapeProperties()
		{
		}

		void ShapeProperties::FillFromTextBody(const nullable_property<TxBody>& Src)
		{
			if(Src.is_init())
			{
				Src->bodyPr->Merge(bodyPr);
				if(Src->lstStyle.is_init())
				{
					for(int i = 0; i < 10; i++)
					{
						if(Src->lstStyle->levels[i].is_init())
							Src->lstStyle->levels[i]->Merge(levels[i]);
					}
				}
			}
		}

		void ShapeProperties::FillFromTextListStyle(const nullable_property<TextListStyle>& Src)
		{
			if(Src.is_init())
			{
				for(int i = 0; i < 10; i++)
				{
					if(Src->levels[i].is_init())
						Src->levels[i]->Merge(masters[i]);
				}
			}
		}

		void ShapeProperties::FillFromTextListStyle(const property<TextListStyle>& Src)
		{
			for(int i = 0; i < 10; i++)
			{
				if(Src->levels[i].is_init())
					Src->levels[i]->Merge(masters[i]);
			}
		}

		void ShapeProperties::FillFromTextListStyle(const TextListStyle& Src)
		{
			for(int i = 0; i < 10; i++)
			{
				if(Src.levels[i].is_init())
					Src.levels[i]->Merge(masters[i]);
			}
		}

		void ShapeProperties::FillFontRef(const FontRef& Src)
		{
			fontStyle = Src;
		}

		void ShapeProperties::FillMasterFontSize(int size)
		{
			for(int i = 0; i < 10; i++)
			{
				if(masters[i]->defRPr.is_init())
					masters[i]->defRPr->sz = size;
				else
				{
					PPTX::Logic::RunProperties rpr;
					rpr.sz = size;
					masters[i]->defRPr = rpr;
				}
			}
		}

		std::string ShapeProperties::GetParagraphAlgn(int level, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pParagraph.is_init())
				if(pParagraph->algn.is_init())
					return pParagraph->algn.get();
			return levels[level]->algn.get_value_or(masters[level]->algn.get_value_or("l"));
		}

		int	ShapeProperties::GetParagraphLeftMargin(int level, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pParagraph.is_init())
				if(pParagraph->marL.is_init())
					return pParagraph->marL.get();
//			return levels[level]->marL.get_value_or(levels[9].marL.get_value_or(347663));
			return levels[level]->marL.get_value_or(masters[level]->marL.get_value_or(0));
		}

		int ShapeProperties::GetParagraphIndent(int level, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pParagraph.is_init())
				if(pParagraph->indent.is_init())
					return pParagraph->indent.get();
//			return levels[level]->indent.get_value_or(levels[9].indent.get_value_or(-342900));
			return levels[level]->indent.get_value_or(masters[level]->indent.get_value_or(0));
		}

		int ShapeProperties::GetParagraphDefTabSz(int level, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pParagraph.is_init())
				if(pParagraph->defTabSz.is_init())
					return pParagraph->defTabSz.get();
//			return levels[level]->defTabSz.get_value_or(levels[9].defTabSz.get_value_or(376300));
			return levels[level]->defTabSz.get_value_or(masters[level]->defTabSz.get_value_or(376300));
		}

		std::string ShapeProperties::GetParagraphFontAlgn(int level, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pParagraph.is_init())
				if(pParagraph->fontAlgn.is_init())
					return pParagraph->fontAlgn.get();
//			return levels[level]->fontAlgn.get_value_or(levels[9].fontAlgn.get_value_or("base"));
			return levels[level]->fontAlgn.get_value_or(masters[level]->fontAlgn.get_value_or("base"));
		}

		bool ShapeProperties::GetParagraphLatinLnBrk(int level, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pParagraph.is_init())
				if(pParagraph->latinLnBrk.is_init())
					return pParagraph->latinLnBrk.get();
//			return levels[level]->latinLnBrk.get_value_or(levels[9].latinLnBrk.get_value_or(true));
			return levels[level]->latinLnBrk.get_value_or(masters[level]->latinLnBrk.get_value_or(false));
		}

		bool ShapeProperties::GetParagraphRtl(int level, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pParagraph.is_init())
				if(pParagraph->rtl.is_init())
					return pParagraph->rtl.get();
//			return levels[level]->rtl.get_value_or(levels[9].rtl.get_value_or(false));
			return levels[level]->rtl.get_value_or(masters[level]->rtl.get_value_or(false));
		}

		int ShapeProperties::GetParagraphLnSpc(int level, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pParagraph.is_init())
				if(pParagraph->lnSpc.is_init())
					return pParagraph->lnSpc->GetVal();
			if(levels[level]->lnSpc.is_init())
				return levels[level]->lnSpc->GetVal();
			if(masters[level]->lnSpc.is_init())
				return masters[level]->lnSpc->GetVal();
			return 100;//0;
		}

		int ShapeProperties::GetParagraphSpcAft(int level, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pParagraph.is_init())
				if(pParagraph->spcAft.is_init())
					return pParagraph->spcAft->GetVal();
			if(levels[level]->spcAft.is_init())
				return levels[level]->spcAft->GetVal();
			if(masters[level]->spcAft.is_init())
				return masters[level]->spcAft->GetVal();
			return 0;
		}

		int ShapeProperties::GetParagraphSpcBef(int level, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pParagraph.is_init())
				if(pParagraph->spcBef.is_init())
					return pParagraph->spcBef->GetVal();
			if(levels[level]->spcBef.is_init())
				return levels[level]->spcBef->GetVal();
			if(masters[level]->spcBef.is_init())
				return masters[level]->spcBef->GetVal();
			return 0;
		}

		bool ShapeProperties::HasParagraphBullet(int level, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pParagraph.is_init())
				if(pParagraph->ParagraphBullet.is_init())
					return pParagraph->ParagraphBullet.has_bullet();
			if(levels[level]->ParagraphBullet.is_init())
				return levels[level]->ParagraphBullet.has_bullet();
			return masters[level]->ParagraphBullet.has_bullet();
		}

		bool ShapeProperties::GetRunBold(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pRun.is_init())
				if(pRun->b.is_init())
					return pRun->b.get();
			if(pParagraph.is_init())
				if(pParagraph->defRPr.is_init())
					if(pParagraph->defRPr->b.is_init())
						return pParagraph->defRPr->b.get();
//			return levels[level].defRPr->b.get_value_or(levels[9].defRPr->b.get_value_or(false));
			return levels[level]->defRPr->b.get_value_or(masters[level]->defRPr->b.get_value_or(false));
		}

		bool ShapeProperties::GetRunItalic(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pRun.is_init())
				if(pRun->i.is_init())
					return pRun->i.get();
			if(pParagraph.is_init())
				if(pParagraph->defRPr.is_init())
					if(pParagraph->defRPr->i.is_init())
						return pParagraph->defRPr->i.get();
//			return levels[level]->defRPr->i.get_value_or(levels[9].defRPr->i.get_value_or(false));
			return levels[level]->defRPr->i.get_value_or(masters[level]->defRPr->i.get_value_or(false));
		}

		std::string ShapeProperties::GetRunUnderline(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pRun.is_init())
				if(pRun->u.is_init())
					return pRun->u.get();
			if(pParagraph.is_init())
				if(pParagraph->defRPr.is_init())
					if(pParagraph->defRPr->u.is_init())
						return pParagraph->defRPr->u.get();
			return levels[level]->defRPr->u.get_value_or(masters[level]->defRPr->u.get_value_or("none"));
		}

		std::string ShapeProperties::GetRunStrike(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pRun.is_init())
				if(pRun->strike.is_init())
					return pRun->strike.get();
			if(pParagraph.is_init())
				if(pParagraph->defRPr.is_init())
					if(pParagraph->defRPr->strike.is_init())
						return pParagraph->defRPr->strike.get();
			return levels[level]->defRPr->strike.get_value_or(masters[level]->defRPr->strike.get_value_or("noStrike"));
		}

		std::string ShapeProperties::GetRunCap(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pRun.is_init())
				if(pRun->cap.is_init())
					return pRun->cap.get();
			if(pParagraph.is_init())
				if(pParagraph->defRPr.is_init())
					if(pParagraph->defRPr->cap.is_init())
						return pParagraph->defRPr->cap.get();
			return levels[level]->defRPr->cap.get_value_or(masters[level]->defRPr->cap.get_value_or("none"));
		}

		int ShapeProperties::GetRunBaseline(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pRun.is_init())
				if(pRun->baseline.is_init())
					return pRun->baseline.get();
			if(pParagraph.is_init())
				if(pParagraph->defRPr.is_init())
					if(pParagraph->defRPr->baseline.is_init())
						return pParagraph->defRPr->baseline.get();
			return levels[level]->defRPr->baseline.get_value_or(masters[level]->defRPr->baseline.get_value_or(0));
		}

		int ShapeProperties::GetRunSize(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pRun.is_init())
				if(pRun->sz.is_init())
					return pRun->sz.get();
			if(pParagraph.is_init())
				if(pParagraph->defRPr.is_init())
					if(pParagraph->defRPr->sz.is_init())
						return pParagraph->defRPr->sz.get();
//			return levels[level]->defRPr->sz.get_value_or(levels[9].defRPr->sz.get_value_or(1000));
			return levels[level]->defRPr->sz.get_value_or(masters[level]->defRPr->sz.get_value_or(1800));
		}

		int ShapeProperties::GetRunSize(int level)const
		{
			return levels[level]->defRPr->sz.get_value_or(masters[level]->defRPr->sz.get_value_or(1800));
		}

		std::string ShapeProperties::GetRunFont(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			std::string strFontName = "";
			if((pRun.is_init()) && (pRun->latin.is_init()))
				strFontName = pRun->latin->typeface.get();
			else if((pParagraph.is_init()) && (pParagraph->defRPr.is_init()) && (pParagraph->defRPr->latin.is_init()))
				strFontName = pParagraph->defRPr->latin->typeface.get();
			else if(levels[level]->defRPr->latin.is_init())
				strFontName = levels[level]->defRPr->latin->typeface.get();
			else if(fontStyle.is_init())
				strFontName = (fontStyle->idx == "minor")?"+mn-lt":"+mj-lt";
			else if(masters[level]->defRPr->latin.is_init())
				strFontName = masters[level]->defRPr->latin->typeface.get();

			if(strFontName == "+mj-lt")
				strFontName = MajorLatin->typeface.get();
			else if(strFontName == "+mn-lt")
				strFontName = MinorLatin->typeface.get();
			else if(strFontName == "")
				strFontName = MinorLatin->typeface.get();
			return strFontName;
		}

		std::string ShapeProperties::GetRunPanose(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			std::string panose = "";
			std::string style = "";
			if((pRun.is_init()) && (pRun->latin.is_init()))
				panose = pRun->latin->panose.get_value_or("");
			else if((pParagraph.is_init()) && (pParagraph->defRPr.is_init()) && (pParagraph->defRPr->latin.is_init()))
				panose = pParagraph->defRPr->latin->panose.get_value_or("");
			else if(levels[level]->defRPr->latin.is_init())
				panose = levels[level]->defRPr->latin->panose.get_value_or("");
			else if(fontStyle.is_init())
				style = fontStyle->idx.get_value_or("major");
			else if(masters[level]->defRPr->latin.is_init())
				panose = masters[level]->defRPr->latin->panose.get_value_or("");

			if(style == "major")
				panose = MajorLatin->panose.get_value_or("");
			else if(style == "minor")
				panose = MinorLatin->panose.get_value_or("");
			return panose;
		}

		BYTE ShapeProperties::GetRunCharset(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			std::string charset = "";
			std::string style = "";
			if((pRun.is_init()) && (pRun->latin.is_init()))
				charset = pRun->latin->charset.get_value_or("");
			else if((pParagraph.is_init()) && (pParagraph->defRPr.is_init()) && (pParagraph->defRPr->latin.is_init()))
				charset = pParagraph->defRPr->latin->charset.get_value_or("");
			else if(levels[level]->defRPr->latin.is_init())
				charset = levels[level]->defRPr->latin->charset.get_value_or("");
			else if(fontStyle.is_init())
				style = fontStyle->idx.get_value_or("major");
			else if(masters[level]->defRPr->latin.is_init())
				charset = masters[level]->defRPr->latin->charset.get_value_or("");

			if(style == "major")
				charset = MajorLatin->charset.get_value_or("");
			else if(style == "minor")
				charset = MinorLatin->charset.get_value_or("");
			if(charset == "")
				charset = "01";
			return GetIntegerFromHex(charset);
		}

		BYTE ShapeProperties::GetRunPitchFamily(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			std::string pitchFamily = "";
			std::string style = "";
			if((pRun.is_init()) && (pRun->latin.is_init()))
				pitchFamily = pRun->latin->pitchFamily.get_value_or("");
			else if((pParagraph.is_init()) && (pParagraph->defRPr.is_init()) && (pParagraph->defRPr->latin.is_init()))
				pitchFamily = pParagraph->defRPr->latin->pitchFamily.get_value_or("");
			else if(levels[level]->defRPr->latin.is_init())
				pitchFamily = levels[level]->defRPr->latin->pitchFamily.get_value_or("");
			else if(fontStyle.is_init())
				style = fontStyle->idx.get_value_or("major");
			else if(masters[level]->defRPr->latin.is_init())
				pitchFamily = masters[level]->defRPr->latin->pitchFamily.get_value_or("");

			if(style == "major")
				pitchFamily = MajorLatin->pitchFamily.get_value_or("");
			else if(style == "minor")
				pitchFamily = MinorLatin->pitchFamily.get_value_or("");
			if(pitchFamily == "")
				pitchFamily = "0";
			return GetIntegerFromHex(pitchFamily);
		}


		DWORD ShapeProperties::GetRunRGBA(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pRun.is_init())
			{
				if(pRun->Fill.is<SolidFill>())
					return pRun->Fill.as<SolidFill>().Color.GetRGBA();
				if(pRun->Fill.is<GradFill>())
					return pRun->Fill.as<GradFill>().GsLst->front().color.GetRGBA();
			}

			if((pParagraph.is_init()) && (pParagraph->defRPr.is_init()))
			{
				if(pParagraph->defRPr->Fill.is<SolidFill>())
					return pParagraph->defRPr->Fill.as<SolidFill>().Color.GetRGBA();
				if(pParagraph->defRPr->Fill.is<GradFill>())
					return pParagraph->defRPr->Fill.as<GradFill>().GsLst->front().color.GetRGBA();
			}

			if(levels[level]->defRPr->Fill.is<SolidFill>())
				return levels[level]->defRPr->Fill.as<SolidFill>().Color.GetRGBA();
			if(levels[level]->defRPr->Fill.is<GradFill>())
				return levels[level]->defRPr->Fill.as<GradFill>().GsLst->front().color.GetRGBA();

			if(fontStyle.is_init())
				return fontStyle->Color.GetRGBA();

			if(masters[level]->defRPr->Fill.is<SolidFill>())
				return masters[level]->defRPr->Fill.as<SolidFill>().Color.GetRGBA();
			if(masters[level]->defRPr->Fill.is<GradFill>())
				return masters[level]->defRPr->Fill.as<GradFill>().GsLst->front().color.GetRGBA();

			return 0;
		}

		DWORD ShapeProperties::GetRunARGB(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pRun.is_init())
			{
				if(pRun->Fill.is<SolidFill>())
					return pRun->Fill.as<SolidFill>().Color.GetARGB();
				if(pRun->Fill.is<GradFill>())
					return pRun->Fill.as<GradFill>().GsLst->front().color.GetARGB();
			}

			if((pParagraph.is_init()) && (pParagraph->defRPr.is_init()))
			{
				if(pParagraph->defRPr->Fill.is<SolidFill>())
					return pParagraph->defRPr->Fill.as<SolidFill>().Color.GetARGB();
				if(pParagraph->defRPr->Fill.is<GradFill>())
					return pParagraph->defRPr->Fill.as<GradFill>().GsLst->front().color.GetARGB();
			}

			if(levels[level]->defRPr->Fill.is<SolidFill>())
				return levels[level]->defRPr->Fill.as<SolidFill>().Color.GetARGB();
			if(levels[level]->defRPr->Fill.is<GradFill>())
				return levels[level]->defRPr->Fill.as<GradFill>().GsLst->front().color.GetARGB();

			if(fontStyle.is_init())
				return fontStyle->Color.GetARGB();

			if(masters[level]->defRPr->Fill.is<SolidFill>())
				return masters[level]->defRPr->Fill.as<SolidFill>().Color.GetARGB();
			if(masters[level]->defRPr->Fill.is<GradFill>())
				return masters[level]->defRPr->Fill.as<GradFill>().GsLst->front().color.GetARGB();

			return 0;
		}

		DWORD ShapeProperties::GetRunBGRA(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pRun.is_init())
			{
				if(pRun->Fill.is<SolidFill>())
					return pRun->Fill.as<SolidFill>().Color.GetBGRA();
				if(pRun->Fill.is<GradFill>())
					return pRun->Fill.as<GradFill>().GsLst->front().color.GetBGRA();
			}

			if((pParagraph.is_init()) && (pParagraph->defRPr.is_init()))
			{
				if(pParagraph->defRPr->Fill.is<SolidFill>())
					return pParagraph->defRPr->Fill.as<SolidFill>().Color.GetBGRA();
				if(pParagraph->defRPr->Fill.is<GradFill>())
					return pParagraph->defRPr->Fill.as<GradFill>().GsLst->front().color.GetBGRA();
			}

			if(levels[level]->defRPr->Fill.is<SolidFill>())
				return levels[level]->defRPr->Fill.as<SolidFill>().Color.GetBGRA();
			if(levels[level]->defRPr->Fill.is<GradFill>())
				return levels[level]->defRPr->Fill.as<GradFill>().GsLst->front().color.GetBGRA();

			if(fontStyle.is_init())
				return fontStyle->Color.GetBGRA();

			if(masters[level]->defRPr->Fill.is<SolidFill>())
				return masters[level]->defRPr->Fill.as<SolidFill>().Color.GetBGRA();
			if(masters[level]->defRPr->Fill.is<GradFill>())
				return masters[level]->defRPr->Fill.as<GradFill>().GsLst->front().color.GetBGRA();

			return 0;
		}

		DWORD ShapeProperties::GetRunABGR(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const
		{
			if(pRun.is_init())
			{
				if(pRun->Fill.is<SolidFill>())
					return pRun->Fill.as<SolidFill>().Color.GetABGR();
				if(pRun->Fill.is<GradFill>())
					return pRun->Fill.as<GradFill>().GsLst->front().color.GetABGR();
			}

			if((pParagraph.is_init()) && (pParagraph->defRPr.is_init()))
			{
				if(pParagraph->defRPr->Fill.is<SolidFill>())
					return pParagraph->defRPr->Fill.as<SolidFill>().Color.GetABGR();
				if(pParagraph->defRPr->Fill.is<GradFill>())
					return pParagraph->defRPr->Fill.as<GradFill>().GsLst->front().color.GetABGR();
			}

			if(levels[level]->defRPr->Fill.is<SolidFill>())
				return levels[level]->defRPr->Fill.as<SolidFill>().Color.GetABGR();
			if(levels[level]->defRPr->Fill.is<GradFill>())
				return levels[level]->defRPr->Fill.as<GradFill>().GsLst->front().color.GetABGR();

			if(fontStyle.is_init())
				return fontStyle->Color.GetABGR();

			if(masters[level]->defRPr->Fill.is<SolidFill>())
				return masters[level]->defRPr->Fill.as<SolidFill>().Color.GetABGR();
			if(masters[level]->defRPr->Fill.is<GradFill>())
				return masters[level]->defRPr->Fill.as<GradFill>().GsLst->front().color.GetABGR();

			return 0;
		}

		void ShapeProperties::SetParentFilePointer(const WrapperFile& pFile)
		{
			bodyPr.SetParentFilePointer(pFile);
			for(int i = 0; i < 10; i ++)
			{
				levels[i]->SetParentFilePointer(pFile);
				masters[i]->SetParentFilePointer(pFile);
			}

			if(fontStyle.is_init())
				fontStyle->SetParentFilePointer(pFile);
		}

		DWORD ShapeProperties::GetHyperlinkRGBA()const
		{
			if(bodyPr.parentFileIs<Slide>())
				return bodyPr.parentFileAs<Slide>().GetRGBAFromMap("hlink");
			else if(bodyPr.parentFileIs<SlideLayout>())
				return bodyPr.parentFileAs<SlideLayout>().GetRGBAFromMap("hlink");
			else if(bodyPr.parentFileIs<SlideMaster>())
				return bodyPr.parentFileAs<SlideMaster>().GetRGBAFromMap("hlink");
			else return 0;
		}

		DWORD ShapeProperties::GetHyperlinkARGB()const
		{
			if(bodyPr.parentFileIs<Slide>())
				return bodyPr.parentFileAs<Slide>().GetARGBFromMap("hlink");
			else if(bodyPr.parentFileIs<SlideLayout>())
				return bodyPr.parentFileAs<SlideLayout>().GetARGBFromMap("hlink");
			else if(bodyPr.parentFileIs<SlideMaster>())
				return bodyPr.parentFileAs<SlideMaster>().GetARGBFromMap("hlink");
			else return 0;
		}

		DWORD ShapeProperties::GetHyperlinkBGRA()const
		{
			if(bodyPr.parentFileIs<Slide>())
				return bodyPr.parentFileAs<Slide>().GetBGRAFromMap("hlink");
			else if(bodyPr.parentFileIs<SlideLayout>())
				return bodyPr.parentFileAs<SlideLayout>().GetBGRAFromMap("hlink");
			else if(bodyPr.parentFileIs<SlideMaster>())
				return bodyPr.parentFileAs<SlideMaster>().GetBGRAFromMap("hlink");
			else return 0;
		}

		DWORD ShapeProperties::GetHyperlinkABGR()const
		{
			if(bodyPr.parentFileIs<Slide>())
				return bodyPr.parentFileAs<Slide>().GetABGRFromMap("hlink");
			else if(bodyPr.parentFileIs<SlideLayout>())
				return bodyPr.parentFileAs<SlideLayout>().GetABGRFromMap("hlink");
			else if(bodyPr.parentFileIs<SlideMaster>())
				return bodyPr.parentFileAs<SlideMaster>().GetABGRFromMap("hlink");
			else return 0;
		}

	} // namespace Logic
} // namespace PPTX