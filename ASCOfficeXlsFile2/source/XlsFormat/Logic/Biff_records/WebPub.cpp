
#include "WebPub.h"

namespace XLS
{

WebPub::WebPub()
{
}


WebPub::~WebPub()
{
}


BaseObjectPtr WebPub::clone()
{
	return BaseObjectPtr(new WebPub(*this));
}


void WebPub::writeFields(CFRecord& record)
{
#pragma message("####################### WebPub record is not implemented")
	Log::error("WebPub record is not implemented.");
	//record << some_value;
}


void WebPub::readFields(CFRecord& record)
{
#pragma message("####################### WebPub record is not implemented")
	Log::error("WebPub record is not implemented.");

	//record >> some_value;
}

} // namespace XLS

