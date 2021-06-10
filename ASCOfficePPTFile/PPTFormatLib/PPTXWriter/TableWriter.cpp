#include "TableWriter.h"
#include "TxBodyConverter.h"

TableWriter::TableWriter(CTableElement *pTableElement) :
    m_pTableElement(pTableElement)
{

}

void TableWriter::Convert(PPTX::Logic::GraphicFrame &oGraphicFrame)
{
    if (!m_pTableElement)
        return;

    oGraphicFrame.nvGraphicFramePr = new PPTX::Logic::NvGraphicFramePr;
    FillNvGraphicFramePr(oGraphicFrame.nvGraphicFramePr.get2());

    oGraphicFrame.xfrm = new PPTX::Logic::Xfrm;
    FillXfrm(oGraphicFrame.xfrm.get2());

    oGraphicFrame.table = new PPTX::Logic::Table;
    FillTable(oGraphicFrame.table.get2());
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
    oXFRM.m_ns = L"p";
    double multip1 = m_pTableElement->m_bAnchorEnabled ? 1587.6 : 1;
    double multip2 = m_pTableElement->m_bAnchorEnabled ? 1273.0 : 1;
    oXFRM.offX = int(m_pTableElement->m_rcAnchor.left * multip1);
    oXFRM.offY = int(m_pTableElement->m_rcAnchor.top  * multip1);

    oXFRM.extX = int(m_pTableElement->m_rcAnchor.right  * multip2);
    oXFRM.extY = int(m_pTableElement->m_rcAnchor.bottom * multip2);
}

void TableWriter::FillTable(PPTX::Logic::Table &oTable)
{
    oTable.tableProperties = new PPTX::Logic::TableProperties;
    FillTblPr(oTable.tableProperties.get2());

    std::vector<CShapeElement*> arrCells, arrSpliters;
    prepareShapes(arrCells, arrSpliters);
    m_nPTable = new ProtoTable(arrCells, arrSpliters);
    FillTblGrid(oTable.TableCols , arrCells);


    auto protoTable = m_nPTable->getTable();
    auto arrHeight = ProtoTable::getHeight(arrCells);
    for (UINT cRow = 0; cRow < protoTable.size(); cRow++)
    {
        PPTX::Logic::TableRow tr;
        FillRow(tr, protoTable[cRow]);
        tr.Height = arrHeight[cRow];
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
    double multip = isWidth ? 1587.6 : 1.0;
    for (const auto& w : mapLeftWidth)
    {
        double value = isWidth ? w.second : w.first;
        gridWidth.push_back(int(value * multip));
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
    double multip = isHeight ? 1587.6 : 1.0;
    for (const auto& h : mapTopHeight)
    {
        double value = isHeight ? h.second : h.first;
        gridHeight.push_back(int(value * multip));
    }

    return gridHeight;
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
            protoRow.push_back(TCell(nullptr, cRow, cCol, nullptr));

        m_table.push_back(protoRow);
    }
}

bool ProtoTable::fillProtoTable(std::vector<CShapeElement *> &arrCells,
                                std::vector<CShapeElement*>& arrSpliters)
{
    if (m_table.empty() || m_arrTop.empty() || m_arrLeft.empty())
        return false;

    bool wasCellsFilled = fillCells(arrCells);

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
    oTblPr.TableStyleId = L"{5C22544A-7EE6-4342-B048-85BDC9FD1C3A}";
    oTblPr.FirstRow = true;
    oTblPr.BandRow = true;
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

TCell::TCell(CShapeElement *pShape, int row, int col, TCell *pParent) :
    m_pShape(pShape), m_row(row), m_col(col), m_rowSpan(1), m_gridSpan(1),
    m_pParent(pParent), m_parentDirection(none)
{
    setParentDirection();
}

void TCell::setArrBorders(const std::vector<CShapeElement *> &arrBorders)
{
    m_arrBorders = arrBorders;
}

void TCell::FillTc(PPTX::Logic::TableCell &oTc)
{
    if (m_gridSpan > 1)
        oTc.GridSpan = m_gridSpan;
    if (m_rowSpan > 1)
        oTc.RowSpan = m_rowSpan;

    oTc.txBody = new PPTX::Logic::TxBody(L"a:txBody");
    FillTxBody(oTc.txBody.get2());
    oTc.CellProperties = new PPTX::Logic::TableCellProperties;
    // ToDo FillTableCellProperties(oTc.CellProperties.get2())

    if (m_pParent)
    {
        FillMergeDirection(oTc);
        return;
    }
}

TCell::eMergeDirection TCell::parentDirection() const
{
    return m_parentDirection;
}

int TCell::getHeight() const
{
    auto pShape = m_pParent ? m_pParent->m_pShape : m_pShape;
    double multip = pShape->m_bAnchorEnabled ? 1587.6 : 1;
    double height = pShape->m_rcChildAnchor.bottom - pShape->m_rcChildAnchor.top;

    return int(height * multip);
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

void TCell::FillTxBody(PPTX::Logic::TxBody &oTxBody)
{
    if (m_pShape == nullptr)
        return;

    TxBodyConverter txBodyConverter(&m_pShape->m_pShape->m_oText, TxBodyConverter::table);
    txBodyConverter.FillTxBody(oTxBody);
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
                       std::vector<CShapeElement*>& arrSpliters)
{
    m_arrLeft = getWidth(arrCells, false);
    m_arrTop = getHeight(arrCells, false);
    initProtoTable();
    fillProtoTable(arrCells, arrSpliters);
}
