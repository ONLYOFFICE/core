#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/RRD.h>
#include <XLS_logic/Biff_structures/BiffString.h>
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class NoteRR : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(NoteRR)
public:
	NoteRR();
	NoteRR(CFRecord& record);
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	RRD rrd;
	bool bitfDelNote;
	bool bitfAddNote;
	RwU row;
	ColU col;
	bool fShow;
	bool fRwHidden;
	bool fColHidden;
	GUID guid;
	DWORD ichEnd;
	DWORD cchNote;
	XLUnicodeString stAuthor;



};

} // namespace XLS

