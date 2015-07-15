#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/Cell.h>

namespace XLS
{;


class IXFCellMulBlankSpecial : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(IXFCellMulBlankSpecial)
public:
	BiffStructurePtr clone();

	//virtual void toXML(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record, const size_t num_cells);
//-----------------------------
	virtual void load(CFRecord& record) {};
	//virtual const bool fromXML(MSXML2::IXMLDOMElementPtr xml_tag) { return false; };
	virtual void store(CFRecord& record){};

//-----------------------------
	unsigned __int16 common_ixfe;
	std::vector<unsigned __int16> rgixfe;
};



// Logical representation of MulBlank record in BIFF8
class MulBlank: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MulBlank)
	BASE_OBJECT_DEFINE_CLASS_NAME(MulBlank)
public:
	MulBlank();
	~MulBlank();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	
	const long GetRow() const;

//-----------------------------
	Rw rw;
	Col colFirst;
	IXFCellMulBlankSpecial rgixfe;
	Col colLast;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(colFirst)
		//BO_ATTRIB_MARKUP_ATTRIB(colLast)
		//BO_ATTRIB_MARKUP_COMPLEX(rgixfe)
	//BO_ATTRIB_MARKUP_END

};



} // namespace XLS

