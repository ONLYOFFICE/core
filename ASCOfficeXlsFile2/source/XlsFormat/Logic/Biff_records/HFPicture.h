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
	BIFF_BOOL fIsDrawing;
	BIFF_BOOL fIsDrawingGroup;
	BIFF_BOOL fContinue;
	//OfficeArtDgContainer rgDrawing;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fIsDrawing)
		//BO_ATTRIB_MARKUP_ATTRIB(fIsDrawingGroup)
		//BO_ATTRIB_MARKUP_ATTRIB(fContinue)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

