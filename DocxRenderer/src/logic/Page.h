#pragma once
#include "../DesktopEditor/graphics/pro/Graphics.h"
#include "elements/OldShape.h"
#include "elements/Paragraph.h"
#include "elements/Shape.h"
#include "managers/StyleManager.h"

namespace NSDocxRenderer
{
    class CPage
    {
    public:
        NSStructures::CFont*		m_pFont {nullptr};
        NSStructures::CPen*			m_pPen {nullptr};
        NSStructures::CBrush*		m_pBrush {nullptr};
        NSStructures::CShadow*		m_pShadow {nullptr};
        NSStructures::CEdgeText*	m_pEdgeText {nullptr};

        Aggplus::CMatrix*			m_pTransform {nullptr};
        Aggplus::CGraphicsPathSimpleConverter* m_pSimpleGraphicsConverter {nullptr};

        CStyleManager*              m_pStyleManager {nullptr};

        CVectorGraphics				m_oVector;

        double m_dWidth {0.0};
        double m_dHeight {0.0};

        LONG						m_lCurrentCommand {0};

        std::vector<CShape*>     m_arImages;
        std::vector<CContText*>  m_arSymbol ;
        std::vector<CTextLine*>  m_arTextLine;
        std::vector<CShape*>	 m_arShapes;
        std::vector<CParagraph*> m_arParagraphs;

        CTextLine* m_pCurrentLine {nullptr};

        CFontManager		m_oFontManager;
        CFontManagerLight	m_oFontManagerLight;

        TextAssociationType m_eTextAssociationType {tatPlainLine};

        bool m_bIsDeleteTextClipPage {true};

        double m_dLastTextX {-1};
        double m_dLastTextY {-1};
        double m_dLastTextX_block {-1};

    public:
        CPage(NSFonts::IApplicationFonts* pFonts);
        ~CPage();
        void Init(NSStructures::CFont* pFont, NSStructures::CPen* pPen, NSStructures::CBrush* pBrush,
            NSStructures::CShadow* pShadow, NSStructures::CEdgeText* pEdge, Aggplus::CMatrix* pMatrix,
            Aggplus::CGraphicsPathSimpleConverter* pSimple, CStyleManager* pStyleManager);

        void Clear();
        void ClearImages();
        void ClearTextData();
        void ClearTextLines();
        void ClearShapes();
        void ClearParagraphs();

        void SelectCurrentLine(const CContText* pCont);
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

        void AnalyzeCollectedShapes();
        void DetermineLinesType();

        //Собранные для текущей страницы данные нужно проанализировать и сгруппировать, лишнее удалить
        void AnalyzeCollectedSymbols();
        void DetermineStrikeoutsUnderlinesHighlights();
        bool IsLineCrossingText(const CShape* pGraphicItem, CContText* pCont, const eHorizontalCrossingType& eHType);
        bool IsLineBelowText(const CShape* pGraphicItem, CContText* pCont, const eHorizontalCrossingType& eHType);
        bool IsItHighlightingBackground(CShape* pGraphicItem, CContText* pCont, const eHorizontalCrossingType& eHType);

        //набивается содержимым вектор m_arTextLine
        void AnalyzeLines();
        void BuildLines();
        void CollectDublicateLines(const CContText *pCont);
        void MergeLinesByVertAlignType();
        void DetermineDominantGraphics();

        void BuildByType();
        void BuildByTypeBlockChar();
        void BuildByTypeBlockLine();
        void BuildByTypePlainLine();
        void BuildByTypeShapeLine();
        void BuildByTypePlainParagraph();

        //Объединяем строки, которые находятся на расстроянии не большем dAffinity
        void Merge(double dAffinity);

        //конвертим m_arImages, m_arShapes, m_arParagraphs в xml-строку
        void ToXml(NSStringUtils::CStringBuilder& oWriter);

        void WriteSectionToFile(bool bLastPage, NSStringUtils::CStringBuilder& oWriter);

        void CreateSingleLineParagraph(CTextLine *pLine, const double *pRight, const double *pBeforeSpacing);
        void CreateSingleLineOldShape(CTextLine *pLine);
        void CreateSingleLineShape(CTextLine *pLine);

        bool IsShadingPresent(const CTextLine* pLine1, const CTextLine* pLine2);

    private:
        CTextLine* GetNextTextLine(size_t& nCurrentIndex, size_t* pIndexForCheking = nullptr);
    };
}
