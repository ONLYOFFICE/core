#include "Table.h"

namespace NSDocxRenderer
{
	CRow::CRow() : CBaseItem(ElemType::etTable)
	{
	}

	CRow::~CRow()
	{
		Clear();
	}

	void CRow::Clear()
	{
		m_arCells.clear();
	}

	void CRow::AddContent(CBaseItem* pObj)
	{
		CBaseItem::AddContent(pObj);

		m_arCells.push_back(dynamic_cast<CCell*>(pObj));
	}

	CTable::CTable() : CBaseItem(ElemType::etTable)
	{

	}

	CTable::~CTable()
	{
		Clear();
	}

	void CTable::Clear()
	{
		m_arRows.clear();
	}

	void CTable::AddContent(CBaseItem* pObj)
	{
		CBaseItem::AddContent(pObj);

		m_arRows.push_back(dynamic_cast<CRow*>(pObj));
	}

	void CTable::ToXml(NSStringUtils::CStringBuilder &oWriter)
	{
		if (m_bIsNotNecessaryToUse)
		{
			return;
		}

		oWriter.WriteString(L"<w:tbl>");

		oWriter.WriteString(L"<w:tblPr>");
		oWriter.WriteString(L"<w:tblStyle w:val=\"TableGrid\"/>");

		if (m_bIsNeedSpacing)
		{
			oWriter.WriteString(L"<w:spacing");
			if (m_dSpaceBefore > 0)
			{
				oWriter.WriteString(L" w:before=\"");
				oWriter.AddInt(static_cast<int>(m_dSpaceBefore * c_dMMToDx));
				oWriter.WriteString(L"\"");
			}

			if (m_dSpaceAfter > 0)
			{
				oWriter.WriteString(L" w:after=\"");
				oWriter.AddInt(static_cast<int>(m_dSpaceAfter * c_dMMToDx));
				oWriter.WriteString(L"\"");
			}

			if (m_dLineHeight > 0)
			{
				oWriter.WriteString(L" w:line=\"");
				oWriter.AddInt(static_cast<int>(m_dLineHeight * c_dMMToDx));
				oWriter.WriteString(L"\" w:lineRule=\"exact\""); // exact - точный размер строки
			}
			oWriter.WriteString(L"/>"); //конец w:spacing

			oWriter.WriteString(L"<w:jc w:val=\"start\"/>");
			oWriter.WriteString(L"<w:tblInd");
			oWriter.WriteString(L" w:w=\"");
			oWriter.AddInt(static_cast<int>(m_dLeft * c_dMMToDx));
			oWriter.WriteString(L"\" w:type=\"dxa\"/>");
		}
		else
		{
			oWriter.WriteString(L"<w:tblpPr");
			oWriter.WriteString(L" w:leftFromText=\"144\" w:rightFromText=\"144\" w:topFromText=\"144\" w:bottomFromText=\"144\"");
			oWriter.WriteString(L" w:vertAnchor=\"page\" w:horzAnchor=\"page\"");
			oWriter.WriteString(L" w:tblpX=\"");
			oWriter.AddInt(static_cast<int>(m_dLeft * c_dMMToDx));
			oWriter.WriteString(L"\" w:tblpY=\"");
			oWriter.AddInt(static_cast<int>(m_dTop * c_dMMToDx));
			oWriter.WriteString(L"\"/>");
		}

		oWriter.WriteString(L"<w:tblW w:w=\"");
		oWriter.AddInt(static_cast<int>(m_dWidth * c_dMMToDx));
		oWriter.WriteString(L"\" w:type=\"auto\"/>");

		oWriter.WriteString(L"<w:tblLook");
		//oWriter.WriteString(L" w:firstRow=\"1\"");
		//oWriter.WriteString(L" w:lastRow=\"0\"");
		//oWriter.WriteString(L" w:firstColumn=\"1\"");
		//oWriter.WriteString(L" w:lastColumn=\"0\"");
		oWriter.WriteString(L" w:noHBand=\"1\"");
		oWriter.WriteString(L" w:noVBand=\"1\"");
		oWriter.WriteString(L"/>");
		oWriter.WriteString(L"</w:tblPr>");

		oWriter.WriteString(L"<w:tblGrid>");
		for (size_t i = 0; i < m_arColumnWidths.size(); ++i)
		{
			oWriter.WriteString(L"<w:gridCol w:w=\"");
			oWriter.AddUInt(m_arColumnWidths[i] * c_dMMToDx);
			oWriter.WriteString(L"\"/>");
		}
		oWriter.WriteString(L"</w:tblGrid>");

		for (size_t nRow = 0; nRow < m_arRows.size(); ++nRow)
		{
			auto pRow = m_arRows[nRow];

			oWriter.WriteString(L"<w:tr>");

			oWriter.WriteString(L"<w:trPr>");
			oWriter.WriteString(L"<w:trHeight w:val=\"");
			oWriter.AddUInt(pRow->m_dHeight * c_dMMToDx);
			oWriter.WriteString(L"\"/>");
			oWriter.WriteString(L"</w:trPr>");

			for (size_t j = 0; j < pRow->m_arCells.size(); ++j)
			{
				pRow->m_arCells[j]->ToXml(oWriter);
			}

			oWriter.WriteString(L"</w:tr>");
		}

		oWriter.WriteString(L"</w:tbl>");
	}

	//note: для полной таблицы, составляющей в сумме ячеек прямоугольник
	void CTable::CalculateColumnWidth()
	{
		m_arColumnWidths.clear();

		if (m_arRows.empty())
		{
			return;
		}

		//todo пока работает с простыми таблицами, где ширины ячеек по всем рядам совпадают
		//if (m_arRows.size() == 1)
		{
			for (size_t i = 0; i < m_arRows.front()->m_arCells.size(); ++i)
			{
				m_arColumnWidths.push_back(m_arRows.front()->m_arCells[i]->m_dWidth);
			}
			return;
		}

		double dMinWidth = 0;
		double dCurrLeftBorder = m_dLeft; //начальное состояние равно левому краю таблицы
		CCell* pCurrCellWithMinWidth = nullptr;

		//todo Доработать логику для сложных таблиц, где ширины ячеек по всем рядам могут не совпадать
		//заполняем m_arColumnWidths пока dCurrLeftBorder не сравняется с правым краем таблицы
		while(dCurrLeftBorder < m_dRight/* && fabs(dCurrLeftBorder - m_dRight) > c_dGRAPHICS_ERROR_MM*/) //где-то тут ошибка!
		{
			//находим минимальный width после текущей dCurrLeftBorder
			for (size_t i = 0; i < m_arRows.size(); ++i)
			{
				for (size_t j = 0; m_arRows[i]->m_arCells.size(); ++j)
				{
					auto pCell = m_arRows[i]->m_arCells[j];

					if (dCurrLeftBorder < pCell->m_dRight && fabs(dCurrLeftBorder - pCell->m_dRight) > c_dGRAPHICS_ERROR_MM)
					{
						if (pCell->m_dRight - dCurrLeftBorder < dMinWidth || dMinWidth == 0)
						{
							dMinWidth = pCell->m_dRight - dCurrLeftBorder;
							pCurrCellWithMinWidth = pCell;
						}
						break;
					}
				}
			}

			m_arColumnWidths.push_back(pCurrCellWithMinWidth->m_dWidth);

			//увеличиваем GridSpan для ячеек между dCurrLeftBorder и правым краем текущей минимальной ячейки
			for (size_t i = 0; i < m_arRows.size(); ++i)
			{
				for (size_t j = 0; m_arRows[i]->m_arCells.size(); ++j)
				{
					auto pCell = m_arRows[i]->m_arCells[j];

					if (dCurrLeftBorder < pCell->m_dRight &&
							fabs(dCurrLeftBorder - pCell->m_dRight) > c_dGRAPHICS_ERROR_MM &&
							pCell->m_dRight > pCurrCellWithMinWidth->m_dRight &&
							fabs(pCell->m_dRight - pCurrCellWithMinWidth->m_dRight) > c_dGRAPHICS_ERROR_MM)
					{
						++pCell->m_uGridSpan;
						break;
					}
				}
			}

			//сдвигаем dCurrLeftBorder
			dCurrLeftBorder = pCurrCellWithMinWidth->m_dRight;
		}
	}
}
