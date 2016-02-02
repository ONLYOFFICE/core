#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of RadarArea record in BIFF8
class RadarArea: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(RadarArea)
	BASE_OBJECT_DEFINE_CLASS_NAME(RadarArea)
public:
	RadarArea();
	~RadarArea();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeRadarArea;

	int serialize(std::wostream & _stream);

//-----------------------------
	bool fRdrAxLab;
	bool fHasShadow;
};

} // namespace XLS

