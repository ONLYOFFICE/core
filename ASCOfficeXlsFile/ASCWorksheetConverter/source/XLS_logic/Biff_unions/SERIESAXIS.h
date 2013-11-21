#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SERIESAXIS union of records 
class SERIESAXIS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SERIESAXIS)
public:
	SERIESAXIS();
	~SERIESAXIS();

	BaseObjectPtr clone();

	virtual const bool loadContentRead(BinReaderProcessor& proc);
	virtual const bool loadContent(BinProcessor& proc);
	virtual void toXML(MSXML2::IXMLDOMElementPtr own_tag);

private:
	DWORD id;

};

} // namespace XLS

