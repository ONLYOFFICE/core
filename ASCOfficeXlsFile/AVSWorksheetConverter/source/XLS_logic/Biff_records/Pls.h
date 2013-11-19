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
	Pls(MSXML2::IXMLDOMElementPtr parent);
	~Pls();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	DEVMODE rgb;
	BIFF_LONG bin_data_id;
	MSXML2::IXMLDOMElementPtr parent_;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(bin_data_id)
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDeviceName, "dmDeviceName")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmSpecVersion, "dmSpecVersion")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDriverVersion, "dmDriverVersion")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmSize, "dmSize")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDriverExtra, "dmDriverExtra")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmFields, "dmFields")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmOrientation, "dmOrientation")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPaperSize, "dmPaperSize")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPaperLength, "dmPaperLength")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPaperWidth, "dmPaperWidth")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmScale, "dmScale")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmCopies, "dmCopies")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDefaultSource, "dmDefaultSource")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPrintQuality, "dmPrintQuality")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmColor, "dmColor")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDuplex, "dmDuplex")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmYResolution, "dmYResolution")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmTTOption, "dmTTOption")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmCollate, "dmCollate")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmFormName, "dmFormName")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmLogPixels, "dmLogPixels")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmBitsPerPel, "dmBitsPerPel")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPelsWidth, "dmPelsWidth")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPelsHeight, "dmPelsHeight")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDisplayFlags, "dmDisplayFlags")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDisplayFrequency, "dmDisplayFrequency")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmICMMethod, "dmICMMethod")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmICMIntent, "dmICMIntent")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmMediaType, "dmMediaType")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmDitherType, "dmDitherType")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPanningWidth, "dmPanningWidth")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rgb.dmPanningHeight, "dmPanningHeight")

	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

