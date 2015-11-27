#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of CRT union of records 
class CRT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CRT)
public:
	CRT();
	~CRT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeCRT;

	BaseObjectPtr		m_ChartFormat;
	BaseObjectPtr		m_ChartType;

	BaseObjectPtr		m_SeriesList;
	BaseObjectPtr		m_CrtLink;
	BaseObjectPtr		m_Chart3d;
	BaseObjectPtr		m_LD;

	BaseObjectPtr				m_DROPBAR[2];
	std::vector<BaseObjectPtr>	m_arCrtLine;
	BaseObjectPtr				m_SS;
	BaseObjectPtr				m_DataLabExtContents;

	std::vector<BaseObjectPtr>	m_arDFTTEXT;
	std::vector<BaseObjectPtr>	m_arSHAPEPROPS;


};

} // namespace XLS

