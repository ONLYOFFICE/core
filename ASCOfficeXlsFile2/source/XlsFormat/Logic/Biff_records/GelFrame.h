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
	void readFields(CFRecord& record);

	static const ElementType	type = typeGelFrame;

//-----------------------------
	ODRAW::OfficeArtFOPT OPT1;
	ODRAW::OfficeArtTertiaryFOPT OPT2;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_COMPLEX(OPT1)
		//BO_ATTRIB_MARKUP_COMPLEX(OPT2)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

