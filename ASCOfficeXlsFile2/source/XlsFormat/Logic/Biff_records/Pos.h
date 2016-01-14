#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Pos record in BIFF8
class Pos: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Pos)
	BASE_OBJECT_DEFINE_CLASS_NAME(Pos)
public:
	Pos();
	~Pos();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	GlobalWorkbookInfoPtr	pGlobalWorkbookInfoPtr;
		
	int serialize(std::wostream & stream_);

	static const ElementType	type = typePos;

	_UINT16 mdTopLt;
	_UINT16 mdBotRt;
	
	BIFF_SHORT x1;
	BIFF_SHORT y1;
	BIFF_SHORT x2;
	BIFF_SHORT y2;

//-----------------------------

	short			m_iLinkObject;
	BaseObjectPtr	m_Frame;

};

} // namespace XLS

