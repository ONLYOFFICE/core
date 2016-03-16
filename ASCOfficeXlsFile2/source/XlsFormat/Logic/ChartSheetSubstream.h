#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;

class CHARTFORMATS;
class SERIESDATA;
class CRT;

class ChartSheetSubstream;
typedef boost::shared_ptr<ChartSheetSubstream>	ChartSheetSubstreamPtr;

// Logical representation of ChartSheetSubstream union of records 
class ChartSheetSubstream: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ChartSheetSubstream)
public:
	ChartSheetSubstream();
	~ChartSheetSubstream();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	int serialize			(std::wostream & _stream);
	int serialize_title		(std::wostream & _stream);
	int serialize_plot_area (std::wostream & _stream);
	int serialize_legend	(std::wostream & _stream, const std::wstring & legend_entries);
	int serialize_ser		(std::wstring sNodeName, std::wostream & _stream, int series_id, const BaseObjectPtr & ai, int type, int count);
	int serialize_dPt		(std::wostream & _stream, int series_id, CRT *crt, int count_point);
	int serialize_dLbls		(std::wostream & _stream, int series_id, CRT *crt);
	int serialize_3D		(std::wostream & _stream);
	
	int serialize_scatter_style(std::wostream & _stream, CRT *crt);

	static const ElementType	type = typeChartSheetSubstream;

	BaseObjectPtr				m_BACKGROUND;
	std::vector<BaseObjectPtr>	m_arFbi;
	BaseObjectPtr				m_CHARTFORMATS;
	BaseObjectPtr				m_SERIESDATA;
	BaseObjectPtr				m_OBJECTSCHART;
	std::vector<BaseObjectPtr>	m_arWINDOW;
	std::vector<BaseObjectPtr>	m_arCUSTOMVIEW;
private:

	void recalc(CHARTFORMATS*	charts);
	void recalc(SERIESDATA*		data);

	std::map<int, std::vector<int>> m_mapTypeChart;//тут нужен несортированый .. пока оставим этот


	GlobalWorkbookInfoPtr pGlobalWorkbookInfo;


};

} // namespace XLS

