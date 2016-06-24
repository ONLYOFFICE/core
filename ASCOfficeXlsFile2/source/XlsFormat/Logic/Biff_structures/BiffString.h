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
#pragma once

#include "BiffAttribute.h"

namespace XLS
{

typedef enum
{
	aw_ANSI = 0,
	aw_WIDE = 1,
	aw_READ_FROM_RECORD = 2,
	aw_READ_FROM_CCH = 3,
	aw_READ_FROM_RECORD_IF_CCH_NOT_ZERO,
} AW_DETERMINATION;


typedef enum
{
	cch_READ_FROM_RECORD = 0,
	cch_PASSED_AS_AN_ARGUMENT = 1,
} CCH_SOURCE;


class BiffString : public BiffAttribute
{
public:
	BiffString();
	BiffString(const size_t size);
	BiffString(const std::wstring & str);
	~BiffString();


	operator std::wstring  () const;

	std::wstring & value() {return str_;}

	BiffString operator=(const std::wstring & str);
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);
	
	void load(CFRecord& record, const size_t cch, const bool is_wide);
	void store(CFRecord& record, const bool is_wide);

	void setSize(const size_t size);		// Set cch for string that don't have own cch field
	const size_t getStructSize() const;		// Number of unsigned chars read while loading
	void setStructSize(const size_t size);	// Set number of unsigned chars read while loading. Must be used by >> operator only
	const size_t getSize() const;			// String length in characters

	const bool isConformToOleLink() const;
	const bool isConformToVirtPath() const;

	const std::wstring  getEscaped_ST_Xstring() const;
	void Escape_ST_Xstring();

	const bool getWideRecommendation() const;

protected:
	std::wstring 	str_;

	size_t struct_size;
	_CP_OPT(size_t) cch_;
};


template<class cchType, AW_DETERMINATION det_id, CCH_SOURCE cch_where>
class XLUnicodeString_T : public BiffString
{
public:
    XLUnicodeString_T() {}
	XLUnicodeString_T(const size_t size) : BiffString(size) {}
    XLUnicodeString_T(const std::wstring & str) : BiffString(str) {}
	XLUnicodeString_T operator=(const std::wstring & str) 
	{
		std::wstring  temp(str);
		std::swap(str_, temp);
		cch_ = str.length();
		return *this;
    }
	const size_t getStructSizeWouldWritten() const // Number of unsigned chars that would be written
	{
		return recalculateStructSize();
    }

private:
	const size_t recalculateStructSize() const
	{
		size_t struct_size_local = 0;
		bool is_wide = false;
		
		if(cch_READ_FROM_RECORD == cch_where)
		{
			if(aw_READ_FROM_CCH == det_id)
			{
				is_wide = true;
			}
			struct_size_local += sizeof(cchType);
		}

		switch(det_id)
		{
			case aw_READ_FROM_RECORD:
				struct_size_local += sizeof(unsigned char)/*fHighByte*/;
				// no break is OK
			case aw_WIDE:
				is_wide = true;
				break;
			case aw_ANSI:
				is_wide = false;
				break;
		}
		return struct_size_local + (cch_.get_value_or(0) << (is_wide ? 1 : 0));
	}
private:
	_CP_OPT(bool) is_wide_rec;
};


typedef XLUnicodeString_T<unsigned short,	aw_READ_FROM_RECORD,					cch_READ_FROM_RECORD>		XLUnicodeString;
typedef XLUnicodeString_T<unsigned char,	aw_READ_FROM_RECORD,					cch_READ_FROM_RECORD>		ShortXLUnicodeString;
typedef XLUnicodeString_T<unsigned short,	aw_WIDE,								cch_READ_FROM_RECORD>		LPWideString;
typedef XLUnicodeString_T<unsigned short,	aw_WIDE,								cch_PASSED_AS_AN_ARGUMENT>	LPWideStringNoCch;
typedef XLUnicodeString_T<unsigned short,	aw_READ_FROM_RECORD,					cch_PASSED_AS_AN_ARGUMENT>	XLUnicodeStringNoCch;
typedef XLUnicodeString_T<unsigned short,	aw_READ_FROM_RECORD_IF_CCH_NOT_ZERO,	cch_READ_FROM_RECORD>		XLUnicodeStringMin2;
typedef XLUnicodeString_T<unsigned short,	aw_ANSI,								cch_READ_FROM_RECORD>		LPAnsiString;
typedef XLUnicodeString_T<unsigned char,	aw_ANSI,								cch_READ_FROM_RECORD>		ShortXLAnsiString;
typedef XLUnicodeString_T<unsigned short,	aw_ANSI,								cch_PASSED_AS_AN_ARGUMENT>	LPAnsiStringNoCch;


template<class cchType, AW_DETERMINATION det_id, CCH_SOURCE cch_where>
CFRecord& operator>>(CFRecord& record, XLUnicodeString_T<cchType, det_id, cch_where>& val)
{
	size_t cch;
	size_t struct_size = 0;

	if (record.getRdPtr() >= record.getDataSize())
		return record;

	bool is_wide = false;
	
	switch(cch_where)
	{
		case cch_READ_FROM_RECORD:
			cchType cch_l; // Just to conform size read.
			record >> cch_l;
			cch = cch_l;
			struct_size += sizeof(cchType);
			break;
		case cch_PASSED_AS_AN_ARGUMENT:
			cch = val.getSize();
			break;
	}
	switch(det_id)
	{
		case aw_READ_FROM_RECORD_IF_CCH_NOT_ZERO:
			if(0 == cch) break;
		case aw_READ_FROM_RECORD:
			unsigned char fHighByte;
			record >> fHighByte;
			fHighByte &= 1;
			is_wide = fHighByte != 0;
			struct_size += sizeof(fHighByte);
			break;
		case aw_WIDE:
			is_wide = true;
			break;
		case aw_ANSI:
			is_wide = false;
			break;
		case aw_READ_FROM_CCH:
			cchType cch_real = static_cast<cchType>(cch);
			is_wide = 0 != (cch_real & (1 << ((sizeof(cchType) * 8) - 1)));
			cch &= (static_cast<cchType>(-1) >> 1);
			break;
	}

	
	struct_size += (cch << (is_wide ? 1 : 0));

	val.load(record, cch, is_wide);
	val.setStructSize(struct_size); 
	
	return record;
}



template<class cchType, AW_DETERMINATION det_id, CCH_SOURCE cch_where>
CFRecord& operator<<(CFRecord& record, XLUnicodeString_T<cchType, det_id, cch_where>& val)
{
	size_t struct_size = 0;
	bool is_wide = val.getWideRecommendation();


	switch(cch_where)
	{
		case cch_READ_FROM_RECORD:
		{
			cchType cch_l = static_cast<cchType>(val.getSize()); // Just to conform size write.
			if(aw_READ_FROM_CCH == det_id)
			{
				SETBIT(cch_l, ((sizeof(cchType) * 8) - 1), is_wide); 
			}
			record << cch_l;
			struct_size += sizeof(cchType);
			break;
		}
		case cch_PASSED_AS_AN_ARGUMENT:
			// Means cch already stored outside
			break;
	}

	switch(det_id)
	{
		case aw_READ_FROM_RECORD_IF_CCH_NOT_ZERO:
			if(0 == val.getSize()) break;
		case aw_READ_FROM_RECORD:
		{
			unsigned char fHighByte = is_wide ? 1 : 0;
			record << fHighByte;
			struct_size += sizeof(fHighByte);
			break;
		}
		case aw_WIDE:
			is_wide = true;
			break;
		case aw_ANSI:
			is_wide = false;
			break;
		case aw_READ_FROM_CCH:
			// See cch_READ_FROM_RECORD case in the previous switch
			break;
	}
	struct_size += (val.getSize() << (is_wide ? 1 : 0));
	val.store(record, is_wide);
	val.setStructSize(struct_size); // The only usage is DXFFntD::load
	return record;
}



} // namespace XLS


namespace OSHARED
{

typedef XLS::XLUnicodeString_T<unsigned short, XLS::aw_READ_FROM_CCH, XLS::cch_READ_FROM_RECORD> PBString;
typedef XLS::XLUnicodeString_T<unsigned int, XLS::aw_WIDE, XLS::cch_READ_FROM_RECORD> HyperlinkString;

} // namespace OSHARED
