#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{;


// Logical representation of Pls record in BIFF8
class Pls: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Pls)
	BASE_OBJECT_DEFINE_CLASS_NAME(Pls)
public:
	Pls(BiffStructurePtr & parent);
	~Pls();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePls;

//-----------------------------
	DEVMODE rgb;
	BIFF_LONG bin_data_id;
	BiffStructurePtr parent_;

public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(bin_data_id)
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDeviceName, L"dmDeviceName")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmSpecVersion, L"dmSpecVersion")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDriverVersion, L"dmDriverVersion")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmSize, L"dmSize")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDriverExtra, L"dmDriverExtra")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmFields, L"dmFields")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmOrientation, L"dmOrientation")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPaperSize, L"dmPaperSize")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPaperLength, L"dmPaperLength")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPaperWidth, L"dmPaperWidth")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmScale, L"dmScale")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmCopies, L"dmCopies")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDefaultSource, L"dmDefaultSource")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPrintQuality, L"dmPrintQuality")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmColor, L"dmColor")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDuplex, L"dmDuplex")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmYResolution, L"dmYResolution")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmTTOption, L"dmTTOption")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmCollate, L"dmCollate")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmFormName, L"dmFormName")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmLogPixels, L"dmLogPixels")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmBitsPerPel, L"dmBitsPerPel")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPelsWidth, L"dmPelsWidth")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPelsHeight, L"dmPelsHeight")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDisplayFlags, L"dmDisplayFlags")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDisplayFrequency, L"dmDisplayFrequency")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmICMMethod, L"dmICMMethod")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmICMIntent, L"dmICMIntent")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmMediaType, L"dmMediaType")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDitherType, L"dmDitherType")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPanningWidth, L"dmPanningWidth")
// 		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPanningHeight, L"dmPanningHeight")

	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

