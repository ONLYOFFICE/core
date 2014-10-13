#pragma once

#include "OfficeArtRecord.h"
#include "OfficeArtFRIT.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtFSP : public OfficeArtRecord
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtFSP)
public:
	OfficeArtFSP();
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	// overriden
	const unsigned __int16 GetInstanceToStore();

private:
	unsigned __int16 shape_id;

	unsigned int spid;

	bool fGroup;
	bool fChild;
	bool fPatriarch;
	bool fDeleted;
	bool fOleShape;
	bool fHaveMaster;
	bool fFlipH;
	bool fFlipV;
	bool fConnector;
	bool fHaveAnchor;
	bool fBackground;
	bool fHaveSpt;
};

typedef boost::shared_ptr<OfficeArtFSP> OfficeArtFSPPtr;


} // namespace XLS
