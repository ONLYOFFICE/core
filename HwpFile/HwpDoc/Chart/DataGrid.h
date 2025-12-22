#ifndef DATAGRID_H
#define DATAGRID_H

#include "../Common/Common.h"

namespace HWP { namespace CHART
{
class CDataGrid
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
};
}}

#endif // DATAGRID_H
