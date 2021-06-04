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

    auto arrRows = getRows(arrCells);
    for (auto row : arrRows)
    {
        PPTX::Logic::TableRow tr;
        FillRow(tr, row);
        oTable.TableRows.push_back(tr);
    }
}

std::vector<int> TableWriter::getWidth(std::vector<CShapeElement*>& arrCells)
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
    for (const auto& w : mapLeftWidth)
        gridWidth.push_back(int(w.second * 1587.6));

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

int TableWriter::getCellHeight(const CShapeElement *pCell) const
{
    double multip = m_pTableElement->m_bAnchorEnabled ? 1587.6 : 1;
    double height = pCell->m_rcChildAnchor.bottom - pCell->m_rcChildAnchor.top;
    return int(height * multip);
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

void TableWriter::FillRow(PPTX::Logic::TableRow &oRow, std::vector<CShapeElement*>& arrCells)
{
    if (arrCells.empty()) return;

    oRow.Height = getCellHeight(arrCells.front());
}
