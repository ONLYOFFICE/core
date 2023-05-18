#pragma once
#include "../DesktopEditor/graphics/pro/Graphics.h"
#include "elements/Paragraph.h"
#include "elements/Shape.h"
#include "elements/Table.h"
#include "managers/FontStyleManager.h"
#include "managers/ParagraphStyleManager.h"
#include "styles/ParagraphStyle.h"


namespace NSDocxRenderer
{
	class CPage
	{
	public:
		NSStructures::CFont*        m_pFont {nullptr};
		NSStructures::CPen*         m_pPen {nullptr};
		NSStructures::CBrush*       m_pBrush {nullptr};
		NSStructures::CShadow*      m_pShadow {nullptr};
		NSStructures::CEdgeText*    m_pEdgeText {nullptr};

		Aggplus::CMatrix*           m_pTransform {nullptr};
		Aggplus::CGraphicsPathSimpleConverter* m_pSimpleGraphicsConverter {nullptr};

		CFontStyleManager*          m_pFontStyleManager {nullptr};
		CParagraphStyleManager*     m_pParagraphStyleManager {nullptr};
		CFontManager*               m_pFontManager {nullptr};
		CFontSelector*				m_pFontSelector {nullptr};
		CVectorGraphics             m_oVector;

		double m_dWidth {0.0};
		double m_dHeight {0.0};

		LONG                     m_lCurrentCommand {0};

		std::vector<CShape*>     m_arImages;
		std::vector<CContText*>  m_arDiacriticalSymbol;
		std::vector<CTextLine*>  m_arTextLine;
		std::vector<CShape*>     m_arShapes;

		std::vector<CBaseItem*>  m_arOutputObjects;

		std::vector<CPeak*>      m_arPeaks;
		std::vector<CCell*>      m_arCells;
		std::vector<CRow*>       m_arRows;
		std::vector<CTable*>     m_arTables;

		CTextLine*               m_pCurrentLine {nullptr};
		CRow*                    m_pCurrentRow {nullptr};


		eTextAssociationType     m_eTextAssociationType {eTextAssociationType::tatPlainParagraph};

		bool m_bIsDeleteTextClipPage {true};
		bool m_bIsRecalcFontSize {true};
		LONG m_lLastCommand = 0;

	public:
		CPage(NSFonts::IApplicationFonts* pFonts);
		~CPage();
		void Init(NSStructures::CFont* pFont, NSStructures::CPen* pPen, NSStructures::CBrush* pBrush,
				  NSStructures::CShadow* pShadow, NSStructures::CEdgeText* pEdge, Aggplus::CMatrix* pMatrix,
				  Aggplus::CGraphicsPathSimpleConverter* pSimple, CFontStyleManager* pStyleManager, CFontManager *pFontManager,
				  CFontSelector* pFontSelector, CParagraphStyleManager* pParagraphStyleManager);


		void BeginCommand(DWORD lType);

		void Clear();
		void ClearTables();

		//удаляем то, что выходит за границы страницы
		void DeleteTextClipPage();

		// image commands
		//набивается содержимым вектор m_arImages
		void WriteImage(const std::shared_ptr<CImageInfo> pInfo, double& fX, double& fY, double& fWidth, double& fHeight);

		// path commands
		void MoveTo(double& dX, double& dY);
		void LineTo(double& dX, double& dY);
		void CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3);
		void Start();
		void End();
		void Close();

		//набивается содержимым вектор m_arShapes
		void DrawPath(LONG lType, const std::shared_ptr<CImageInfo> pInfo);

		//набивается содержимым вектор m_arTextData
		void CollectTextData(const PUINT pUnicodes, const PUINT pGids, const UINT& nCount,
							 const double& fX, const double& fY, const double& fWidth, const double& fHeight,
							 const double& fBaseLineOffset, const bool& bIsPDFAnalyzer);
		void AddContToTextLine(CContText *pCont);

		void ProcessingAndRecordingOfPageData(NSStringUtils::CStringBuilder& oWriter, LONG lPagesCount, LONG lNumberPages);

		void AnalyzeCollectedShapes();
		void BuildTables();
		void CollectPeaks();
		void CreatCells();
		void BuildRows();
		void SelectCurrentRow(const CCell *pCell);
		void DetermineLinesType();

		//Собранные для текущей страницы данные нужно проанализировать и сгруппировать, лишнее удалить
		void AnalyzeCollectedTextLines();
		void AnalyzeCollectedConts();
		void DetermineStrikeoutsUnderlinesHighlights();
		bool IsLineCrossingText(const CShape* pGraphicItem, CContText* pCont, const eHorizontalCrossingType& eHType);
		bool IsLineBelowText(const CShape* pGraphicItem, CContText* pCont, const eHorizontalCrossingType& eHType);
		bool IsItHighlightingBackground(const CShape* pGraphicItem, CContText* pCont, const eHorizontalCrossingType& eHType);
		void AddDiacriticalSymbols();
		void MergeLinesByVertAlignType();
		void DetermineTextColumns();
		void DetermineDominantGraphics();

		//конвертим m_arImages, m_arShapes, m_arParagraphs в xml-строку
		void ToXml(NSStringUtils::CStringBuilder& oWriter);

		void WriteSectionToFile(bool bLastPage, NSStringUtils::CStringBuilder& oWriter);
	};
}
