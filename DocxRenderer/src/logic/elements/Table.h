#pragma once
#include "Cell.h"

namespace NSDocxRenderer
{
	class CRow : public CBaseItem
	{
	public:
		std::vector<CCell*> m_arCells;

	public:
		CRow() = default;
		virtual ~CRow();
		virtual void Clear() override final;
		virtual void AddContent(CBaseItem* pObj) override final;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final {}
	};

	class CTable : public COutputObject
	{
	public:
		std::vector<double> m_arColumnWidths; //общее количество колонок в таблице
		std::vector<CRow*>  m_arRows;

		double              m_dSpaceBefore {0.0}; //по умолчанию выставляется 0, если отсутсвует w:before
		double              m_dSpaceAfter {0.0}; //в shape по умолчанию выставляется 8pt, если отсутсвует w:after
		double              m_dLineHeight {0.0};
		bool                m_bIsNeedSpacing {false};

	public:
		CTable() = default;
		virtual ~CTable();
		virtual void Clear() override final;
		virtual void AddContent(CBaseItem* pItem) override final;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;

		double CalculateBeforeSpacing(double dPreviousStringBaseline)
		{
			return m_dTop - dPreviousStringBaseline;
		}
		void CalculateColumnWidth();
	};
}
