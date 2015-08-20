#pragma once

#include "BiffStructure.h"
#include <Binary/CFRecord.h>
#include <Logic/Biff_structures/FormatRun.h>
#include <Logic/Biff_structures/ExtRst.h>

namespace XLS
{

class GlobalWorkbookInfo;
typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;

class XLUnicodeRichExtendedString : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XLUnicodeRichExtendedString)
public:
	XLUnicodeRichExtendedString(std::list<CFRecordPtr>& cont_recs);
	~XLUnicodeRichExtendedString();
	XLUnicodeRichExtendedString operator=(const XLUnicodeRichExtendedString& other);

	static const ElementType	type = typeXLUnicodeRichExtendedString;

	BiffStructurePtr clone();

	int serialize		(std::wostream & _stream);	
	int serialize_rPr	(std::wostream & _stream, int iFmt);

	void set_code_page(short cp) {code_page_ = cp;}
	
	virtual void load	(CFRecord& record);
	virtual void store	(CFRecord& record);

	GlobalWorkbookInfoPtr pGlobalWorkbookInfoPtr;

	void loadSymbols(CFRecord& record, const size_t cch, const bool is_wide);
	void storeSymbols(CFRecord& record, const size_t start_pos, const size_t cch, const bool is_wide);

	const size_t getNonVariablePartSize() const;
	const size_t getFullSize() const;

	const bool appendNextContinue(CFRecord& record, const bool read_high_byte);

	std::wstring  str_;

	bool fHighByte;
	bool fExtSt;
	bool fRichSt;
	
	std::vector<FormatRun> rgRun;
	
	ExtRst extRst;
	std::list<CFRecordPtr>& cont_recs_;
	bool mark_set_start;

	unsigned short code_page_ ;
};

typedef boost::shared_ptr<XLUnicodeRichExtendedString> XLUnicodeRichExtendedStringPtr;

CFRecord& operator>>(CFRecord& record, XLUnicodeRichExtendedString& val);

} // namespace XLS
