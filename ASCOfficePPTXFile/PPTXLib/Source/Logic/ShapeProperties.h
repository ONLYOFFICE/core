#pragma once
#ifndef PPTX_LOGIC_SHAPEPROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_SHAPEPROPERTIES_INCLUDE_H_

#include <property.h>
#include <nullable_property.h>
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

		class ShapeProperties
		{
		public:
			ShapeProperties();
			virtual ~ShapeProperties();

		private:
			nullable_property<FontRef> fontStyle;
			nullable_property<TextParagraphPr> levels[10];
			nullable_property<TextParagraphPr> masters[10];
			BodyPr bodyPr;

			//std::string MajorLatin;
			//std::string MinorLatin;
			property<TextFont> MajorLatin;
			property<TextFont> MinorLatin;

		public:
			void FillFromTextListStyle(const nullable_property<TextListStyle>& Src);
			void FillFromTextListStyle(const property<TextListStyle>& Src);
			void FillFromTextListStyle(const TextListStyle& Src);
			void FillFromTextBody(const nullable_property<TxBody>& Src);
			void FillFontRef(const FontRef& Src);
			void FillMasterFontSize(int size);

			//void SetMajorLatin(const std::string& mjltFont){MajorLatin = mjltFont;};
			//void SetMinorLatin(const std::string& mnltFont){MinorLatin = mnltFont;};
			void SetMajorLatin(const property<TextFont>& mjltFont){MajorLatin = mjltFont;};
			void SetMinorLatin(const property<TextFont>& mnltFont){MinorLatin = mnltFont;};

			std::string GetAnchor()const{return bodyPr.anchor.get_value_or("t");};

			std::string GetParagraphAlgn	(int level, const nullable_property<TextParagraphPr>& pParagraph)const;
			int	GetParagraphLeftMargin		(int level, const nullable_property<TextParagraphPr>& pParagraph)const;
			int GetParagraphIndent			(int level, const nullable_property<TextParagraphPr>& pParagraph)const;
			int GetParagraphDefTabSz		(int level, const nullable_property<TextParagraphPr>& pParagraph)const;
			std::string GetParagraphFontAlgn(int level, const nullable_property<TextParagraphPr>& pParagraph)const;
			bool GetParagraphLatinLnBrk		(int level, const nullable_property<TextParagraphPr>& pParagraph)const;
			bool GetParagraphRtl			(int level, const nullable_property<TextParagraphPr>& pParagraph)const;
			int GetParagraphLnSpc			(int level, const nullable_property<TextParagraphPr>& pParagraph)const;
			int GetParagraphSpcAft			(int level, const nullable_property<TextParagraphPr>& pParagraph)const;
			int GetParagraphSpcBef			(int level, const nullable_property<TextParagraphPr>& pParagraph)const;

			bool HasParagraphBullet			(int level, const nullable_property<TextParagraphPr>& pParagraph)const;

			bool GetRunBold					(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			bool GetRunItalic				(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			std::string GetRunUnderline		(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			std::string GetRunStrike		(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			std::string GetRunCap			(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			int GetRunBaseline				(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			int GetRunSize					(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			int GetRunSize					(int level)const;

			std::string GetRunFont			(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			std::string GetRunPanose		(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			BYTE GetRunCharset				(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			BYTE GetRunPitchFamily			(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;

			DWORD GetRunRGBA				(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			DWORD GetRunARGB				(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			DWORD GetRunBGRA				(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;
			DWORD GetRunABGR				(int level, const nullable_property<RunProperties>& pRun, const nullable_property<TextParagraphPr>& pParagraph)const;

			DWORD GetHyperlinkRGBA()const;
			DWORD GetHyperlinkARGB()const;
			DWORD GetHyperlinkBGRA()const;
			DWORD GetHyperlinkABGR()const;

			void SetParentFilePointer(const WrapperFile& pFile);
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SHAPEPROPERTIES_INCLUDE_H