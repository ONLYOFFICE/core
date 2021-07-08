#include "TxBodyConverter.h"
#include "../../../Common/MS-LCID.h"
#include "../../../ASCOfficeXlsFile2/source/XlsXlsxConverter/ShapeType.h"
#include "StylesWriter.h"

using namespace PPT_FORMAT;

TxBodyConverter::TxBodyConverter(CShapeElement *pShapeElement, TxBodyConverter::eTxType txType) :
    m_pShapeElement(pShapeElement), oText(pShapeElement->m_pShape->m_oText), m_txType(txType)
{
}

void TxBodyConverter::FillTxBody(PPTX::Logic::TxBody &oTxBody)
{
    if (m_pShapeElement == nullptr)
        return;

    switch (m_txType)
    {
    case shape: ConvertShapeTxBody(oTxBody); break;
    case table: ConvertTableTxBody(oTxBody); break;
    }
}

void TxBodyConverter::ConvertTableTxBody(PPTX::Logic::TxBody &oTxBody)
{
    oTxBody.m_name = L"a:txBody";


    oTxBody.bodyPr = new PPTX::Logic::BodyPr;
    FillBodyPr(oTxBody.bodyPr.get2());


    size_t nCount = oText.m_arParagraphs.size();
    if (nCount == 0)
    {
        oTxBody.lstStyle = new PPTX::Logic::TextListStyle;
        PPTX::Logic::Paragraph paragraph;
        auto pEndParaRPr = new PPTX::Logic::RunProperties;
        pEndParaRPr->m_name = L"a:endParaRPr";
        pEndParaRPr->dirty = false;
        paragraph.endParaRPr = pEndParaRPr;
        oTxBody.Paragrs.push_back(paragraph);

        return;
    }

    oTxBody.lstStyle = new PPTX::Logic::TextListStyle;


//    if (m_pText)
//    {
//        FillLstStyles(oTxBody.lstStyle.get2(), m_pText->m_oStyles);
//        FillParagraphs(oTxBody.Paragrs, m_pText->m_arParagraphs);
//    }
}

void TxBodyConverter::ConvertShapeTxBody(PPTX::Logic::TxBody &oTxBody)
{

}

void TxBodyConverter::FillBodyPr(PPTX::Logic::BodyPr &oBodyPr)
{
    CDoubleRect oTextRect;
    m_pShapeElement->m_pShape->GetTextRect(oTextRect);

    oBodyPr.lIns = oTextRect.left;
    oBodyPr.rIns = oTextRect.right;
    oBodyPr.tIns = oTextRect.top;
    oBodyPr.bIns = oTextRect.bottom;


    auto pAnchor = new PPTX::Limit::TextAnchor;
    switch (oText.m_oAttributes.m_nTextAlignVertical)
    {
    case 0: pAnchor->set(L"t"); break;
    case 2: pAnchor->set(L"b"); break;
    default:
        pAnchor->set(L"ctr");
        oBodyPr.anchorCtr = false;
    }
    oBodyPr.anchor = pAnchor;


    if (m_pShapeElement->m_pShape->m_oText.m_bAutoFit)
        oBodyPr.Fit.type = PPTX::Logic::TextFit::FitSpAuto;


    if (oText.m_oAttributes.m_dTextRotate > 0)
        oBodyPr.rot = round(oText.m_oAttributes.m_dTextRotate * 60000);


    if (oText.m_nTextFlow >= 0)
    {
        auto *pVert = new PPTX::Limit::TextVerticalType;
        switch(oText.m_nTextFlow)
        {
        case 1:
        case 3: pVert->set(L"vert"); break;
        case 2: pVert->set(L"vert270"); break;
        case 5: pVert->set(L"wordArtVert");	break;
        }
        oBodyPr.vert = pVert;
    }
    else if (oText.m_bVertical)
    {
        auto *pVert = new PPTX::Limit::TextVerticalType;
        pVert->set(L"eaVert");
        oBodyPr.vert = pVert;
    }


    std::wstring prstGeom	= oox::Spt2ShapeType_mini((oox::MSOSPT)m_pShapeElement->m_lShapeType);
    std::wstring prstTxWarp = oox::Spt2WordArtShapeType((oox::MSOSPT)m_pShapeElement->m_lShapeType);
    if (prstGeom.size() && prstTxWarp.size())
    {
        auto pPrstTxWarp = new PPTX::Logic::PrstTxWarp;
        pPrstTxWarp->prst.set(prstTxWarp);


        CPPTShape *pPPTShape = dynamic_cast<CPPTShape *>(m_pShapeElement->m_pShape->getBaseShape().get());
        std::wstring strVal;

        for (size_t i = 0 ; (pPPTShape) && (i < pPPTShape->m_arAdjustments.size()); i++)
        {
            PPTX::Logic::Gd gd;
            switch(m_pShapeElement->m_lShapeType)
            {
                case oox::msosptTextFadeUp:
                {
                    double kf = 4.63; //"волшебный"
                    std::wstring strVal = std::to_wstring((int)(kf * pPPTShape->m_arAdjustments[i]));

                    gd.fmla = L"val";
                    gd.name = L"adj";
                    pPrstTxWarp->avLst.push_back(std::move(gd));
                }break;
            }

        }

        oBodyPr.prstTxWarp = pPrstTxWarp;
    }
}
