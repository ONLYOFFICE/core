﻿#include "TableWriter.h"
#include "TxBodyConverter.h"
#include <iostream>

TableWriter::TableWriter(CTableElement *pTableElement, CRelsGenerator* pRels) :
    m_pTableElement(pTableElement), m_pRels(pRels)
{

}

void TableWriter::Convert(PPTX::Logic::GraphicFrame &oGraphicFrame)
{
    if (!m_pTableElement)
        return;

    if (/*m_pTableElement->m_xmlRawData.empty() || */true)
    {
        oGraphicFrame.nvGraphicFramePr = new PPTX::Logic::NvGraphicFramePr;
        FillNvGraphicFramePr(oGraphicFrame.nvGraphicFramePr.get2());

        oGraphicFrame.xfrm = new PPTX::Logic::Xfrm;
        FillXfrm(oGraphicFrame.xfrm.get2());

        oGraphicFrame.table = new PPTX::Logic::Table;
        FillTable(oGraphicFrame.table.get2());
    }
}

void TableWriter::FillNvGraphicFramePr(PPTX::Logic::NvGraphicFramePr& oNvGFPr)
{
    oNvGFPr.cNvPr.id = m_pTableElement->m_lID;
    oNvGFPr.cNvPr.name = m_pTableElement->m_sName;

    // not works in full
    oNvGFPr.cNvGraphicFramePr.noGrp = true;

    PPTX::Logic::Ext ext;
    ext.uri = L"{5C22544A-7EE6-4342-B048-85BDC9FD1C3A}";
    oNvGFPr.nvPr.extLst.push_back(ext);
}

void TableWriter::FillXfrm(PPTX::Logic::Xfrm &oXFRM)
{
    oXFRM.node_name = L"p:xfrm";
    double multip1 = m_pTableElement->m_bAnchorEnabled ? 1587.5 : 1;

    oXFRM.offX = round(m_pTableElement->m_rcAnchor.left * multip1);
    oXFRM.offY = round(m_pTableElement->m_rcAnchor.top  * multip1);

    oXFRM.extX = round(m_pTableElement->m_rcAnchor.GetWidth()  * multip1);
    oXFRM.extY = round(m_pTableElement->m_rcAnchor.GetHeight() * multip1);
}

void TableWriter::FillTable(PPTX::Logic::Table &oTable)
{
    oTable.tableProperties = new PPTX::Logic::TableProperties;
    FillTblPr(oTable.tableProperties.get2());

    std::vector<CShapeElement*> arrCells, arrSpliters;
    prepareShapes(arrCells, arrSpliters);
    m_nPTable = new ProtoTable(arrCells, arrSpliters, m_pRels);
    FillTblGrid(oTable.TableCols , arrCells);


    auto protoTable = m_nPTable->getTable();
    auto arrHeight = ProtoTable::getHeight(arrCells);
    for (UINT cRow = 0; cRow < protoTable.size(); cRow++)
    {
        PPTX::Logic::TableRow tr;
        FillRow(tr, protoTable[cRow]);
        tr.Height = ProtoTable::checkRowForZeroHeight(protoTable[cRow]) ? 0 : arrHeight[cRow];
        oTable.TableRows.push_back(tr);
    }
}

std::vector<int> ProtoTable::getWidth(std::vector<CShapeElement*>& arrCells, bool isWidth)
{
    std::map<double, double> mapLeftWidth;
    for (const auto* pCell : arrCells)
    {
        if (!pCell) continue;
        double left = pCell->m_rcChildAnchor.left;
        double width = pCell->m_rcChildAnchor.right - left;

        // Here we search uniq left to answer: is it new colum?
        auto iter = mapLeftWidth.find(left);
        if (iter == mapLeftWidth.end())
        {
            mapLeftWidth.insert(std::make_pair(left, width));
        }
        // Here we check qestion: was it merged cell and now is it true width of colum?
        else if (iter->second > width)
        {
            mapLeftWidth.erase(iter);
            mapLeftWidth.insert(std::make_pair(left, width));
        }
    }

    std::vector<int> gridWidth;
    double multip = isWidth ? 1587.5 : 1.0;
    for (const auto& w : mapLeftWidth)
    {
        double value = isWidth ? w.second : w.first;
        gridWidth.push_back(round(value * multip));
    }

    return gridWidth;
}

std::vector<int> ProtoTable::getHeight(std::vector<CShapeElement*>& arrCells, bool isHeight)
{
    std::map<double, double> mapTopHeight;
    for (auto* pCell : arrCells)
    {
        if (!pCell) continue;
        double top = pCell->m_rcChildAnchor.top;
        double height = pCell->m_rcChildAnchor.bottom - top;

        // Here we search uniq top to answer: is it new row?
        auto iter = mapTopHeight.find(top);
        if (iter == mapTopHeight.end())
        {
            mapTopHeight.insert(std::make_pair(top, height));
        }
        // Here we check qestion: was it merged cell and now is it true height of row?
        else if (iter->second > height)
        {
            mapTopHeight.erase(iter);
            mapTopHeight.insert(std::make_pair(top, height));
        }

    }

    std::vector<int> gridHeight;
    double multip = isHeight ? 1587.5 : 1.0;
    for (const auto& h : mapTopHeight)
    {
        double value = isHeight ? h.second : h.first;
        gridHeight.push_back(round(value * multip));
    }

    return gridHeight;
}

bool ProtoTable::checkRowForZeroHeight(const ProtoTableRow &oRow)
{
    for (const auto& cell : oRow)
    {
        if (cell.isRealCell())
            return false;
    }

    return true;
}

void ProtoTable::initProtoTable()
{
    const UINT countRow = m_arrTop.size();
    const UINT countCol = m_arrLeft.size();

    m_table.clear();
    for (UINT cRow = 0; cRow < countRow; cRow++)
    {
        ProtoTableRow protoRow;
        for (UINT cCol = 0; cCol < countCol; cCol++)
            protoRow.push_back(TCell(nullptr, cRow, cCol, m_pRels, nullptr));

        m_table.push_back(protoRow);
    }
}

bool ProtoTable::fillProtoTable(std::vector<CShapeElement *> &arrCells,
                                std::vector<CShapeElement*>& arrSpliters)
{
    if (m_table.empty() || m_arrTop.empty() || m_arrLeft.empty())
        return false;

    bool wasCellsFilled  = fillCells(arrCells);
    fillBorders(arrSpliters);

    return wasCellsFilled;
}

bool ProtoTable::fillCells(std::vector<CShapeElement *> &arrCells)
{
    const UINT countRow = m_arrTop.size();
    const UINT countCol = m_arrLeft.size();

    for (auto* pCell : arrCells)
    {
        int left = pCell->m_rcChildAnchor.left;
        int top = pCell->m_rcChildAnchor.top;
        int right = pCell->m_rcChildAnchor.right;
        int bottom = pCell->m_rcChildAnchor.bottom;

        UINT posRow = 0, posCol = 0;
        for (; posRow < countRow; posRow++)
            if (top == m_arrTop[posRow]) break;
        for (; posCol < countCol; posCol++)
            if (left == m_arrLeft[posCol]) break;
        TCell* pParent = &m_table[posRow][posCol];

        UINT posRightCol = 0, posBottomRow = 0;
        for (; posBottomRow < countRow; posBottomRow++)
            if (bottom == m_arrTop[posBottomRow]) break;
        for (; posRightCol < countCol; posRightCol++)
            if (right == m_arrLeft[posRightCol]) break;

        for (UINT cRow = posRow; cRow < posBottomRow; cRow++)
            for (UINT cCol = posCol; cCol < posRightCol; cCol++)
            {
                auto& tCell = m_table[cRow][cCol];
                tCell.setPParent(pParent);

                if (posCol == cCol)
                    tCell.setGridSpan(posRightCol - cCol);

                if (posRow == cRow)
                    tCell.setRowSpan(posBottomRow - cRow);
            }
        pParent->setPParent(nullptr);
        pParent->setPShape(pCell);

        //        pParent->setGridSpan(posRightCol - posCol);
        //        pParent->setRowSpan(posBottomRow - posRow);
    }

    return true;
}

void ProtoTable::fillBorders(std::vector<CShapeElement *> &arrSpliters)
{
    for (auto* pBorder : arrSpliters)
    {
        int left = pBorder->m_rcChildAnchor.left;
        int top = pBorder->m_rcChildAnchor.top;
        int right = pBorder->m_rcChildAnchor.right;
        int bottom = pBorder->m_rcChildAnchor.bottom;

        UINT posFirstTop, posFirstLeft;
        UINT posLastTop, posLastLeft;

        findCellPos(top, left, posFirstTop, posFirstLeft);
        findCellPos(bottom, right, posLastTop, posLastLeft);
        if (!isDefaultBoard(pBorder))
        {
            setBorders(posFirstTop, posFirstLeft, posLastTop, posLastLeft, pBorder);
        }
    }
}

bool ProtoTable::findCellPos(const int top, const int left, UINT &posRow, UINT &posCol)
{
    const UINT countRow = m_arrTop.size();
    const UINT countCol = m_arrLeft.size();

    for (posRow = 0; posRow < countRow; posRow++)
        if (top == m_arrTop[posRow]) break;
    for (posCol = 0; posCol < countCol; posCol++)
        if (left == m_arrLeft[posCol]) break;

    return !(posRow == countRow || posCol == countCol);
}

void ProtoTable::setBorders(const UINT posFRow, const UINT posFCol, const UINT posLRow, const UINT posLCol, CShapeElement* pBorder)
{

    const UINT countRow = m_arrTop.size();
    const UINT countCol = m_arrLeft.size();

    // lnTlToBr or lnBlToTr
    if ((posFRow != posLRow) && (posFCol != posLCol))
    {
        auto borPos = posFRow > posLRow ? TCell::lnBlToTr : TCell::lnTlToBr;
        m_table[posFRow][posFCol].setBorder(borPos, pBorder);
        return;
    }

    // lnT or(and) lnB
    if (posFCol != posLCol)
    {
        for (UINT i = posFCol; i < posLCol; i++)
        {
            if (posFRow < countRow)
                m_table[posFRow][i].setBorder(TCell::lnT, pBorder);
            if (posFRow > 0)
                m_table[posFRow-1][i].setBorder(TCell::lnB, pBorder);
        }

        return;
    }

    // lnL or(and) lnR
    if (posFRow != posLRow)
    {
        for (UINT i = posFRow; i < posLRow; i++)
        {
            if (posFCol < countCol)
                m_table[i][posFCol].setBorder(TCell::lnL, pBorder);
            if (posFCol > 0)
                m_table[i][posFCol-1].setBorder(TCell::lnR, pBorder);
        }

        return;
    }
}

bool ProtoTable::isDefaultBoard(const CShapeElement *pBorder)
{
    if (pBorder == nullptr)
        return false;

    auto& color = pBorder->m_oPen.Color;

    return color.m_lSchemeIndex == 13;
}

std::vector<std::vector<CShapeElement*> > ProtoTable::getRows(std::vector<CShapeElement *> &arrCells)
{
    std::vector<std::vector<CShapeElement*> > arrRows;
    int rowTop = -1;
    for (auto* pCell : arrCells)
    {
        if (!pCell) continue;

        int top = int(pCell->m_rcChildAnchor.top);
        if (top != rowTop)
        {
            rowTop = top;
            arrRows.push_back(std::vector<CShapeElement*>());
        }
        arrRows.back().push_back(pCell);
    }

    return arrRows;
}

MProtoTable ProtoTable::getTable() const
{
    return m_table;
}

void TableWriter::FillTblPr(PPTX::Logic::TableProperties &oTblPr)
{
    //    oTblPr.TableStyleId = L"{5C22544A-7EE6-4342-B048-85BDC9FD1C3A}";
    //    oTblPr.FirstRow = true;
    //    oTblPr.BandRow = true;
}

void TableWriter::FillTblGrid(std::vector<PPTX::Logic::TableCol> &tblGrid, std::vector<CShapeElement*>& arrCells)
{
    auto grid = ProtoTable::getWidth(arrCells);
    for (const auto& w : grid)
    {
        PPTX::Logic::TableCol tc;
        tc.Width = w;
        tblGrid.push_back(tc);
    }
}

void TableWriter::prepareShapes(std::vector<CShapeElement *> &arrCells, std::vector<CShapeElement *> &arrSpliters)
{
    for (const auto& ptrShape : m_pTableElement->m_pChildElements)
    {
        auto pShapeEl = dynamic_cast<CShapeElement*>(ptrShape.get());
        switch (pShapeEl->m_lShapeType)
        {
        case 1:
            arrCells.push_back(pShapeEl);
            break;
        case 20:
            arrSpliters.push_back(pShapeEl);
            break;
        }
    }
}

void TableWriter::FillRow(PPTX::Logic::TableRow &oRow, ProtoTableRow& arrCells)
{
    if (arrCells.empty()) return;

    for (auto& protoCell : arrCells)
    {
        PPTX::Logic::TableCell cell;
        protoCell.FillTc(cell);
        oRow.Cells.push_back(cell);
    }
}

std::wstring TableWriter::getXmlForGraphicFrame(int ID, int idx) const
{
    //в таблице могут быть линки и ссылки на другние объекты (картинки???)
    // yliko_provolis.ppt (+ тут есть смарт арты)

    auto& rXml = m_pTableElement->m_xmlRawData;

    PPTX::Logic::GraphicFrame graphic_frame;
    graphic_frame.fromXMLString(rXml);

    if (graphic_frame.nvGraphicFramePr.IsInit())
    {
        graphic_frame.nvGraphicFramePr->cNvPr.id = ID; // или менять в карте связей для анимаций

        if (graphic_frame.nvGraphicFramePr->nvPr.ph.IsInit() && idx >= 0)
        {//проверить
            graphic_frame.nvGraphicFramePr->nvPr.ph->idx = std::to_wstring(idx);
        }
    }
    return graphic_frame.toXML();
}

void TableWriter::CorrectGraphicFrame(PPTX::Logic::GraphicFrame &oGraphicFrame)
{
    FillnvPr(oGraphicFrame.nvGraphicFramePr->nvPr);
}

void TableWriter::FillnvPr(PPTX::Logic::NvPr &oNvPr)
{
    PPTX::Logic::Ext ext;
    ext.uri = L"{D42A27DB-BD31-4B8C-83A1-F6EECF244321}";

    oNvPr.extLst.push_back(ext);
}

TCell::TCell(CShapeElement *pShape, int row, int col, CRelsGenerator* pRels, TCell *pParent) :
    m_pShape(pShape), m_row(row), m_col(col), m_rowSpan(1), m_gridSpan(1),
    m_pParent(pParent), m_parentDirection(none), m_pRels(pRels)
{
    setParentDirection();
}

void TCell::FillTc(PPTX::Logic::TableCell &oTc)
{
    if (m_gridSpan > 1)
        oTc.GridSpan = m_gridSpan;
    if (m_rowSpan > 1)
        oTc.RowSpan = m_rowSpan;

    oTc.txBody = new PPTX::Logic::TxBody(L"a:txBody");
    FillTxBody(oTc.txBody.get2());

    if (m_pParent)
    {
        FillMergeDirection(oTc);
        return;
    }

    oTc.CellProperties = new PPTX::Logic::TableCellProperties;
    FillTcPr(oTc.CellProperties.get2());
}

void TCell::setBorder(TCell::eBorderPossition borderPos, CShapeElement *pBorder)
{
    if (m_pShape)
        m_mapBorders[borderPos] = pBorder;
    else if (m_pParent)
        m_pParent->setBorder(borderPos, pBorder);
}

TCell::eMergeDirection TCell::parentDirection() const
{
    return m_parentDirection;
}

int TCell::getHeight() const
{
    auto pShape = m_pParent ? m_pParent->m_pShape : m_pShape;
    double multip = pShape->m_bAnchorEnabled ? 1587.5 : 1;
    double height = pShape->m_rcChildAnchor.bottom - pShape->m_rcChildAnchor.top;

    return round(height * multip);
}

void TCell::setPParent(TCell *pParent)
{
    m_pParent = pParent;
    setParentDirection();
}

void TCell::FillMergeDirection(PPTX::Logic::TableCell &oTc)
{
    if (parentDirection() & horz)
        oTc.HMerge = true;
    if (parentDirection() & vert)
        oTc.VMerge = true;
}

void TCell::setParentDirection()
{
    if (m_pParent)
    {
        // Todo rewrite it
        if (m_pParent->m_col == m_col && m_pParent->m_row != m_row)
            m_parentDirection = vert;
        else if (m_pParent->m_col != m_col && m_pParent->m_row == m_row)
            m_parentDirection = horz;
        else
            m_parentDirection = hove;
    }
}

void TCell::setGridSpan(int gridSpan)
{
    m_gridSpan = gridSpan;
}

bool TCell::isRealCell() const
{
    if (m_rowSpan == 2 || m_gridSpan == 2 || m_parentDirection != TCell::none)
        return false;
    return true;
}

void TCell::FillTxBody(PPTX::Logic::TxBody &oTxBody)
{
    if (m_pShape == nullptr)
    {
        TxBodyConverter txBodyConverter(nullptr, m_pRels, TxBodyConverter::table);
        txBodyConverter.FillTxBody(oTxBody);
    } else
    {
        TxBodyConverter txBodyConverter(m_pShape, m_pRels, TxBodyConverter::table);
        txBodyConverter.FillTxBody(oTxBody);
    }

}

void TCell::FillTcPr(PPTX::Logic::TableCellProperties &oTcPr)
{
    //anchor
    auto& attr = m_pShape->m_pShape->m_oText.m_oAttributes;
    if (attr.m_nTextAlignVertical != -1)
    {
        auto pAnchor = new PPTX::Limit::TextAnchor;
        BYTE anchor[] = {4,1,0};
        if (sizeof (anchor) > (UINT)attr.m_nTextAlignVertical)
            pAnchor->SetBYTECode(anchor[attr.m_nTextAlignVertical]);

        oTcPr.Anchor = pAnchor;
    }
    if (attr.m_nTextAlignHorizontal == 1)
    {
        oTcPr.AnchorCtr = true;
    }

    oTcPr.MarB = round(m_pShape->m_pShape->m_dTextMarginBottom); // 0
    oTcPr.MarT = round(m_pShape->m_pShape->m_dTextMarginY); // 12512
    oTcPr.MarL = round(m_pShape->m_pShape->m_dTextMarginX);
    oTcPr.MarR = round(m_pShape->m_pShape->m_dTextMarginRight);

    if (true)
    {
        oTcPr.HorzOverflow = new PPTX::Limit::HorzOverflow;
        oTcPr.HorzOverflow->set(L"overflow");
    }

    auto& brush = m_pShape->m_oBrush;
    auto pSolidFill = new PPTX::Logic::SolidFill;

    auto& clr = brush.Color1;
    pSolidFill->Color.SetRGBColor(clr.GetR(), clr.GetG(), clr.GetB());
    if (brush.Alpha1 != 255)
    {
        PPTX::Logic::ColorModifier alpha;
        alpha.name = L"a:alpha";
        alpha.val = brush.Alpha1 * 392;
        pSolidFill->Color.Color->Modifiers.push_back(alpha);
    }
    if (brush.Type == 5000)
    {
        PPTX::Logic::ColorModifier alpha;
        alpha.name = L"a:alpha";
        alpha.val = 0;
        pSolidFill->Color.Color->Modifiers.push_back(alpha);
    }

    oTcPr.Fill.Fill.reset(pSolidFill);

    std::set<eBorderPossition> setOfUnusedBorderPos =
    {TCell::lnB, TCell::lnT, TCell::lnR, TCell::lnL};

    for (auto IterBorder : m_mapBorders)
    {
        auto pLn = new PPTX::Logic::Ln;
        FillLn(*pLn, IterBorder.first, IterBorder.second);
        ApplyLn(oTcPr,pLn, IterBorder.first);
        setOfUnusedBorderPos.erase(IterBorder.first);
    }

    // set invisible other borders
    for (auto unusedBP : setOfUnusedBorderPos)
        SetTcPrInvisibleBorder(oTcPr, unusedBP);
}

void TCell::SetTcPrInvisibleBorders(PPTX::Logic::TableCellProperties &oTcPr) const
{
    for (int i = 0; i < 4; i++)
    {
        auto eBP = (eBorderPossition)i;
        SetTcPrInvisibleBorder(oTcPr, eBP);
    }
}

void TCell::SetTcPrInvisibleBorder(PPTX::Logic::TableCellProperties &oTcPr, TCell::eBorderPossition eBP) const
{
    auto pLn = new PPTX::Logic::Ln;
    pLn->Fill.Fill = new PPTX::Logic::NoFill;
    SetLnName(*pLn, eBP);
    ApplyLn(oTcPr, pLn, eBP);
}

bool TCell::isInvisibleBorder(const CShapeElement *pBorder)
{
    const bool isInv = pBorder && pBorder->m_bLine == false;
    return isInv;
}

bool TCell::isInvisibleBorders() const
{
    if (m_mapBorders.empty())
        return true;

    bool isInvisibele = true;
    for (auto IterBorder : m_mapBorders)
    {
        if ((IterBorder.first != TCell::lnBlToTr && IterBorder.first != TCell::lnTlToBr)
                && isInvisibleBorder(IterBorder.second) == false)
        {
            isInvisibele = false;
        }
    }

    return isInvisibele;
}

void TCell::FillLn(PPTX::Logic::Ln &Ln, TCell::eBorderPossition eBP, CShapeElement *pBorder)
{
    if (pBorder == nullptr)
        return;
    SetLnName(Ln, eBP);

    auto& pen = pBorder->m_oPen;
    auto& brush = pBorder->m_oBrush;

    Ln.w = pen.Size;

    if (isInvisibleBorder(pBorder))
        Ln.Fill.Fill.reset(new PPTX::Logic::NoFill);
    else
    {
        auto pSolidFill = new PPTX::Logic::SolidFill;
        auto& clr = pen.Color;
        pSolidFill->Color.SetRGBColor(clr.GetR(), clr.GetG(), clr.GetB());
        if (pen.Alpha && pen.Alpha != 255)
        {
            PPTX::Logic::ColorModifier alpha;
            alpha.name = L"a:alpha";
            alpha.val = pen.Alpha * 392;
            pSolidFill->Color.Color->Modifiers.push_back(alpha);
        }
        Ln.Fill.Fill.reset(pSolidFill);
    }

    Ln.prstDash = new PPTX::Logic::PrstDash;
    Ln.prstDash->val = new PPTX::Limit::PrstDashVal;
    auto oldDash = pen.DashStyle;
    BYTE dash[] = {6,7,10,8,9,2,0,3,1,4};
    if (sizeof (dash) > oldDash)
        Ln.prstDash->val->SetBYTECode(dash[oldDash]);

    // TODO?
    Ln.cap = new PPTX::Limit::LineCap;
    Ln.cmpd = new PPTX::Limit::CompoundLine;
    Ln.algn = new PPTX::Limit::PenAlign;

    auto* pLineEnd = new PPTX::Logic::LineEnd;
    pLineEnd->m_name = L"a:headEnd";
    pLineEnd->type = new PPTX::Limit::LineEndType;
    pLineEnd->w = new PPTX::Limit::LineEndSize;
    pLineEnd->len = new PPTX::Limit::LineEndSize;
    Ln.headEnd = pLineEnd;

    pLineEnd = new PPTX::Logic::LineEnd;
    pLineEnd->m_name = L"a:tailEnd";
    pLineEnd->type = new PPTX::Limit::LineEndType;
    pLineEnd->w = new PPTX::Limit::LineEndSize;
    pLineEnd->len = new PPTX::Limit::LineEndSize;
    Ln.tailEnd = pLineEnd;

    Ln.Join.type = PPTX::Logic::eJoin::JoinRound;
}

void TCell::SetLnName(PPTX::Logic::Ln &Ln, eBorderPossition eBP) const
{
    switch (eBP)
    {
    case lnL: Ln.m_name = L"a:lnL"; break;
    case lnR: Ln.m_name = L"a:lnR"; break;
    case lnT: Ln.m_name = L"a:lnT"; break;
    case lnB: Ln.m_name = L"a:lnB"; break;
    case lnBlToTr: Ln.m_name = L"a:lnBlToTr"; break;
    case lnTlToBr: Ln.m_name = L"a:lnTlToBr"; break;
    }
}

void TCell::ApplyLn(PPTX::Logic::TableCellProperties &oTcPr, PPTX::Logic::Ln *pLn, eBorderPossition eBP) const
{
    switch (eBP)
    {
    case lnL: oTcPr.LnL = pLn; break;
    case lnR: oTcPr.LnR = pLn; break;
    case lnT: oTcPr.LnT = pLn; break;
    case lnB: oTcPr.LnB = pLn; break;
    case lnBlToTr: oTcPr.LnBlToTr = pLn; break;
    case lnTlToBr: oTcPr.LnTlToBr = pLn; break;
    }
}

void TCell::setRowSpan(int rowSpan)
{
    m_rowSpan = rowSpan;
}

void TCell::setPShape(CShapeElement *pShape)
{
    m_pShape = pShape;
    m_parentDirection = none;
}



ProtoTable::ProtoTable(std::vector<CShapeElement *> &arrCells,
                       std::vector<CShapeElement*>& arrSpliters,
                       CRelsGenerator *pRels) : m_pRels(pRels)
{
    m_arrLeft = getWidth(arrCells, false);
    m_arrTop = getHeight(arrCells, false);
    initProtoTable();
    fillProtoTable(arrCells, arrSpliters);
}
