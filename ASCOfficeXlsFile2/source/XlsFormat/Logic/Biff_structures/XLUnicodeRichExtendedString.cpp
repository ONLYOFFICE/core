#include "precompiled_xls.h"
#include "XLUnicodeRichExtendedString.h"
#include <Logic/BaseObject.h>

namespace XLS
{;


XLUnicodeRichExtendedString XLUnicodeRichExtendedString::operator=(const XLUnicodeRichExtendedString& other)
{
	XLUnicodeRichExtendedString tmp(other);
	std::swap(*this, tmp);
	return *this;
}



XLUnicodeRichExtendedString::XLUnicodeRichExtendedString(std::list<CFRecordPtr>& cont_recs)
:	cont_recs_(cont_recs),
	fHighByte(true)
{
}


const bool XLUnicodeRichExtendedString::appendNextContinue(CFRecord& record, const bool read_high_byte)
{
	if(cont_recs_.empty())
	{
		return false;
	}
	if(read_high_byte)
	{
		fHighByte = 0x01 == cont_recs_.front()->getData()[0];
		record.appendRawData(cont_recs_.front()->getData() + 1, cont_recs_.front()->getDataSize() - 1);
	}
	else
	{
		record.appendRawData(cont_recs_.front());
	}
	cont_recs_.pop_front();
	return true;
}


XLUnicodeRichExtendedString::~XLUnicodeRichExtendedString()
{
}


BiffStructurePtr XLUnicodeRichExtendedString::clone()
{
	return BiffStructurePtr(new XLUnicodeRichExtendedString(*this));
}

//
//void XLUnicodeRichExtendedString::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	static std::wstring  text_str(L"text");
//	xml_tag->setAttribute(text_str, STR::escape_ST_Xstring(static_cast<wchar_t*>(str_)).c_str());
//
//	std::for_each(rgRun.begin(), rgRun.end(), boost::bind(&FormatRun::toXML, _1, xml_tag));
//	if(fExtSt)
//	{
//		extRst.toXML(xml_tag);
//	}
//	
//}
//
//void XLUnicodeRichExtendedString::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	static std::wstring  text_str(L"text");
//	static std::wstring  mark_set_start_str(L"mark_set_start");
//	str_ = getStructAttribute(xml_tag, text_str);
//	str_ = STR::unescape_ST_Xstring(static_cast<wchar_t*>(str_)).c_str();
//
//	fRichSt = false;
//	FormatRun run;
//	size_t number = 0;
//	while(run.fromXML(xml_tag, ++number, false))
//	{
//		rgRun.push_back(run);
//		fRichSt = true;
//	}
//
//	fExtSt = false;
//	if(extRst.fromXML(xml_tag))
//	{
//		fExtSt = true;
//	}
//	mark_set_start = getStructAttribute(xml_tag, mark_set_start_str);
//
//}

void XLUnicodeRichExtendedString::store(CFRecord& record)
{
	if(mark_set_start)
	{
		record.registerDelayedFilePointerAndOffsetSource(record.getDataSize(), rt_ExtSST);
		record.registerDelayedDataSource(record.getDataSize(), rt_ExtSST);
	}

	unsigned short cch = str_.length();
	record << cch;
	unsigned char flags = 0;
	fHighByte = true; // We'll save always in UNICODE
	SETBIT(flags, 0, fHighByte);
	SETBIT(flags, 2, fExtSt);
	SETBIT(flags, 3, fRichSt);
	record << flags;
	unsigned short cRun = rgRun.size();
	if(fRichSt)
	{
		record << cRun;
	}
	if(fExtSt)
	{
		long cbExtRst = extRst.getSize();
		record << cbExtRst;
	}

	if(!record.checkFitWriteSafe(static_cast<size_t>(cch) << (fHighByte ? 1 : 0)))
	{
		size_t space_left = (record.getMaxRecordSize() - record.getDataSize()) >> (fHighByte ? 1 : 0);
		storeSymbols(record, 0, space_left, fHighByte);

		size_t pos1 = space_left;
		do
		{
			CFRecord& current_record = cont_recs_.size() ? *cont_recs_.back() : record; // Points to the original 'record' or the last created 'Continue'
			current_record.commitData();
			CFRecordPtr cont(new CFRecord(rt_Continue, record.getGlobalWorkbookInfo()));
			cont_recs_.push_back(cont);
			unsigned char flags = 0;
			SETBIT(flags, 0, fHighByte);
			*cont << flags;
			size_t cch_to_write = cont->checkFitWriteSafe((static_cast<size_t>(cch) - pos1) << (fHighByte ? 1 : 0)) ? 
									(static_cast<size_t>(cch) - pos1) :
									(cont->getMaxRecordSize() - cont->getDataSize()) >> (fHighByte ? 1 : 0);
			storeSymbols(*cont, pos1, cch_to_write, fHighByte);
			pos1 += cch_to_write;
			
		} while(pos1 < static_cast<size_t>(cch));

	}
	else
	{
		storeSymbols(record, 0, cch, fHighByte);
	}

	for(std::vector<FormatRun>::iterator it = rgRun.begin(), itEnd = rgRun.end(); it != itEnd; ++it)
	{
		record << *it;
	}

	if(fExtSt)
	{
		record << extRst;
	}
}


void XLUnicodeRichExtendedString::load(CFRecord& record)
{
	unsigned short cch;
	unsigned char flags;
	record >> cch >> flags;
	fHighByte = GETBIT(flags, 0);
	fExtSt = GETBIT(flags, 2);
	fRichSt = GETBIT(flags, 3);
	unsigned short cRun = 0;
	if(fRichSt)
	{
		record >> cRun;
	}
	long cbExtRst = 0;
	if(fExtSt)
	{
		record >> cbExtRst;
	}
	
	if(!record.checkFitReadSafe(static_cast<size_t>(cch) << (fHighByte ? 1 : 0)))
	{
		size_t num_symbols = min(cch, (record.getDataSize() - record.getRdPtr()) >> (fHighByte ? 1 : 0));
		loadSymbols(record, num_symbols, fHighByte);
		std::wstring  str_full(str_);
		do
		{
			appendNextContinue(record, true); // fHighByte changes here
			num_symbols = min(cch - str_full.length(), (record.getDataSize() - record.getRdPtr()) >> (fHighByte ? 1 : 0));
			loadSymbols(record, num_symbols, fHighByte); // cch has changed, fHighByte has changed
			str_full += str_;
		} 
		while(str_full.length() < cch);
		
		std::swap(str_full, str_); // Thus we avoid additional copy in operator=
	}
	else
	{
		loadSymbols(record, cch, fHighByte);
	}
	
	for(size_t i = 0; i < cRun; ++i)
	{
		FormatRun format;
		record >> format;
		rgRun.push_back(format);
	}

	if(fExtSt)
	{
		record >> extRst;
	}
}

void XLUnicodeRichExtendedString::loadSymbols(CFRecord& record, const size_t cch, const bool is_wide)
{
	size_t raw_length = cch << (is_wide ? 1 : 0);
	record.checkFitRead(raw_length);

	if(is_wide)
	{
		std::wstring int_str(record.getCurData<wchar_t>(), cch);
		str_ = int_str.c_str();
	}
	else
	{
		std::string int_str(record.getCurData<char>(), cch);
		str_ = STR::toStdWString(int_str, record.getGlobalWorkbookInfo()->CodePage).c_str();
	}
	record.skipNunBytes(raw_length);
}


void XLUnicodeRichExtendedString::storeSymbols(CFRecord& record, const size_t start_pos, const size_t cch, const bool is_wide)
{
	size_t raw_length = cch << (is_wide ? 1 : 0);
	record.checkFitWrite(raw_length);

	if(is_wide)
	{
		std::wstring int_str = str_.substr(start_pos, cch);
		for(std::wstring::iterator it = int_str.begin(), itEnd = int_str.end(); it != itEnd; ++it)
		{
			record << *it;
		}
		// no trailing zero - it is OK
	}
	else
	{
		std::string int_str = STR::toStdString(str_, record.getGlobalWorkbookInfo()->CodePage).substr(start_pos, cch);
		for(std::string::iterator it = int_str.begin(), itEnd = int_str.end(); it != itEnd; ++it)
		{
			record << *it;
		}
		// no trailing zero - it is OK
	}
}




CFRecord& operator>>(CFRecord& record, XLUnicodeRichExtendedString& val)
{
	val.load(record);
	return record;
}


const size_t XLUnicodeRichExtendedString::getNonVariablePartSize() const
{	
	unsigned short size = sizeof(unsigned short)/*cch*/ + sizeof(unsigned char)/*flags*/;
	if(fRichSt)
	{
		size += sizeof(unsigned short)/*cRun*/;
	}
	if(fExtSt)
	{
		size += sizeof(long)/*cbExtRst*/;
	}
	return size;
}


const size_t XLUnicodeRichExtendedString::getFullSize() const
{	
	unsigned short size = getNonVariablePartSize();

	size += str_.length() << (fHighByte ? 1 : 0);

	size += rgRun.size() * (2 * sizeof(unsigned short))/*FormatRun*/;

	if(fExtSt)
	{
		size += extRst.getSize();
	}

	return size;
}


} // namespace XLS
