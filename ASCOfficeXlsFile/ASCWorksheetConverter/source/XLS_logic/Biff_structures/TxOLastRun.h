#pragma once

#include <XLS_logic/Biff_records/BiffRecord.h>
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class TxOLastRun: public BiffStructTagged
{	
	BASE_OBJECT_DEFINE_CLASS_NAME(TxOLastRun)
public:
	BiffStructurePtr clone();

	TxOLastRun();
	~TxOLastRun();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:	
	BIFF_WORD cchText;
};

typedef boost::shared_ptr<TxOLastRun> TxOLastRunPtr;

} // namespace XLS