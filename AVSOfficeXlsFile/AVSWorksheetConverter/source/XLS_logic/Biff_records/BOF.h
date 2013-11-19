#pragma once

#include "BiffRecord.h"
//#include <XLS_logic/Biff_structures/FilePointer.h>

namespace XLS
{;

class CFStreamCacheReader;

// Logical representation of BOF record in BIFF8
class BOF: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BOF)
	BASE_OBJECT_DEFINE_CLASS_NAME(BOF)
public:
	BOF();
	~BOF();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	WORD getSubstreamType();

	typedef enum
	{
		st_Workbook = 0x0005,
		st_Globals = st_Workbook,
		st_Dialog = 0x0010,
		st_Worksheet = st_Dialog,
		st_Chart = 0x0020,
		st_Macro = 0x040
	} SubstreamType;

//	MSXML2::IXMLDOMElementPtr makeXMLNode(const std::string& substream_name);

private:
	BIFF_WORD vers;
	BIFF_WORD dt;
	BIFF_WORD rupBuild;
	BIFF_WORD rupYear;
	BIFF_BOOL fWin;
	BIFF_BOOL fRisc;
	BIFF_BOOL fBeta;
	BIFF_BOOL fWinAny;
	BIFF_BOOL fMacAny;
	BIFF_BOOL fBetaAny;
	BIFF_BOOL fRiscAny;
	BIFF_BOOL fOOM;
	BIFF_BOOL fGlJmp;
	BIFF_BOOL fFontLimit;
	BIFF_WORD verXLHigh;
	BIFF_BYTE verLowestBiff;
	BIFF_BYTE verLastXLSaved;
	ForwardOnlyParam<DWORD> stream_ptr;
	

public:
	
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(vers)
		BO_ATTRIB_MARKUP_ATTRIB(dt)
		BO_ATTRIB_MARKUP_ATTRIB(rupBuild)
		BO_ATTRIB_MARKUP_ATTRIB(rupYear)
		BO_ATTRIB_MARKUP_ATTRIB(fWin)
		BO_ATTRIB_MARKUP_ATTRIB(fRisc)
		BO_ATTRIB_MARKUP_ATTRIB(fBeta)
		BO_ATTRIB_MARKUP_ATTRIB(fWinAny)
		BO_ATTRIB_MARKUP_ATTRIB(fMacAny)
		BO_ATTRIB_MARKUP_ATTRIB(fBetaAny)
		BO_ATTRIB_MARKUP_ATTRIB(fRiscAny)
		BO_ATTRIB_MARKUP_ATTRIB(fOOM)
		BO_ATTRIB_MARKUP_ATTRIB(fGlJmp)
		BO_ATTRIB_MARKUP_ATTRIB(fFontLimit)
		BO_ATTRIB_MARKUP_ATTRIB(verXLHigh)
		BO_ATTRIB_MARKUP_ATTRIB(verLowestBiff)
		BO_ATTRIB_MARKUP_ATTRIB(verLastXLSaved)
		BO_ATTRIB_MARKUP_ATTRIB(stream_ptr)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

