#include "ChartReader.h"

#include "VtChart.h"

namespace HWP { namespace CHART
{
CChartReader::CChartReader()
{

}

bool CChartReader::ReadFromOle(CHWPStream& oOleData)
{
	CChartStream oChartStream(&oOleData);

	oChartStream.Skip(44); //Unknown data

	CHART_STRING sStoredName;
	oChartStream.ReadString(sStoredName);

	CVtChart oVtChart;

	if (!oVtChart.Read(oChartStream))
		return false;

	return false;
}
}}
