/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "BiffString.h"
#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

BiffString::BiffString()
:	struct_size(0),
	bDeleteZero(false)
{
}


BiffString::BiffString(const size_t size)
:	struct_size(0),
	cch_(size),
	bDeleteZero(false)
{
}


BiffString::BiffString(const std::wstring & str)
:	struct_size(0),
	str_(str),
	cch_(str.length()),
	bDeleteZero(false)
{
}


BiffString::~BiffString() 
{
}




BiffString::operator std::wstring  () const
{
	return str_;
}


BiffString BiffString::operator=(const std::wstring & str)
{
	str_ = str;
	cch_ = str_.length();
	return *this;
}


BiffStructurePtr BiffString::clone()
{
	return BiffStructurePtr(new BiffString(*this));
}

void BiffString::load(CFRecord& record)
{
	// EXCEPT::LE::WhatIsTheFuck("Wrong usage of BiffString. Stack overflow stopped.", __FUNCTION__);
	//	record >> *this; // :-)
}
void BiffString::load(IBinaryReader* reader)
{
}

void BiffString::load(IBinaryReader* reader, const size_t cch1, const bool is_wide1)
{
	bool is_wide	= is_wide1;
	
	size_t cch = cch1;
	if ((cch_) && (*cch_ != cch1) && cch1 < 1)
	{
		cch =  cch_.get();
	}
	size_t raw_length = cch << (is_wide ? 1 : 0);
	
	if (reader->GetPosition() + cch > reader->GetSize())
	{
		//ОШИБКА - нехватило Continue records - нужно найти место где именно и подзагрузить
		return;
	}
	unsigned char* pData = reader->ReadBytes(cch, true);
	
	if(is_wide)
	{
#if defined(_WIN32) || defined(_WIN64)
		str_ = std::wstring((wchar_t*)pData, cch);
#else
		str_ = convertUtf16ToWString((UTF16*)pData, cch);
#endif
	}
	else
	{
	}
	delete []pData;
}

void BiffString::load(CFRecord& record, const size_t cch1, const bool is_wide1)
{
	bool is_wide	= is_wide1;

	size_t cch = cch1;
	if ((cch_) && (*cch_ != cch1) && cch1 < 1)
	{
		cch =  cch_.get();
	}
	size_t raw_length = cch << (is_wide ? 1 : 0);
	
	if (record.checkFitRead(raw_length)==false)
	{
		//ОШИБКА - нехватило Continue records - нужно найти место где именно и подзагрузить
		return;
	}

	if(is_wide)
	{
		if (false == bDeleteZero)
		{
#if defined(_WIN32) || defined(_WIN64)
			str_ = std::wstring(record.getCurData<wchar_t>(), cch);
#else
			str_= convertUtf16ToWString(record.getCurData<UTF16>(), cch);
#endif
			record.skipNunBytes(raw_length);
		}
		else
		{
			//5804543.xls - font name in dx for table - c a l i 0 0 0 0 b r i - !!!!
			UTF16 *buf_read = new UTF16[cch];

			for (size_t i = 0; i < cch; i++)
			{
				unsigned short val;
				do
				{
					record >> val;
				}while(val == 0);
				buf_read[i] = val;
			}
	#if defined(_WIN32) || defined(_WIN64)
			str_ = std::wstring((wchar_t*)buf_read, cch);
	#else
			str_= convertUtf16ToWString(buf_read, cch);
	#endif
			delete []buf_read;
		}
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
		record.skipNunBytes(raw_length);
	}
}

const size_t BiffString::getSize() const
{
	if(!cch_)
	{
		// EXCEPT::LE::StructureSizeNotSet("BiffString", __FUNCTION__/*__FUNCDNAME__*/); 
	}
	return *cch_;
}


void BiffString::setSize(const size_t size)
{
	cch_ = size;
}


const size_t BiffString::getStructSize() const
{
	return struct_size;
}


// Set number of unsigned chars read while loading. Must be used by >> operator only
void BiffString::setStructSize(const size_t size)
{
	struct_size = size;
}

const std::wstring  BiffString::getEscaped_ST_Xstring() const
{
	if(!str_.length())
	{
		return str_;
	}
	std::wstring copy_str(str_);
	return STR::escape_ST_Xstring(copy_str).c_str();
}


void BiffString::Escape_ST_Xstring()
{
	str_ = getEscaped_ST_Xstring();
}


const bool BiffString::getWideRecommendation() const
{
	std::wstring str(str_);
	for(std::wstring::const_iterator it = str.begin(), itEnd = str.end(); it != itEnd; ++it)
	{
		if(0xff00 & *it)
		{
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------
BiffStructurePtr XLUnicodeStringSegmented::clone()
{
	return BiffStructurePtr(new XLUnicodeStringSegmented(*this));
}
void XLUnicodeStringSegmented::load(CFRecord& record)
{
	record >> cchTotal;

	if (cchTotal < 1) return;
	
	if (cchTotal > record.getDataSize() - record.getRdPtr())
	{
		cchTotal = cchTotal >> 8;
	}

	_UINT32 cchTotal_test = 0;
	while(true)
	{
		if (record.isEOF())
			break;

		if (cchTotal_test >= cchTotal) 
			break;
		
		_UINT32 max_string_size = cchTotal - cchTotal_test;

		XLUnicodeString string;
		record >> string;
		
		arStrings.push_back(string.value());

		cchTotal_test	+= arStrings.back().length();
		strTotal		+= arStrings.back();
	}
}
void XLUnicodeStringSegmented::load(IBinaryReader* reader)
{
	cchTotal = reader->ReadUInt32();

	if (cchTotal < 1) return;
	
	if (cchTotal > reader->GetSize() - reader->GetPosition())
	{
		cchTotal = cchTotal >> 8;
	}

	_UINT32 cchTotal_test = 0;
	while(true)
	{
		if (reader->GetPosition() >= reader->GetSize())
			break;

		if (cchTotal_test >= cchTotal) 
			break;
		
		_UINT32 max_string_size = cchTotal - cchTotal_test;

		XLUnicodeString string;
		string.load(reader);
		
		arStrings.push_back(string.value());

		cchTotal_test	+= arStrings.back().length();
		strTotal		+= arStrings.back();
	}
}
} // namespace XLS
