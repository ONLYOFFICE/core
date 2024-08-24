#pragma once
#include "../../../DesktopEditor/graphics/GraphicsPath.h"

#include "elements/Paragraph.h"
#include "elements/Shape.h"
#include "managers/FontStyleManager.h"
#include "managers/ParagraphStyleManager.h"
#include "convert_params.h"

namespace NSDocxRenderer
{
	class CPage
	{
	public:
		double m_dWidth {0.0};
		double m_dHeight {0.0};

		LONG m_lCurrentCommand{0};
		LONG m_lClipMode{0};

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

		CVectorGraphics             m_oCurrVectorGraphics;
		CVectorGraphics				m_oClipVectorGraphics;

		std::vector<std::shared_ptr<CContText>>	 m_arConts;
		std::vector<std::shared_ptr<CTextLine>>  m_arTextLines;
		std::vector<std::shared_ptr<CContText>>  m_arDiacriticalSymbols;
		std::vector<std::shared_ptr<CShape>>     m_arShapes;

		std::vector<std::wstring>   m_arCompleteObjectsXml;
		std::vector<std::shared_ptr<CBaseItem>>  m_arOutputObjects;

		bool m_bIsDeleteTextClipPage {true};
		bool m_bIsRecalcFontSize {true};

		CPage();
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
		void EndCommand(DWORD lType);
		void Clear();

		// удаляем то, что выходит за границы страницы
		void DeleteTextClipPage();

		// image commands
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

		std::shared_ptr<CContText> m_pCurrCont {nullptr};
		NSStructures::CFont m_oPrevFont;
		NSStructures::CBrush m_oPrevBrush;

		size_t m_nShapeOrder = 0;
		bool m_bIsClipPath {false};
	};
}
