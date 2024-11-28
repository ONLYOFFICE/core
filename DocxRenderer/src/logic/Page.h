#pragma once

#include "elements/Paragraph.h"
#include "elements/Shape.h"
#include "managers/ImageManager.h"
#include "managers/FontStyleManager.h"
#include "managers/ParagraphStyleManager.h"
#include "../../convert_params.h"

namespace NSDocxRenderer
{
	class CPage
	{
	public:
		struct CManagers
		{
			CManagers() = default;
			CManagers(const CManagers& other) = default;
			~CManagers() = default;

			CImageManager*             pImageManager;
			CFontStyleManager*         pFontStyleManager;
			CParagraphStyleManager*    pParagraphStyleManager;
			CFontManager*              pFontManager;
			CFontSelector*			   pFontSelector;
		};

		double m_dWidth {0.0};
		double m_dHeight {0.0};

		LONG m_lCurrentCommand{0};
		LONG m_lClipMode{0};

		TextAssociationType m_eTextAssociationType{TextAssociationType::tatPlainParagraph};
		NSFonts::IApplicationFonts* m_pAppFonts{nullptr};

		NSStructures::CFont     m_oFont;
		NSStructures::CPen      m_oPen;
		NSStructures::CBrush    m_oBrush;
		NSStructures::CShadow   m_oShadow;
		NSStructures::CEdgeText m_oEdgeText;

		Aggplus::CMatrix m_oTransform;
		CManagers        m_oManagers;

		CVectorGraphics  m_oCurrVectorGraphics, m_oClipVectorGraphics;
		CContTextBuilder m_oContBuilder;

		std::vector<std::shared_ptr<CContText>> m_arConts;
		std::vector<std::shared_ptr<CTextLine>> m_arTextLines;
		std::vector<std::shared_ptr<CContText>> m_arDiacriticalSymbols;
		std::vector<std::shared_ptr<CShape>>    m_arShapes;

		std::vector<std::wstring>   m_arCompleteObjectsXml;
		std::vector<std::shared_ptr<CBaseItem>>  m_arOutputObjects;

		bool m_bIsDeleteTextClipPage{true};
		bool m_bIsRecalcFontSize    {true};
		bool m_bIsGradient          {false};
		bool m_bUseDefaultFont      {false};
		bool m_bWriteStyleRaw       {false};

		CPage(NSFonts::IApplicationFonts* pAppFonts, const CManagers& oManagers);
		~CPage();

		void BeginCommand(DWORD lType);
		void EndCommand(DWORD lType);

		void Clear();
		void DeleteTextClipPage();

		void WriteImage(const std::shared_ptr<CImageInfo> pInfo, double& fX, double& fY, double& fWidth, double& fHeight);

		void PathMoveTo(double& dX, double& dY);
		void PathLineTo(double& dX, double& dY);
		void PathCurveTo(double& dX1, double& dY1, double& dX2, double& dY2, double& dX3, double& dY3);
		void PathStart();
		void PathEnd();
		void PathClose();

		void DrawPath(LONG lType, const std::shared_ptr<CImageInfo> pInfo);
		void AddText(
			const PUINT pUnicodes,
			const PUINT pGids,
			const UINT& nCount,
			const double& fX,
			const double& fY,
			const double& fWidth,
			const double& fHeight,
			const double& fBaseLineOffset);

		void Analyze();
		void Record(NSStringUtils::CStringBuilder& oWriter, bool bIsLastPage);
		void ReorderShapesForPptx();

	private:
		void BuildDiacriticalSymbols();
		void BuildTextLines();
		void AnalyzeTextLines();
		void SplitLines();
		void CalcSelected();
		void BuildParagraphes();

		std::vector<std::vector<std::shared_ptr<CTextLine>>> GetLinesByGroups();

		void MergeShapes();
		void CalcShapesRotation();

		// strikeouts, underlines, highlights, outline
		void AnalyzeEffects();

		bool IsLineCrossingText(std::shared_ptr<CShape> pShape, std::shared_ptr<CContText> pCont);
		bool IsLineBelowText(std::shared_ptr<CShape> pShape, std::shared_ptr<CContText> pCont);
		bool IsHighlight(std::shared_ptr<CShape> pShape, std::shared_ptr<CContText> pCont);
		bool IsOutline(std::shared_ptr<CShape> pShape, std::shared_ptr<CContText> pCont);

		void AnalyzeDropCaps();
		void AnalyzeConts();
		void AddDiacriticalSymbols();
		void MergeLinesByVertAlignType();

		void AnalyzeShapes();
		void DetermineLinesType();

		void MergeConts();
		void DetermineDominantGraphics();

		bool IsShapeBorderBetweenVertical(std::shared_ptr<CTextLine> pFirst, std::shared_ptr<CTextLine> pSecond) const noexcept;
		bool IsShapeBorderBetweenHorizontal(std::shared_ptr<CTextLine> pFirst, std::shared_ptr<CTextLine> pSecond) const noexcept;
		bool IsShapeBorderTrough(std::shared_ptr<CContText> pItem, double& dXCrossing, double& dYCrossing) const noexcept;

		std::shared_ptr<CShape> CreateSingleLineShape(std::shared_ptr<CTextLine>& pLine);
		std::shared_ptr<CShape> CreateSingleParagraphShape(std::shared_ptr<CParagraph>& pParagraph);

		// конвертим m_arImages, m_arShapes, m_arParagraphs в xml-строку
		void ToXml(NSStringUtils::CStringBuilder& oWriter);
		void WriteSectionToFile(bool bLastPage, NSStringUtils::CStringBuilder& oWriter);

		size_t m_nShapeOrder = 0;
	};
}
