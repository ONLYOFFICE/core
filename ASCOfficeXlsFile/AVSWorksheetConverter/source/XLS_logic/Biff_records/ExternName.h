#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/AddinUdf.h>
#include <XLS_logic/Biff_structures/ExternDdeLinkNoOper.h>
#include <XLS_logic/Biff_structures/ExternOleDdeLink.h>
#include <XLS_logic/Biff_structures/ExternDocName.h>
#include <Exception/NotImplemented.h>

namespace XLS
{;


// Logical representation of ExternName record in BIFF8
class ExternName: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ExternName)
	BASE_OBJECT_DEFINE_CLASS_NAME(ExternName)
public:
	ExternName(const WORD supporting_link_type);
	~ExternName();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BOOL fBuiltIn;
	BIFF_BOOL fWantAdvise;
	BIFF_BOOL fWantPict;
	BIFF_BOOL fOle;
	BIFF_BOOL fOleLink;
	BIFF_WORD cf;
	BIFF_BOOL fIcon;
	BiffStructurePtr body;

private:
	BIFF_WORD supbook_cch;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fBuiltIn)
		BO_ATTRIB_MARKUP_ATTRIB(fWantAdvise)
		BO_ATTRIB_MARKUP_ATTRIB(fWantPict)
		BO_ATTRIB_MARKUP_ATTRIB(fOle)
		BO_ATTRIB_MARKUP_ATTRIB(fOleLink)
		BO_ATTRIB_MARKUP_ATTRIB(cf)
		BO_ATTRIB_MARKUP_ATTRIB(fIcon)
		if(0x3A01 == supbook_cch)
		{
			if(!body)
			{
				body = BiffStructurePtr(new AddinUdf);
			}
			BO_ATTRIB_MARKUP_COMPLEX(*body)
		}
		else
		{
			throw EXCEPT::LE::NotImplemented(__FUNCTION__);
		}
	BO_ATTRIB_MARKUP_END

};

typedef boost::shared_ptr<ExternName> ExternNamePtr;

} // namespace XLS

