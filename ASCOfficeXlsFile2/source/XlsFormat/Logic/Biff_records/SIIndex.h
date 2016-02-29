#pragma once

#include "BiffRecord.h"

#include <Logic/Biff_structures/ChartParsedFormula.h>

namespace XLS
{


// Logical representation of SIIndex record in BIFF8
class SIIndex: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SIIndex)
	BASE_OBJECT_DEFINE_CLASS_NAME(SIIndex)
public:
	SIIndex();
	~SIIndex();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	
	static const ElementType	type = typeSIIndex;

	int serialize(std::wostream & _stream, ChartParsedFormula & in_ref);
	int serialize(std::wostream & _stream, int idx, const CellRef & in_ref);

//-----------------------------
	_UINT16					numIndex;

	std::vector<BaseObjectPtr >	m_arData;

};

} // namespace XLS

