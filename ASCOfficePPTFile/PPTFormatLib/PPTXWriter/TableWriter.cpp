#include "TableWriter.h"

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
    // graphicFrameLocks

    PPTX::Logic::Ext ext;
    ext.uri = L"{D42A27DB-BD31-4B8C-83A1-F6EECF244321}";
    oNvGFPr.nvPr.extLst.push_back(ext);
}

void TableWriter::FillXfrm(PPTX::Logic::Xfrm &oXFRM)
{
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
    FillTblGrid(oTable.TableCols , arrCells);

    auto arrRows = createProtoTable(arrCells);
    for (auto& row : *arrRows)
    {
        PPTX::Logic::TableRow tr;
        FillRow(tr, row);
        oTable.TableRows.push_back(tr);
    }
}

std::vector<int> TableWriter::getWidth(std::vector<CShapeElement*>& arrCells, bool isWidth)
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

std::vector<std::vector<CShapeElement*> > TableWriter::getRows(std::vector<CShapeElement *> &arrCells)
{
    std::vector<std::vector<CShapeElement*> > arrRows;
    int rowTop = -1;
    for (auto* pCell : arrCells)
    {
        if (!pCell) continue;

        double multip = m_pTableElement->m_bAnchorEnabled ? 1587.6 : 1;
        int top = int(pCell->m_rcChildAnchor.top * multip);
        if (top != rowTop)
        {
            rowTop = top;
            arrRows.push_back(std::vector<CShapeElement*>());
        }
        arrRows.back().push_back(pCell);
    }

    return arrRows;
}

void TableWriter::FillTblPr(PPTX::Logic::TableProperties &oTblPr)
{
    oTblPr.TableStyleId = L"{F5AB1C69-6EDB-4FF4-983F-18BD219EF322}";
    oTblPr.FirstRow = true;
    oTblPr.BandRow = true;
}

void TableWriter::FillTblGrid(std::vector<PPTX::Logic::TableCol> &tblGrid, std::vector<CShapeElement*>& arrCells)
{
    auto grid = getWidth(arrCells);
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

ptrProtoTable TableWriter::createProtoTable(std::vector<CShapeElement*> &arrCells)
{
    auto gridWidth = getWidth(arrCells, false);
    auto arrRows = getRows(arrCells);
    auto cellIter = arrCells.begin();

    const unsigned countRows = arrRows.size();
    const unsigned countCols = gridWidth.size();

    ptrProtoTable pProtoTable(new ProtoTable);
    for (unsigned cRow = 0; cRow < countRows; cRow++)
    {
        pProtoTable->push_back(std::vector<TCell>());
        std::vector<TCell>& row = pProtoTable->back();

        int top = arrRows[cRow].front()->m_rcChildAnchor.top;
        for (unsigned cCol = 0; cCol < countCols; cCol++)
        {
            int left = gridWidth[cCol];

            CShapeElement* current = (*cellIter);
            if (current &&
                    current->m_rcChildAnchor.top == top &&
                    current->m_rcChildAnchor.left == left)
            {
                row.push_back(TCell(current, cRow, cCol));
                cellIter++;
            } else
            {
                TCell* pParent = FindCellParent(pProtoTable, cRow, cCol);
                row.push_back(TCell(nullptr, cRow, cCol, pParent));
            }

        }
    }

    return  pProtoTable;
}

TCell *TableWriter::FindCellParent(ptrProtoTable pPT, int row, int col)
{
    if (!pPT || pPT->empty() || row < 0 || col < 1)
        return nullptr;

    TCell::eMergeDirection curDir = row > 0 ?
                (*pPT)[--row][col].parentDirection() : (*pPT)[row][--col].parentDirection();

    // go up
    while (curDir == TCell::vert && row >= 0)
    {
        if (curDir == TCell::none)
            return &(*pPT)[row][col];
        curDir = (*pPT)[--row][col].parentDirection();
    }

    // go up and left
    while (curDir == TCell::hove && row >= 0 && col >= 0)
    {
        if (curDir == TCell::none)
            return &(*pPT)[row][col];
        curDir = (*pPT)[--row][--col].parentDirection();
    }

    // go left
    while (curDir == TCell::horz && col >= 0)
    {
        if (curDir == TCell::none)
            return &(*pPT)[row][col];
        curDir = (*pPT)[row][--col].parentDirection();
    }

    return &(*pPT)[(row >= 0 ? row : 0)][(col >= 0 ? col : 0)];
}

int TableWriter::getTRHeight(const ProtoTableRow& row)
{
    const TCell* pCell = nullptr;
    for (const auto& cell : row)
        if (cell.parentDirection() == TCell::none)
        {
            pCell = &cell;
            break;
        }

    return pCell ? pCell->getHeight() : 0;
}

void TableWriter::FillRow(PPTX::Logic::TableRow &oRow, ProtoTableRow& arrCells)
{
    if (arrCells.empty()) return;

    oRow.Height = getTRHeight(arrCells);
    for (auto& protoCell : arrCells)
    {
        PPTX::Logic::TableCell cell;
        protoCell.FillTc(cell);
        oRow.Cells.push_back(cell);
    }
}

TCell::TCell(CShapeElement *pShape, int row, int col, TCell *pParent) :
    m_pShape(pShape), m_row(row), m_col(col), m_pParent(pParent), m_parentDirection(none)
{
    if (m_pParent)
    {
        if (m_pParent->m_col == m_col && m_pParent->m_row != m_row)
            m_parentDirection = vert;
        else if (m_pParent->m_col != m_col && m_pParent->m_row == m_row)
            m_parentDirection = horz;
        else
            m_parentDirection = hove;
    }
}

void TCell::setArrBorders(const std::vector<CShapeElement *> &arrBorders)
{
    m_arrBorders = arrBorders;
}

void TCell::FillTc(PPTX::Logic::TableCell &oTc)
{
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

void TCell::FillMergeDirection(PPTX::Logic::TableCell &oTc)
{
    oTc.HMerge = parentDirection() & horz;
    oTc.VMerge = parentDirection() & vert;
}


