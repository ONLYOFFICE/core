#pragma once
#ifndef PPTX_LOGIC_SHAPETEXTPROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_SHAPETEXTPROPERTIES_INCLUDE_H_

#include "RunProperties.h"
#include "TextListStyle.h"
#include "TxBody.h"
#include "FontRef.h"
#include "TextParagraphPr.h"
#include "BodyPr.h"
#include "TextFont.h"

namespace PPTX
{
	namespace Logic
	{
		class CFontInfo
		{
		public:
			LONG	FontRef;
			CString	strFontName;
			CString strPanose;
			CString strPitchFamily;
			BYTE	Charset;

		public:
			CFontInfo()
			{
				FontRef			= -1;
				strFontName		= _T("");
				strPanose		= _T("");
				strPitchFamily	= _T("");
				Charset			= 0;
			}
			CFontInfo(const CFontInfo& oSrc)
			{
				*this = oSrc;
			}
			CFontInfo& operator=(const CFontInfo& oSrc)
			{
				FontRef			= oSrc.FontRef;
				strFontName		= oSrc.strFontName;
				strPanose		= oSrc.strPanose;
				strPitchFamily	= oSrc.strPitchFamily;
				Charset			= oSrc.Charset;

				return *this;
			}
		};

		class CShapeTextProperties
		{
		public:
			CShapeTextProperties();
			virtual ~CShapeTextProperties();

		private:
			nullable<FontRef>			m_FontStyle;
			LONG						m_lTextType;
			nullable<TextParagraphPr>	m_levels[10];
			nullable<TextParagraphPr>	m_body[10];
			BodyPr bodyPr;

			// просто указатель - он должен выставиться из темы
			std::vector<nullable<TextParagraphPr>*>*	m_masters;

			TextFont					MajorLatin;
			TextFont					MinorLatin;

			bool						m_bIsFontRefFromSlide;
			int							m_lMasterTextSize; // заглушка под таблицы.

			bool						m_bIsSlideShape;

			const WrapperFile*			m_pFile;

		public:
			void FillTextType(const LONG& lTextMasterType);
			void FillFromTextBody(const nullable<TxBody>& Src, const nullable<TextParagraphPr>* bodyStyles);
			void FillFontRef(const FontRef& Src, const bool& bIsSlideProperty);
			void FillMasterFontSize(int size);

            AVSINLINE void SetMajorLatin(const TextFont& mjltFont){MajorLatin = mjltFont;};
            AVSINLINE void SetMinorLatin(const TextFont& mnltFont){MinorLatin = mnltFont;};

            AVSINLINE void SetMasterStyles(std::vector<nullable<TextParagraphPr>*>* pStyles) { m_masters = pStyles; }

            AVSINLINE nullable<TextParagraphPr>* GetLevels()
			{ 
				if (!m_bIsSlideShape) 
					return NULL;
				return m_levels; 
			}
            AVSINLINE nullable<TextParagraphPr>* GetLevelsBody() { return m_body; }
            AVSINLINE LONG GetTextType() { return m_lTextType; }
            AVSINLINE LONG GetFontRef(bool& bIsSlideSetUp)
			{
				bIsSlideSetUp = m_bIsFontRefFromSlide;

				if (m_FontStyle.is_init() && m_FontStyle->idx.is_init())
				{
					return (m_FontStyle->idx->get() == _T("minor")) ? 1 : 0;
				}
				return -1;
			}

			CString GetAnchor()const{return bodyPr.anchor.get_value_or(_T("t"));};

			nullable_base<WORD> GetParagraphAlgn		(int level, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<LONG>	GetParagraphLeftMargin	(int level, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<LONG> GetParagraphIndent		(int level, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<LONG> GetParagraphDefTabSz	(int level, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<WORD> GetParagraphFontAlgn	(int level, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<WORD> GetParagraphLatinLnBrk	(int level, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<WORD> GetParagraphRtl			(int level, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<LONG> GetParagraphLnSpc		(int level, const nullable<TextParagraphPr>& pParagraph, const double& LnSpcReduction)const;
			nullable_base<LONG> GetParagraphSpcAft		(int level, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<LONG> GetParagraphSpcBef		(int level, const nullable<TextParagraphPr>& pParagraph)const;

			nullable_base<BOOL>	GetHasBullet			(int level, const nullable<TextParagraphPr>& pParagraph)const;

			nullable_base<BOOL> GetRunBold				(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<BOOL> GetRunItalic			(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<BOOL> GetRunUnderline			(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<BOOL> GetRunStrike			(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<WORD> GetRunCap				(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<double> GetRunBaseline		(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const;
			nullable_base<WORD> GetRunSize				(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph, const double& FontScale)const;

			nullable_base<CFontInfo> GetRunFont			(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const;

			PPTX::Logic::UniColor	GetColor(int level, const nullable<RunProperties>& pRun, const nullable<TextParagraphPr>& pParagraph)const;
			PPTX::Logic::UniColor	GetColorBullet(int level, const nullable<TextParagraphPr>& pParagraph)const;

			DWORD GetHyperlinkRGBA()const;
			DWORD GetHyperlinkARGB()const;
			DWORD GetHyperlinkBGRA()const;
			DWORD GetHyperlinkABGR()const;

			void SetParentFilePointer(const WrapperFile* pFile);
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SHAPETEXTPROPERTIES_INCLUDE_H_
