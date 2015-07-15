#pragma once

#include "BiffStructure.h"
#include <Binary/CFRecord.h>
#include <Logic/Biff_structures/FormatRun.h>
#include <Logic/Biff_structures/ExtRst.h>

namespace XLS
{;

class XLUnicodeRichExtendedString : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XLUnicodeRichExtendedString)
public:
	XLUnicodeRichExtendedString(std::list<CFRecordPtr>& cont_recs);
	~XLUnicodeRichExtendedString();
	XLUnicodeRichExtendedString operator=(const XLUnicodeRichExtendedString& other);


	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	void loadSymbols(CFRecord& record, const size_t cch, const bool is_wide);
	void storeSymbols(CFRecord& record, const size_t start_pos, const size_t cch, const bool is_wide);

	const size_t getNonVariablePartSize() const;
	const size_t getFullSize() const;

//private:
	// Append data of the next Continue record if exists and remove it from the list
	const bool appendNextContinue(CFRecord& record, const bool read_high_byte);

private:
	std::wstring  str_;
	bool fHighByte;
	bool fExtSt;
	bool fRichSt;
	std::vector<FormatRun> rgRun;
	ExtRst extRst;
	std::list<CFRecordPtr>& cont_recs_;
	bool mark_set_start;
};

typedef boost::shared_ptr<XLUnicodeRichExtendedString> XLUnicodeRichExtendedStringPtr;

CFRecord& operator>>(CFRecord& record, XLUnicodeRichExtendedString& val);

} // namespace XLS
