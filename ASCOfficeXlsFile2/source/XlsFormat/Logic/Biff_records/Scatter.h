#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Scatter record in BIFF8
class Scatter: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Scatter)
	BASE_OBJECT_DEFINE_CLASS_NAME(Scatter)
public:
	Scatter();
	~Scatter();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeScatter;

	int	serialize(std::wostream & _stream);

//-----------------------------
	BIFF_WORD pcBubbleSizeRatio;
	BIFF_WORD wBubbleSize;
	BIFF_BOOL fBubbles;
	BIFF_BOOL fShowNegBubbles;
	BIFF_BOOL fHasShadow;

};

} // namespace XLS

