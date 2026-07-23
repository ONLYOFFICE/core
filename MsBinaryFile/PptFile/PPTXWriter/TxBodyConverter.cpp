/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "TxBodyConverter.h"
#include "../../XlsFile/Converter/ShapeType.h"
#include "../../../OOXML/PPTXFormat/Logic/Colors/SchemeClr.h"
#include "StylesWriter.h"

using namespace PPT;

TxBodyConverter::TxBodyConverter(CElementPtr pShapeElement, CRelsGenerator* pRels, CTextCFRun *pLastCF) :
    m_bError(false), m_pLastCF(pLastCF)
{
    m_bError = !m_oShapeWriter.SetElement(pShapeElement);

    if (!m_bError)
    {
        m_oShapeWriter.SetRelsGenerator(pRels);
    }

}

void TxBodyConverter::FillTxBody(PPTX::Logic::TxBody &oTxBody)
{
    if (m_bError)
        FillMergedTxBody(oTxBody);
    else
        ConvertTableTxBody(oTxBody);
}

void TxBodyConverter::ConvertTableTxBody(PPTX::Logic::TxBody &oTxBody)
{
    m_oShapeWriter.WriteTextInfo(m_pLastCF);
    XmlUtils::CXmlNode xmlReader;
    xmlReader.FromXmlString(m_oShapeWriter.getOWriterStr());
    oTxBody.fromXML(xmlReader);
    oTxBody.m_name = L"a:txBody";
}

void TxBodyConverter::FillMergedTxBody(PPTX::Logic::TxBody &oTxBody)
{
    oTxBody.lstStyle = new PPTX::Logic::TextListStyle;
    oTxBody.bodyPr = new PPTX::Logic::BodyPr;
    oTxBody.lstStyle = new PPTX::Logic::TextListStyle;

    PPTX::Logic::Paragraph paragraph;
    paragraph.endParaRPr = getNewEndParaRPr();
    oTxBody.Paragrs.push_back(paragraph);
}

PPTX::Logic::RunProperties *TxBodyConverter::getNewEndParaRPr(const int dirty, const int sz , const std::wstring& lang)
{
    auto pEndParaRPr = new PPTX::Logic::RunProperties;
    pEndParaRPr->m_name = L"a:endParaRPr";

    if (dirty == 0 || dirty == 1)
        pEndParaRPr->dirty = (bool)dirty;
    if (sz > 0)
        pEndParaRPr->sz = sz;
    if (lang.size())
        pEndParaRPr->lang = lang;

    return pEndParaRPr;
}
