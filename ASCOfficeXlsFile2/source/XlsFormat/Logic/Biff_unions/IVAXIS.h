#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of IVAXIS union of records 
class IVAXIS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(IVAXIS)
public:
	IVAXIS();
	~IVAXIS();

	BaseObjectPtr clone();

	virtual const bool loadContentRead(BinReaderProcessor& proc);
	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeIVAXIS;

	int serialize(std::wostream & _stream);

	unsigned int id;

	BaseObjectPtr	m_Axis;
	BaseObjectPtr	m_AXS;

	BaseObjectPtr	m_CatSerRange;
	BaseObjectPtr	m_AxcExt;
	BaseObjectPtr	m_CatLab;

	BaseObjectPtr	m_CRTMLFRT;

//---------------------------------
	bool m_bSecondary;
	bool bDataAxis;
};

} // namespace XLS

