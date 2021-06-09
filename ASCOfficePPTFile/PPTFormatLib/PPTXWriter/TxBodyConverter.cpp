#include "TxBodyConverter.h"

using namespace PPT_FORMAT;

TxBodyConverter::TxBodyConverter(CTextAttributesEx *pText, TxBodyConverter::eTxType txType) :
    m_pText(pText), m_txType(txType)
{

}

void TxBodyConverter::FillTxBody(PPTX::Logic::TxBody &oTxBody)
{
    if (m_pText == nullptr)
        return;
    switch (m_txType)
    {
    case shape: ConvertShapeTxBody(oTxBody); break;
    case table: ConvertTableTxBody(oTxBody); break;
    }

}

void TxBodyConverter::ConvertTableTxBody(PPTX::Logic::TxBody &oTxBody)
{
    oTxBody.bodyPr = new PPTX::Logic::BodyPr;
    oTxBody.lstStyle = new PPTX::Logic::TextListStyle;
//    if (m_pShape == nullptr)
//    {
        PPTX::Logic::Paragraph p;
        p.endParaRPr = new PPTX::Logic::RunProperties;
        p.endParaRPr->m_name = L"a:endParaRPr";
        p.endParaRPr->lang = L"en-US";

        oTxBody.Paragrs.push_back(p);
//    }
}

void TxBodyConverter::ConvertShapeTxBody(PPTX::Logic::TxBody &oTxBody)
{

}
