#pragma once

#include "Ptg.h"
#include "SerAr.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class PtgExtraArray : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgExtraArray)
public:
	PtgExtraArray();
	PtgExtraArray(const std::wstring& word);
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record); // No type info
	virtual void store(CFRecord& record); // No type info

	const std::wstring toString() const;
	void fromString(const std::wstring& str);

private:
	DColunByteU cols;
	DRw rows;
	std::vector<SerArPtr> array_;
};

typedef boost::shared_ptr<PtgExtraArray> PtgExtraArrayPtr;

} // namespace XLS

