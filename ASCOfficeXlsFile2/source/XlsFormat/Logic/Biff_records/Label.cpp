
#include "Label.h"

namespace XLS
{

Label::Label()
{
	isst_ = -1;
}


Label::~Label()
{
}


BaseObjectPtr Label::clone()
{
	return BaseObjectPtr(new Label(*this));
}


void Label::writeFields(CFRecord& record)
{
	record << cell << st;
}


void Label::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr pGlobalWorkbookInfoPtr = record.getGlobalWorkbookInfo();
	
	record >> cell >> st;

    isst_ = pGlobalWorkbookInfoPtr->startAddedSharedStrings + pGlobalWorkbookInfoPtr->arAddedSharedStrings.size() + 1;
	pGlobalWorkbookInfoPtr->arAddedSharedStrings.push_back(st.value());
}

int Label::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		int row = cell.rw;
			
		std::wstring ref = cell.getLocation().toString();// getColRowRef(i, row);
		CP_XML_NODE(L"c")
		{
			CP_XML_ATTR(L"r", ref);

			CP_XML_ATTR(L"t", L"s");
			
			CP_XML_NODE(L"v")
			{
				CP_XML_STREAM() << isst_;
			}
		}			
	}
	return 0;
}

} // namespace XLS

