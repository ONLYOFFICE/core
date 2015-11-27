#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of CHARTFOMATS union of records 
class CHARTFORMATS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CHARTFORMATS)
public:
	CHARTFORMATS();
	~CHARTFORMATS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	BaseObjectPtr				m_ChartRect;
	std::vector<BaseObjectPtr>	m_arSERIESFORMAT;
	std::vector<BaseObjectPtr>	m_arFONTLIST;
	BaseObjectPtr				m_Scl;
	BaseObjectPtr				m_PlotGrowth;
	BaseObjectPtr				m_FRAME;
	BaseObjectPtr				m_AxesUsed;
	BaseObjectPtr				m_ShtProps;
	BaseObjectPtr				m_TEXTPROPS;
	BaseObjectPtr				m_DAT;
	BaseObjectPtr				m_CrtLayout12A;

	std::vector<BaseObjectPtr>	m_arSS;
	std::vector<BaseObjectPtr>	m_arAXISPARENT;
	std::vector<BaseObjectPtr>	m_arATTACHEDLABEL;
	std::vector<BaseObjectPtr>	m_arDFTTEXT;

	struct _chart_format
	{
		BaseObjectPtr	attachedLABEL;
		BaseObjectPtr	dataLabExt;
	};
	std::vector<_chart_format>	m_arChartFormats;

//------------------------------------------------------------------------------
	BaseObjectPtr find_label		( _UINT16 link_id);
	BaseObjectPtr find_default_text	( _UINT16 link_id);
};

} // namespace XLS

