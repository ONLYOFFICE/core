#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;

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

	int serialize		(std::wostream & _stream);
	int serialize_title (std::wostream & _stream, const BaseObjectPtr & attached_label, const BaseObjectPtr & dft_text);

	static const ElementType	type = typeChartSheetSubstream;

	BaseObjectPtr				m_BACKGROUND;
	std::vector<BaseObjectPtr>	m_arFbi;
	BaseObjectPtr				m_CHARTFORMATS;
	BaseObjectPtr				m_SERIESDATA;
	BaseObjectPtr				m_OBJECTSCHART;

};

} // namespace XLS

