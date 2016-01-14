#pragma once

#include <Logic/CompositeObject.h>

#define CHART_TYPE_Bar			1
#define CHART_TYPE_Line			2
#define CHART_TYPE_Pie			3
#define CHART_TYPE_Area			4
#define CHART_TYPE_Surf			5		
#define CHART_TYPE_Radar		6
#define CHART_TYPE_RadarArea	7
#define CHART_TYPE_BopPop		8		
#define CHART_TYPE_Scatter		9
#define CHART_TYPE_Bubble		10
#define CHART_TYPE_Stock		11
#define CHART_TYPE_Doughnut		12

namespace XLS
{


// Logical representation of SS union of records 
class SS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SS)
public:
	SS();
	~SS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSS;

	void apply_crt_ss		(BaseObjectPtr crt_ss);

	int serialize			(std::wostream & _stream, int series_type, int ind = -1);
	int serialize_default	(std::wostream & _stream, int series_type, int ind);
	int serialize2			(std::wostream & _stream, int series_type);
	
	BaseObjectPtr	m_DataFormat;
	
	BaseObjectPtr	m_Chart3DBarShape;
	BaseObjectPtr	m_LineFormat;
	BaseObjectPtr	m_AreaFormat;
	BaseObjectPtr	m_PieFormat;

	BaseObjectPtr	m_SerFmt;
	BaseObjectPtr	m_GELFRAME;
	BaseObjectPtr	m_MarkerFormat;
	BaseObjectPtr	m_AttachedLabel;
	
	std::vector<BaseObjectPtr >	m_arSHAPEPROPS;

//---------------------------------------------------

	bool			m_is3D;
	_CP_OPT(bool)	m_isVaried;

	bool			m_isAutoFill;
	bool			m_isAutoLine;
	
};

} // namespace XLS

