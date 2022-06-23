#ifndef DOCX_PAGE_H
#define DOCX_PAGE_H

#include "ElementOldShape.h"
#include "ElementParagraph.h"
#include "ElementImage.h"
#include "ElementShape.h"

namespace NSDocxRenderer
{
    inline bool IsSpaceUtf32(const uint32_t& c)
    {
        return (' ' == c) ? true : false;
    }
    inline bool IsSpaceUtf32(const NSStringUtils::CStringUTF32& oText)
    {
        if (1 != oText.length())
            return false;
        return IsSpaceUtf32(oText.ToStdWString()[0]);
    }

    inline bool IsUnicodeSymbol( int symbol )
    {
        bool result = false;

        if ( ( 0x0009 == symbol ) || ( 0x000A == symbol ) || ( 0x000D == symbol ) ||
           ( ( 0x0020 <= symbol ) && ( 0xD7FF >= symbol ) ) || ( ( 0xE000 <= symbol ) && ( symbol <= 0xFFFD ) ) ||
           ( ( 0x10000 <= symbol ) && symbol ) )
        {
            result = true;
        }

        return result;
    }

    class CPage
    {
    public:
        NSStructures::CFont*		m_pFont;
        NSStructures::CPen*			m_pPen;
        NSStructures::CBrush*		m_pBrush;
        NSStructures::CShadow*		m_pShadow;
        NSStructures::CEdgeText*	m_pEdgeText;

        Aggplus::CMatrix*			m_pTransform;
        Aggplus::CGraphicsPathSimpleConverter* m_pSimpleGraphicsConverter;

        CVectorGraphics				m_oVector;

        double m_dWidth;
        double m_dHeight;

        LONG						m_lCurrentCommand;

        std::vector<CContText*> m_arTextData;

        std::vector<CBaseItem*>	m_arGraphicItems;
        std::vector<CParagraph*> m_arParagraphs;

        std::vector<CTextLine*> m_arTextLine;
        CTextLine* m_pCurrentLine;

        CFontManager		m_oManager;
        CFontManagerLight	m_oManagerLight;

        TextAssociationType m_eTextAssociationType;

        bool m_bIsDeleteTextClipPage;

        double m_dLastTextX;
        double m_dLastTextY;
        double m_dLastTextX_block;

    public:
        CPage(NSFonts::IApplicationFonts* pFonts);
        ~CPage();
        void Init(NSStructures::CFont* pFont, NSStructures::CPen* pPen, NSStructures::CBrush* pBrush,
            NSStructures::CShadow* pShadow, NSStructures::CEdgeText* pEdge, Aggplus::CMatrix* pMatrix, Aggplus::CGraphicsPathSimpleConverter* pSimple);

        void Clear();
        void ClearTextData();
        void ClearTextLines();
        void ClearGraphicItems();
        void ClearParagraphs();

        void SetCurrentLineByBaseline(const double& dBaseLinePos);
        //удаляем то, что выходит за границы страницы
        void DeleteTextClipPage();

        // image commands
        //набивается содержимым вектор m_arGraphicItems
        void WriteImage(CImageInfo& oInfo, double& fX, double& fY, double& fWidth, double& fHeight);

        // path commands
        void MoveTo(double& dX, double& dY);
        void LineTo(double& dX, double& dY);
        void CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3);
        void Start();
        void End();
        void Close();
        //набивается содержимым вектор m_arGraphicItems
        void DrawPath(LONG lType, LONG lTxId);

        //набивается содержимым вектор m_arTextData
        void CollectTextData(const PUINT pUnicodes, const PUINT pGids, const UINT& nCount,
                             const double& fX, const double& fY, const double& fWidth, const double& fHeight,
                             const double& fBaseLineOffset, const bool& bIsPDFAnalyzer);

        void AnalyzeCollectedGraphics();

        //Собранные для текущей страницы данные нужно проанализировать и сгруппировать, лишнее удалить
        void AnalyzeCollectedData();

        //набивается содержимым вектор m_arTextLine
        void BuildLines();
        void BuildLines(const CContText* pContText);

        void BuildByType();
        void BuildByTypeBlockChar();
        void BuildByTypeBlockLine();
        void BuildByTypePlainLine();
        void BuildByTypeShapeLine();
        void BuildByTypePlainParagraph();

        //Объединяем строки, которые находятся на расстроянии не большем dAffinity
        void Merge(double dAffinity);

        //конвертим m_arGraphicItems, m_arParagraphs в xml-строку
        void Write(NSStringUtils::CStringBuilder& oWriter);

        void WriteSectionToFile(bool bLastPage, NSStringUtils::CStringBuilder& oWriter);

        double RightBorderCorrection(const CTextLine *pLine);

        void CreateSingleLineParagraph(CTextLine *pLine, const double *pRight, const double *pBeforeSpacing);
        void CreateSingleLineOldShape(CTextLine *pLine);
        void CreateSingleLineShape(CTextLine *pLine);

        bool IsLineCrossingText(const CShape* pGraphicItem, CContText* pContText);
        bool IsLineBelowText(const CShape* pGraphicItem, CContText* pContText);
        bool IsItHighlightingBackground(const CShape* pGraphicItem, CContText* pContText);
    };
}

#endif // DOCX_PAGE_H
