#ifndef DATAGRID_H
#define DATAGRID_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CDataGrid : public IChartObject
{
	CHART_INTEGER m_nColumnCount;
	CHART_STRING m_sColumnLabel;
	CHART_INTEGER m_nColumnLabelCount;
	CHART_INTEGER m_nCompositeColumnLabel;
	CHART_STRING m_sCompositeRowLabel;
	CHART_INTEGER m_nRowCount;
	CHART_STRING m_sRowLabel;
	CHART_INTEGER m_nRowLabelCount;
public:
	CDataGrid();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // DATAGRID_H
