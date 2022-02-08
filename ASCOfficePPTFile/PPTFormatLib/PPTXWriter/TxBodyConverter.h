/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#pragma once

#include "../../../ASCOfficePPTXFile/Editor/Drawing/Elements.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/TxBody.h"
#include "ImageManager.h"

namespace PPT_FORMAT
{
class TxBodyConverter
{
public:
    enum eTxType
    {
        shape,
        table
    };
    TxBodyConverter(CShapeElement* pShapeElement, CRelsGenerator* pRels,  eTxType txType);

    void FillTxBody(PPTX::Logic::TxBody& oTxBody);
private:
    void ConvertTableTxBody(PPTX::Logic::TxBody& oTxBody);
    void ConvertShapeTxBody(PPTX::Logic::TxBody& oTxBody);
    void FillMergedTxBody(PPTX::Logic::TxBody& oTxBody);

public:
    static void FillPPr(PPTX::Logic::TextParagraphPr& oPPr, CParagraph &paragraph, CRelsGenerator *pRels);
    static void ConvertPFRun(PPTX::Logic::TextParagraphPr &oPPr, PPT_FORMAT::CTextPFRun* pPF, CRelsGenerator *pRels);

private:
    void FillBodyPr(PPTX::Logic::BodyPr& oBodyPr);
    void FillLstStyles(PPTX::Logic::TextListStyle& oTLS, CTextStyles& oStyles);
    void FillParagraph(PPTX::Logic::Paragraph& p, CParagraph& paragraph);
    static void ConvertTabStops(std::vector<PPTX::Logic::Tab>& arrTabs, std::vector<std::pair<int, int>>& arrTabStops);
    static void FillBuChar(PPTX::Logic::Bullet& oBullet, WCHAR symbol);
    static void ConvertAllBullets(PPTX::Logic::TextParagraphPr &oPPr, CTextPFRun *pPF, CRelsGenerator *pRels);
    static void FillBuClr(PPTX::Logic::BulletColor& oBuClr, CColor& oColor);
    void FillRun(PPTX::Logic::Run& oRun, CSpan &oSpan);
    PPTX::Logic::RunProperties* getNewEndParaRPr(const int dirty = -1, const int sz = -1, const std::wstring& lang = L"");
    void FillRPr(PPTX::Logic::RunProperties& oRPr, CTextCFRun& oCFRun);
    void ConvertLine(PPTX::Logic::Ln& oLn);
    void ConvertLineEnd(PPTX::Logic::LineEnd& oLine, unsigned char cap, unsigned char length, unsigned char width, bool isHead);
    void ConvertBrush(PPTX::Logic::RunProperties& oRPr);
    void FillSolidFill(PPTX::Logic::UniFill& oUF, CColor& oColor);
    void FillSchemeClr(PPTX::Logic::UniFill& oUF, CColor& oColor);
    void FillEffectLst(PPTX::Logic::EffectProperties &oEList, CTextCFRun& oCFRun);



    void FillParagraphs(std::vector<PPTX::Logic::Paragraph>& arrP, std::vector<CParagraph>& arrParagraphs);
    void ConvertStyleLevel(PPTX::Logic::TextParagraphPr &oLevel, CTextStyleLevel& oOldLevel, const int& nLevel);
    void FillEndParaRPr(PPTX::Logic::RunProperties& oEndPr, CTextPFRun& oPFRun);
    void FillCS(PPTX::Logic::TextFont& oCs, CFontProperties& font);

private:
    CShapeElement* m_pShapeElement;
    CRelsGenerator* m_pRels;
    CTextAttributesEx* pText;

    eTxType m_txType;
    bool m_bWordArt;
};
}
