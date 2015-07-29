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
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFSP)
public:
	OfficeArtFSP();
	XLS::BiffStructurePtr clone();

	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	// overriden
	const unsigned short GetInstanceToStore();

	unsigned short shape_id;

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
