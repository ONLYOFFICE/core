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
#include "TxBodyConverter.h"
#include "../../../Common/MS-LCID.h"
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
