#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Chart3d record in BIFF8
class Chart3d: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Chart3d)
	BASE_OBJECT_DEFINE_CLASS_NAME(Chart3d)
public:
	Chart3d();
	~Chart3d();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeChart3d;

//-----------------------------
	BIFF_SHORT anRot;
	BIFF_SHORT anElev;
	BIFF_SHORT pcDist;
	BIFF_DWORD pcHeight;
	BIFF_SHORT pcDepth;
	BIFF_WORD pcGap;

	BIFF_BOOL fPerspective;
	BIFF_BOOL fCluster;
	BIFF_BOOL f3DScaling;
	BIFF_BOOL fNotPieChart;
	BIFF_BOOL fWalls2D;

};

} // namespace XLS

