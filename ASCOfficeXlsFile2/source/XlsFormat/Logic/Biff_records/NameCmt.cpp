
#include "NameCmt.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{

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
	unsigned short cchName = static_cast<unsigned short>(name.getSize());
	unsigned short cchComment = static_cast<unsigned short>(comment.getSize());
	record << cchName << cchComment;
	record << name << comment;
}


void NameCmt::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_HeaderFooter);
	record >> frtHeader;
	unsigned short cchName;
	unsigned short cchComment;
	record >> cchName >> cchComment;
	name.setSize(cchName);
	comment.setSize(cchComment);
	record >> name >> comment;

}

} // namespace XLS

