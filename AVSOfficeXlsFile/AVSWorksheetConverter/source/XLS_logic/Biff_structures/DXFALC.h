#pragma once

#include "BiffStructTagged.h"

namespace XLS
{;

class CFRecord;

class DXFALC : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DXFALC)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	BYTE alc;
	bool fWrap;
	BYTE alcv;
	bool fJustLast;
	BYTE trot;
	BYTE cIndent;
	bool fShrinkToFit;
	bool fMergeCell;
	BYTE iReadingOrder;

	DWORD iIndent;
};

} // namespace XLS

