#pragma once
#include "../../../DesktopEditor/graphics/pro/Graphics.h"
#include "elements/Paragraph.h"
#include "elements/Shape.h"
#include "managers/FontStyleManager.h"
#include "managers/ParagraphStyleManager.h"
#include "styles/ParagraphStyle.h"
#include "elements/DropCap.h"

namespace NSDocxRenderer
{
	class CPage
	{
	public:
		double m_dWidth {0.0};
		double m_dHeight {0.0};

		LONG m_lCurrentCommand {0};

		TextAssociationType m_eTextAssociationType {TextAssociationType::tatPlainParagraph};

		bool m_bUseDefaultFont{false};
		bool m_bWriteStyleRaw {false};

		NSStructures::CFont*        m_pFont      {nullptr};
		NSStructures::CPen*         m_pPen       {nullptr};
		NSStructures::CBrush*       m_pBrush     {nullptr};
		NSStructures::CShadow*      m_pShadow    {nullptr};
		NSStructures::CEdgeText*    m_pEdgeText  {nullptr};

		Aggplus::CMatrix*                      m_pTransform              {nullptr};
		Aggplus::CGraphicsPathSimpleConverter* m_pSimpleGraphicsConverter{nullptr};

		CFontStyleManager*          m_pFontStyleManager     {nullptr};
		CParagraphStyleManager*     m_pParagraphStyleManager{nullptr};
		CFontManager*               m_pFontManager          {nullptr};
		CFontSelector*				m_pFontSelector         {nullptr};
		CVectorGraphics             m_oVector;

		std::vector<std::shared_ptr<CContText>>	 m_arConts;
		std::vector<std::shared_ptr<CTextLine>>  m_arTextLines;
		std::vector<std::shared_ptr<CContText>>  m_arDiacriticalSymbols;
		std::vector<std::shared_ptr<CShape>>     m_arImages;
		std::vector<std::shared_ptr<CShape>>     m_arShapes;

		std::vector<std::wstring>   m_arCompleteObjectsXml;

		std::vector<std::shared_ptr<COutputObject>>  m_arOutputObjects;

		CTextLine*               m_pCurrentLine {nullptr};

		bool m_bIsDeleteTextClipPage {true};
		bool m_bIsRecalcFontSize {true};
		LONG m_lLastCommand = 0;

		CPage(NSFonts::IApplicationFonts* pFonts);
		~CPage();

		void Init(NSStructures::CFont* pFont,
			NSStructures::CPen* pPen,
			NSStructures::CBrush* pBrush,
			NSStructures::CShadow* pShadow,
			NSStructures::CEdgeText* pEdge,
			Aggplus::CMatrix* pMatrix,
			Aggplus::CGraphicsPathSimpleConverter* pSimple,
			CFontStyleManager* pStyleManager,
			CFontManager *pFontManager,
			CFontSelector* pFontSelector,
			CParagraphStyleManager* pParagraphStyleManager);

		void BeginCommand(DWORD lType);
		void Clear();

		//удаляем то, что выходит за границы страницы
		void DeleteTextClipPage();

		// image commands
		//набивается содержимым вектор m_arImages
		void WriteImage(const std::shared_ptr<CImageInfo> pInfo, double& fX, double& fY, double& fWidth, double& fHeight);

		// path commands
		void MoveTo(double& dX, double& dY);
		void LineTo(double& dX, double& dY);
		void CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3);
		void PathStart();
		void PathEnd();
		void PathClose();

		//набивается содержимым вектор m_arShapes
		void DrawPath(LONG lType, const std::shared_ptr<CImageInfo> pInfo);

		//набивается содержимым вектор m_arTextData
		void CollectTextData(const PUINT pUnicodes,
			const PUINT pGids,
			const UINT& nCount,
			const double& fX,
			const double& fY,
			const double& fWidth,
			const double& fHeight,
			const double& fBaseLineOffset);

		void Analyze();
		void Record(NSStringUtils::CStringBuilder& oWriter, bool bIsLastPage);

	private:
		// methods to build text lines
		void BuildDiacriticalSymbols();
		void BuildTextLines();
		void AddContToTextLine(std::shared_ptr<CContText> pCont);

		void AnalyzeTextLines();
		void AnalyzeConts();
		void DetermineStrikeoutsUnderlinesHighlights();

		void AnalyzeDropCaps();
		void AddDiacriticalSymbols();
		void MergeLinesByVertAlignType();
		void DetermineTextColumns();

		bool IsLineCrossingText(const CShape* pGraphicItem, CContText* pCont, const eHorizontalCrossingType& eHType);
		bool IsLineBelowText(const CShape* pGraphicItem, CContText* pCont, const eHorizontalCrossingType& eHType);
		bool IsItHighlightingBackground(const CShape* pGraphicItem, CContText* pCont, const eHorizontalCrossingType& eHType);

		void AnalyzeShapes();
		void DetermineLinesType();

		void BuildLines();
		void DetermineDominantGraphics();

		void BuildParagraphes();

		std::shared_ptr<CShape> CreateSingleLineShape(std::shared_ptr<CTextLine>& pLine);
		std::shared_ptr<CShape> CreateSingleParagraphShape(std::shared_ptr<CParagraph>& pParagraph);

		void MergeShapes();
		void CalcSelected();

		// конвертим m_arImages, m_arShapes, m_arParagraphs в xml-строку
		void ToXml(NSStringUtils::CStringBuilder& oWriter);
		void WriteSectionToFile(bool bLastPage, NSStringUtils::CStringBuilder& oWriter);
	};
}
