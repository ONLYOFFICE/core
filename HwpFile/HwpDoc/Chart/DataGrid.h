#ifndef DATAGRID_H
#define DATAGRID_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CDataGrid : public IChartObject
{
	int m_nColumnCount;
	HWP_STRING m_sColumnLabel;
	int m_nColumnLabelCount;
	int m_nCompositeColumnLabel;
	HWP_STRING m_sCompositeRowLabel;
	int m_nRowCount;
	HWP_STRING m_sRowLabel;
	int m_nRowLabelCount;
public:
	CDataGrid();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // DATAGRID_H
