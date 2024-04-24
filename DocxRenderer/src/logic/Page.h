/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

﻿#pragma once
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
