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

#include "../../../ASCOfficePPTXFile/Editor/Drawing/TextAttributesEx.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/TxBody.h"

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
    TxBodyConverter(CTextAttributesEx* pText, eTxType txType);

    void FillTxBody(PPTX::Logic::TxBody& oTxBody);
private:
    void ConvertTableTxBody(PPTX::Logic::TxBody& oTxBody);
    void ConvertShapeTxBody(PPTX::Logic::TxBody& oTxBody);

private:
    void FillParagraphs(std::vector<PPTX::Logic::Paragraph>& arrP, std::vector<CParagraph>& arrParagraphs);
    void FillParagraph(PPTX::Logic::Paragraph& p, CParagraph& paragraph);
    void FillRun(PPTX::Logic::Run& oRun, CSpan &oSpan);
    void FillEndParaRPr(PPTX::Logic::RunProperties& oEndPr, CTextPFRun& oPFRun);
    void FillPPr(PPTX::Logic::TextParagraphPr& oPPr, CTextPFRun& oPFRun);
    void FillRPr(PPTX::Logic::RunProperties& oRPr, CTextCFRun& oCFRun);
    void FillLatin(PPTX::Logic::TextFont& oLatin, CFontProperty &font);
    void FillCS(PPTX::Logic::TextFont& oCs, CFontProperties& font);
    void FillEffectLst(PPTX::Logic::EffectProperties &oEList, CTextCFRun& oCFRun);
    void FillSolidFill(PPTX::Logic::UniFill& oUF, CColor& oColor);
private:
    CTextAttributesEx* m_pText;
    eTxType m_txType;
};
}
