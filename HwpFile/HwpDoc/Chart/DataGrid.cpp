#include "DataGrid.h"

namespace HWP { namespace CHART
{
CDataGrid::CDataGrid()
{

}

bool CDataGrid::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nColumnCount) && oStream.ReadString(m_sColumnLabel, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadInt(m_nColumnLabelCount) && oStream.ReadInt(m_nCompositeColumnLabel) &&
	       oStream.ReadString(m_sCompositeRowLabel, DEFAULT_STRING_CHARACTER) && oStream.ReadInt(m_nRowCount) &&
	       oStream.ReadString(m_sRowLabel, DEFAULT_STRING_CHARACTER) && oStream.ReadInt(m_nRowLabelCount);
}
}}
