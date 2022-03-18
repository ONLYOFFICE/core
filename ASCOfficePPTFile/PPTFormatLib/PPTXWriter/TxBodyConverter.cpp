#include "TxBodyConverter.h"
#include "../../../Common/MS-LCID.h"
#include "../../../ASCOfficeXlsFile2/source/XlsXlsxConverter/ShapeType.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SchemeClr.h"
#include "StylesWriter.h"

using namespace PPT_FORMAT;

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
