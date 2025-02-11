#ifndef OLECONVERTER_H
#define OLECONVERTER_H

#include "../HWPStream.h"

namespace HWP
{
class COleConverter
{
	unsigned int m_unCountCharts;
	std::wstring m_wsTempDir;

	void CreateChartData(const std::wstring& wsChartData);
public:
	COleConverter();

	void Clear();

	unsigned int GetChartsCount() const;

	void SetTempDir(const std::wstring& wsTempDir);
	void CreateChart(CHWPStream& oOleStream);
	void CreateRels();
};
}

#endif // OLECONVERTER_H
