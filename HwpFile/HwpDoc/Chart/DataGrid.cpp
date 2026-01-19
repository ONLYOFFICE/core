#include "DataGrid.h"

namespace HWP { namespace CHART
{
CDataGrid::CDataGrid()
{

}

bool CDataGrid::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nColumnCount) && oStream.ReadString(m_sColumnLabel) &&
	       oStream.ReadInteger(m_nColumnLabelCount) && oStream.ReadInteger(m_nCompositeColumnLabel) &&
	       oStream.ReadString(m_sCompositeRowLabel) && oStream.ReadInteger(m_nRowCount) &&
	       oStream.ReadString(m_sRowLabel) && oStream.ReadInteger(m_nRowLabelCount);
}
}}
