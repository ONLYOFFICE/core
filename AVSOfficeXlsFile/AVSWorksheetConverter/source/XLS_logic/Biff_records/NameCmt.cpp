#include "stdafx.h"
#include "NameCmt.h"
#include <XLS_logic/Biff_structures/FrtHeader.h>

namespace XLS
{;

NameCmt::NameCmt()
{
}


NameCmt::NameCmt(const XLUnicodeStringNoCch lbl_name)
:	name(lbl_name)
{
}


NameCmt::~NameCmt()
{
}


BaseObjectPtr NameCmt::clone()
{
	return BaseObjectPtr(new NameCmt(*this));
}


void NameCmt::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_HeaderFooter);
	record << frtHeader;
	WORD cchName = static_cast<WORD>(name.getSize());
	WORD cchComment = static_cast<WORD>(comment.getSize());
	record << cchName << cchComment;
	record << name << comment;
}


void NameCmt::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_HeaderFooter);
	record >> frtHeader;
	WORD cchName;
	WORD cchComment;
	record >> cchName >> cchComment;
	name.setSize(cchName);
	comment.setSize(cchComment);
	record >> name >> comment;

}

} // namespace XLS

