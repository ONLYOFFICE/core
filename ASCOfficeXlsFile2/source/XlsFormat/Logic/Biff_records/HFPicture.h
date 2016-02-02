#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of HFPicture record in BIFF8
class HFPicture: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(HFPicture)
	BASE_OBJECT_DEFINE_CLASS_NAME(HFPicture)
public:
	HFPicture();
	~HFPicture();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeHFPicture;

//-----------------------------
	bool fIsDrawing;
	bool fIsDrawingGroup;
	bool fContinue;
	//OfficeArtDgContainer rgDrawing;

};

} // namespace XLS

