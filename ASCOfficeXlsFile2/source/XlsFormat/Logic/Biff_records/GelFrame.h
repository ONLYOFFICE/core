#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/ODRAW/OfficeArtFOPT.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtTertiaryFOPT.h>

namespace XLS
{


// Logical representation of GelFrame record in BIFF8
class GelFrame: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(GelFrame)
	BASE_OBJECT_DEFINE_CLASS_NAME(GelFrame)
public:
	GelFrame();
	~GelFrame();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields	(CFRecord& record);

	void concatinate(GelFrame * addit);

	static const ElementType	type = typeGelFrame;

//-----------------------------
	ODRAW::OfficeArtFOPT			OPT1;
	ODRAW::OfficeArtTertiaryFOPT	OPT2;

};

} // namespace XLS

