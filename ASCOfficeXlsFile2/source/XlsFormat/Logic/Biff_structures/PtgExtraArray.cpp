
#include "PtgExtraArray.h"
#include "SyntaxPtg.h"
#include <Binary/CFRecord.h>
//#include <Exception/UnexpectedProgramPath.h>
#include "SerBool.h"
#include "SerErr.h"
#include "SerNil.h"
#include "SerNum.h"
#include "SerStr.h"

namespace XLS
{


PtgExtraArray::PtgExtraArray()
{
}


PtgExtraArray::PtgExtraArray(const std::wstring& word)
{
	fromString(word);
}


BiffStructurePtr PtgExtraArray::clone()
{
	return BiffStructurePtr(new PtgExtraArray(*this));
}



void PtgExtraArray::store(CFRecord& record)
{
	record << cols << rows;
	for(std::vector<SerArPtr>::const_iterator it = array_.begin(), itEnd = array_.end(); it != itEnd; ++it)
	{
		record << **it;
	}
}


void PtgExtraArray::load(CFRecord& record)
{
	record >> cols >> rows;
	for(int i = 0; i < (cols + 1) * (rows + 1); ++i)
	{
		if (record.getRdPtr() >= record.getDataSize()) 
			break;
		unsigned char rec_type;
		record >> rec_type;
		SerArPtr ser(SerAr::createSerAr(rec_type));
		record >> *ser;
		array_.push_back(ser);
	}
}


const std::wstring PtgExtraArray::toString() const
{
	std::wstring ret_val;
	unsigned char col_cnt = cols + 1;
	for(std::vector<SerArPtr>::const_iterator it = array_.begin(), itEnd = --array_.end(); it != itEnd; ++it)
	{
		ret_val += (*it)->toString();
		if(--col_cnt)
		{
			ret_val += L',';
		}
		else
		{
			ret_val += L';';
			col_cnt = cols + 1;
		}
	}
	ret_val += array_.back()->toString();
	return ret_val;
}


void PtgExtraArray::fromString(const std::wstring& str)
{
	if(str.length() <= 2)
	{
		throw;// EXCEPT::LE::WhatIsTheFuck("Unknown PtgExtraArray format extracted from formula.", __FUNCTION__);

	}
	std::wstring::const_iterator first = str.begin() + 1;
	std::wstring::const_iterator last = str.end() - 1;

	cols = 0;
	rows = 0;
	while(first != last)
	{
		std::wstring operand_str;
		if(SyntaxPtg::extract_PtgNum(first, last, operand_str))
		{
			array_.push_back(SerArPtr(new SerNum(operand_str)));
		}
		else if(SyntaxPtg::extract_PtgBool(first, last, operand_str))
		{
			array_.push_back(SerArPtr(new SerBool(operand_str)));
		}
		else if(SyntaxPtg::extract_PtgErr(first, last, operand_str))
		{
			array_.push_back(SerArPtr(new SerErr(operand_str)));
		}
		else if(SyntaxPtg::extract_PtgStr(first, last, operand_str))
		{
			array_.push_back(SerArPtr(new SerStr(operand_str)));
		}
		else if(SyntaxPtg::extract_comma(first, last))
		{
			++cols;
		}
		else if(SyntaxPtg::extract_semicolon(first, last)) // extract semicolon
		{
			++rows;
			cols = 0;
		}
		else
		{
			throw;// EXCEPT::LE::WhatIsTheFuck("Unknown operand format in PtgArray.", __FUNCTION__);
		}

	}

}


} // namespace XLS

