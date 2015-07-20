
#include "Dimensions.h"
#include <Logic/Biff_structures/CellRangeRef.h>

#include <simple_xml_writer.h>

namespace XLS
{;

Dimensions::Dimensions()
{
}


Dimensions::~Dimensions()
{
}


BaseObjectPtr Dimensions::clone()
{
	return BaseObjectPtr(new Dimensions(*this));
}


void Dimensions::writeFields(CFRecord& record)
{
	if(!(ref_ == std::wstring (L"") || ref_ == std::wstring (L"A1")))
	{
		CellRangeRef ref_ref(ref_);
		rwMic = ref_ref.getRowFirst();
		rwMac = ref_ref.getRowLast() + 1;// zero-based index of the row after the last row in the sheet that contains a used cell
		colMic = static_cast<unsigned short>(ref_ref.getColumnFirst());
		colMac = static_cast<unsigned short>(ref_ref.getColumnLast()) + 1; // zero-based index of the column after the last column in the sheet that contains a used cell
	}
	else
	{
		rwMic = 0;
		rwMac = 0;
		colMic = 0;
		colMac = 0;
	}
	record << rwMic << rwMac << colMic << colMac;
	record.reserveNunBytes(2); // reserved
}


void Dimensions::readFields(CFRecord& record)
{
	record >> rwMic >> rwMac >> colMic >> colMac;
	record.skipNunBytes(2); // reserved
	
	if(rwMac.value() && colMac.value())
	{
		ref_ = static_cast<std::wstring >(CellRangeRef(CellRef(*rwMic.value(), *colMic.value(), true, true), CellRef(*rwMac.value() - 1, *colMac.value() - 1, true, true)).toString(false).c_str());
	}
	else
	{
		ref_ = std::wstring (L"");
	}
}

int Dimensions::serialize(std::wostream & stream)
{
	if (ref_.empty()) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"dimension")
		{
			CP_XML_ATTR(L"ref", ref_);
		}
	}
	return 0;
}
} // namespace XLS

