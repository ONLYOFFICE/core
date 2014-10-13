#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of HeaderFooter record in BIFF8
class HeaderFooter: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(HeaderFooter)
	BASE_OBJECT_DEFINE_CLASS_NAME(HeaderFooter)
public:
	HeaderFooter();
	~HeaderFooter();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BSTR guidSView;
	BIFF_BOOL fHFDiffOddEven;
	BIFF_BOOL fHFDiffFirst;
	BIFF_BOOL fHFScaleWithDoc;
	BIFF_BOOL fHFAlignMargins;
	BIFF_WORD cchHeaderEven;
	BIFF_WORD cchFooterEven;
	BIFF_WORD cchHeaderFirst;
	BIFF_WORD cchFooterFirst;
	XLUnicodeString strHeaderEven;
	XLUnicodeString strFooterEven;
	XLUnicodeString strHeaderFirst;
	XLUnicodeString strFooterFirst;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(guidSView)
		BO_ATTRIB_MARKUP_ATTRIB(fHFDiffOddEven)
		BO_ATTRIB_MARKUP_ATTRIB(fHFDiffFirst)
		BO_ATTRIB_MARKUP_ATTRIB(fHFScaleWithDoc)
		BO_ATTRIB_MARKUP_ATTRIB(fHFAlignMargins)
// 		BO_ATTRIB_MARKUP_ATTRIB(cchHeaderEven)
// 		BO_ATTRIB_MARKUP_ATTRIB(cchFooterEven)
// 		BO_ATTRIB_MARKUP_ATTRIB(cchHeaderFirst)
// 		BO_ATTRIB_MARKUP_ATTRIB(cchFooterFirst)
		BO_ATTRIB_MARKUP_ATTRIB(strHeaderEven)
		BO_ATTRIB_MARKUP_ATTRIB(strFooterEven)
		BO_ATTRIB_MARKUP_ATTRIB(strHeaderFirst)
		BO_ATTRIB_MARKUP_ATTRIB(strFooterFirst)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

