#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/AddinUdf.h>
#include <Logic/Biff_structures/ExternDdeLinkNoOper.h>
#include <Logic/Biff_structures/ExternOleDdeLink.h>
#include <Logic/Biff_structures/ExternDocName.h>
//#include <Exception/NotImplemented.h>

namespace XLS
{


// Logical representation of ExternName record in BIFF8
class ExternName: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ExternName)
	BASE_OBJECT_DEFINE_CLASS_NAME(ExternName)
public:
	ExternName(const unsigned short supporting_link_type);
	~ExternName();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeExternName;

//-----------------------------
	bool fBuiltIn;
	bool fWantAdvise;
	bool fWantPict;
	bool fOle;
	bool fOleLink;
	_UINT16 cf;
	bool fIcon;
	BiffStructurePtr body;

//-----------------------------
	_UINT16 supbook_cch;

	//if(0x3A01 == supbook_cch)
	//{
	//	if(!body)
	//	{
	//		body = BiffStructurePtr(new AddinUdf);
	//	}
	//	//BO_ATTRIB_MARKUP_COMPLEX(*body)
	//}
	//else
	//{
	//	throw;// EXCEPT::LE::NotImplemented(__FUNCTION__);
	//}

};

typedef boost::shared_ptr<ExternName> ExternNamePtr;

} // namespace XLS

