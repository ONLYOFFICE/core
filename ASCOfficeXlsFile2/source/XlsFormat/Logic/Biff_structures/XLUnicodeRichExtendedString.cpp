/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "XLUnicodeRichExtendedString.h"
#include <Logic/BaseObject.h>
#include <Logic/GlobalWorkbookInfo.h>
#include <Logic/Biff_records/Font.h>

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
	extRst(cont_recs)
{
	code_page_  = 0;
}

const bool XLUnicodeRichExtendedString::appendNextContinue(CFRecord& record, const bool read_high_byte)
{
	if(cont_recs_.empty())
	{
		return false;
	}

	RecordType type = (RecordType)cont_recs_.front()->getTypeId();
	
	while(!cont_recs_.empty())
	{
		type = (RecordType)cont_recs_.front()->getTypeId();

		if (type == rt_SST || type == rt_Continue)
		{
			if(read_high_byte)
			{
				fHighByte = (0x01 == cont_recs_.front()->getData()[0]);
				record.appendRawData(cont_recs_.front()->getData() + 1, cont_recs_.front()->getDataSize() - 1);
			}
			else
			{
				record.appendRawData(cont_recs_.front());
			}
			cont_recs_.pop_front();
			break;
		}
		else
			cont_recs_.pop_front();

	}
	return true;
}


XLUnicodeRichExtendedString::~XLUnicodeRichExtendedString()
{
}

void XLUnicodeRichExtendedString::set_code_page(short cp)
{
	code_page_ = cp;
}


BiffStructurePtr XLUnicodeRichExtendedString::clone()
{
	return BiffStructurePtr(new XLUnicodeRichExtendedString(*this));
}

int XLUnicodeRichExtendedString::serialize (std::wostream & _stream)
{
	int start_string = 0;
	int Fmt = 0; //форматы со сдвигом !!! .. первый - тот что определен в ячейке.
	
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
					CP_XML_STREAM() << STR::escape_ST_Xstring(xml::utils::replace_text_to_xml(str_part));
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
					CP_XML_ATTR(L"xml:space", L"preserve");
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
	if (!pGlobalWorkbookInfoPtr)			return 0;
	if (!pGlobalWorkbookInfoPtr->m_arFonts) return 0;

	int sz = pGlobalWorkbookInfoPtr->m_arFonts->size();
	if (iFmt -1 > sz || iFmt < 1) return 0;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"rPr")
		{
			Font * font = dynamic_cast<Font*>(pGlobalWorkbookInfoPtr->m_arFonts->at(iFmt-1).get());

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
		extRst.store(record);
	}
}


void XLUnicodeRichExtendedString::load(CFRecord& record)
{
	pGlobalWorkbookInfoPtr = record.getGlobalWorkbookInfo();

	if (code_page_== 0) 
		code_page_ = pGlobalWorkbookInfoPtr ->CodePage;

	unsigned short cch = 0;
	unsigned char flags = 0;
	record >> cch >> flags;

	if (record.getDataSize() == record.getRdPtr())
	{
		if (appendNextContinue(record,false) == false) 
			return;
	}

	fHighByte	= GETBIT(flags, 0);
	fExtSt		= GETBIT(flags, 2);
	fRichSt		= GETBIT(flags, 3);

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
		size_t str_sz = num_symbols;

		std::wstring  str_full(str_);
		do
		{
			if (appendNextContinue(record, true) == false) // fHighByte changes here
				break; //dicionario de kanji.xls
			num_symbols = (std::min)(cch - str_full.length(), (record.getDataSize() - record.getRdPtr()) >> (fHighByte ? 1 : 0));
			loadSymbols(record, num_symbols, fHighByte); // cch has changed, fHighByte has changed
			str_full	+= str_;
			str_sz		+= num_symbols;
		} 
		while(str_sz < cch);
		
		std::swap(str_full, str_); // Thus we avoid additional copy in operator=
	}
	else
	{
		loadSymbols(record, cch, fHighByte);
	}
	if (record.getRdPtr() + cRun * 4 > record.getDataSize() && !cont_recs_.empty())
	{
		record.appendRawData(cont_recs_.front());		
		cont_recs_.pop_front();	
	}
	for(size_t i = 0; i < cRun; ++i)
	{
		FormatRun format;
		record >> format;
		rgRun.push_back(format);
	}

	if(fExtSt && cbExtRst > 0)
	{
		if (record.getRdPtr() + cbExtRst > record.getDataSize() && !cont_recs_.empty())
		{
			record.appendRawData(cont_recs_.front());		
			cont_recs_.pop_front();	
		}
		extRst.load(record);
	}
}

void XLUnicodeRichExtendedString::loadSymbols(CFRecord& record, const size_t cch, const bool is_wide)
{
	size_t raw_length = cch << (is_wide ? 1 : 0);
	if (record.checkFitRead(raw_length))
	{
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
			std::string inp_str(record.getCurData<char>(), cch);

			if (record.getGlobalWorkbookInfo()->CodePage == 1200)
			{
				int inp_str_size = inp_str.length();
                UTF16 *out_str = new UTF16[inp_str_size + 1];
				char* out_str_char = (char*) out_str;
				for (int i = 0; i < inp_str_size; i++)
				{
					out_str_char[2*i+0] = inp_str.c_str()[i];
					out_str_char[2*i+1] = 0;
				}
				out_str[inp_str_size] = 0;

#if defined(_WIN32) || defined(_WIN64)
                str_ = std::wstring((wchar_t*)out_str, inp_str_size);
#else
                str_ = convertUtf16ToWString(out_str, inp_str_size);
#endif
                delete []out_str;
			}
			else
			{
				str_ = STR::toStdWString(inp_str, record.getGlobalWorkbookInfo()->CodePage).c_str();
			}
		}
		record.skipNunBytes(raw_length);
	}
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
