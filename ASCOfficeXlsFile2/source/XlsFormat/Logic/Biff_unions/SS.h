#pragma once

#include <Logic/CompositeObject.h>

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

