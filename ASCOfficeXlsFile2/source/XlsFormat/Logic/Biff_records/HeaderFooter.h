#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


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

	static const ElementType	type = typeHeaderFooter;

//-----------------------------
	std::wstring	guidSView;
	bool			fHFDiffOddEven;
	bool			fHFDiffFirst;
	bool			fHFScaleWithDoc;
	bool			fHFAlignMargins;
	
	_UINT16 cchHeaderEven;
	_UINT16 cchFooterEven;
	_UINT16 cchHeaderFirst;
	_UINT16 cchFooterFirst;

	XLUnicodeString strHeaderEven;
	XLUnicodeString strFooterEven;
	XLUnicodeString strHeaderFirst;
	XLUnicodeString strFooterFirst;

};

} // namespace XLS

