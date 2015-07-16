#pragma once

#include "BiffRecord.h"

namespace XLS
{;

class BopPopCustomPiesIndices : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RGISeriesListSpecial)
public:
	BiffStructurePtr clone();

	//virtual void toXML(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual const bool fromXML(MSXML2::IXMLDOMElementPtr xml_tag){return false;}
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record){}

//-----------------------------
	std::vector<unsigned short> pie_indices;
};



// Logical representation of BopPopCustom record in BIFF8
class BopPopCustom: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BopPopCustom)
	BASE_OBJECT_DEFINE_CLASS_NAME(BopPopCustom)
public:
	BopPopCustom();
	~BopPopCustom();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeBopPopCustom;

//-----------------------------
	BopPopCustomPiesIndices rggrbit;

};

} // namespace XLS

