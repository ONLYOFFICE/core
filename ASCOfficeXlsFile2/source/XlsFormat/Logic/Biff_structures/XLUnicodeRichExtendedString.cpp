
#include "XLUnicodeRichExtendedString.h"
#include <Logic/BaseObject.h>
#include <Logic/GlobalWorkbookInfo.h>
#include <Logic/Biff_records/Font.h>

#include <simple_xml_writer.h>
#include <utils.h>

namespace XLS
{

XLUnicodeRichExtendedString XLUnicodeRichExtendedString::operator=(const XLUnicodeRichExtendedString& other)
{
	XLUnicodeRichExtendedString tmp(other);
	std::swap(*this, tmp);
	return *this;
}

XLUnicodeRichExtendedString::XLUnicodeRichExtendedString(std::list<CFRecordPtr>& cont_recs)
:	cont_recs_(cont_recs),
	fHighByte(true),
	code_page_(0)
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
	code_page_  = 0;
}


BiffStructurePtr XLUnicodeRichExtendedString::clone()
{
	return BiffStructurePtr(new XLUnicodeRichExtendedString(*this));
}

int XLUnicodeRichExtendedString::serialize (std::wostream & _stream)
{
	int start_string = 0;
	int Fmt = 0; //форматы со сдвигом !!! .. первый - тот что определен в €чейке.
	
	CP_XML_WRITER(_stream)    
	{
		for (int i = 0 ; i < rgRun.size(); i++)
		{
			CP_XML_NODE(L"r")
			{
				serialize_rPr(CP_XML_STREAM(), Fmt );
				Fmt = rgRun[i].ifnt;

				CP_XML_NODE(L"t")
				{		
					CP_XML_ATTR(L"xml:space", L"preserve");

					std::wstring str_part = str_.substr( start_string, rgRun[i].ich - start_string );
					start_string = rgRun[i].ich;
					CP_XML_STREAM() << xml::utils::replace_text_to_xml(str_part);
				}
			}
		}
		if (start_string < str_.size())
		{
			CP_XML_NODE(L"r")
			{
				serialize_rPr(CP_XML_STREAM(), Fmt );

				CP_XML_NODE(L"t")
				{	
					std::wstring str_part = str_.substr( start_string, str_.size() - start_string );
					CP_XML_STREAM() << xml::utils::replace_text_to_xml(str_part);
				}
			}
		}
	}
	
	return 0;
}
int XLUnicodeRichExtendedString::serialize_rPr	(std::wostream & _stream, int iFmt)
{
	if (!pGlobalWorkbookInfoPtr)		return 0;
	if (!pGlobalWorkbookInfoPtr->fonts) return 0;

	int sz = pGlobalWorkbookInfoPtr->fonts->size();
	if (iFmt -1 > sz || iFmt < 1) return 0;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"rPr")
		{
			Font * font = dynamic_cast<Font*>(pGlobalWorkbookInfoPtr->fonts->at(iFmt-1).get());

			if (font) font->serialize_properties(CP_XML_STREAM(), true);
		}
	}
	
	return 0;
}

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
        int cbExtRst = extRst.getSize();
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
	pGlobalWorkbookInfoPtr = record.getGlobalWorkbookInfo();

	if (code_page_== 0) 
		code_page_ = pGlobalWorkbookInfoPtr ->CodePage;

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
    int cbExtRst = 0;
	if(fExtSt)
	{
		record >> cbExtRst;

		if (cbExtRst < 0)
		{
			fExtSt = false;
			fRichSt = true;
			record.RollRdPtrBack(4);

			record >> cRun;
		}
	}
	
	if(!record.checkFitReadSafe(static_cast<size_t>(cch) << (fHighByte ? 1 : 0)))
	{
        size_t num_symbols = (std::min)(static_cast<size_t>(cch), (record.getDataSize() - record.getRdPtr()) >> (fHighByte ? 1 : 0));
		loadSymbols(record, num_symbols, fHighByte);
		std::wstring  str_full(str_);
		do
		{
			if (appendNextContinue(record, true) == false) // fHighByte changes here
				break; //dicionario de kanji.xls
            num_symbols = (std::min)(cch - str_full.length(), (record.getDataSize() - record.getRdPtr()) >> (fHighByte ? 1 : 0));
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

	if(fExtSt && cbExtRst > 0)
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
#if defined(_WIN32) || defined(_WIN64)
        std::wstring int_str(record.getCurData<wchar_t>(), cch);
		str_ = int_str.c_str();
#else
        str_ = convertUtf16ToWString(record.getCurData<UTF16>(), cch);
#endif
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
        size += sizeof(unsigned short)/*cRun*/; //2 !!!!
	}
	if(fExtSt)
	{
        size += sizeof(int)/*cbExtRst*/; // ??? 4!!!
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
