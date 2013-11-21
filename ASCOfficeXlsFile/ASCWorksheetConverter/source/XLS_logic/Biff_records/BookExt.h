#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/BookExt_Conditional11.h>
#include <XLS_logic/Biff_structures/BookExt_Conditional12.h>

namespace XLS
{;


// Logical representation of BookExt record in BIFF8
class BookExt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BookExt)
	BASE_OBJECT_DEFINE_CLASS_NAME(BookExt)
public:
	BookExt();
	~BookExt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_DWORD cb;
	BIFF_BOOL fDontAutoRecover;
	BIFF_BOOL fHidePivotList;
	BIFF_BOOL fFilterPrivacy;
	BIFF_BOOL fEmbedFactoids;
	BIFF_BYTE mdFactoidDisplay;
	BIFF_BOOL fSavedDuringRecovery;
	BIFF_BOOL fCreatedViaMinimalSave;
	BIFF_BOOL fOpenedViaDataRecovery;
	BIFF_BOOL fOpenedViaSafeLoad;

	BookExt_Conditional11 grbit1;
	BookExt_Conditional12 grbit2;
public:
	BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(cb)
		BO_ATTRIB_MARKUP_ATTRIB(fDontAutoRecover)
		BO_ATTRIB_MARKUP_ATTRIB(fHidePivotList)
		BO_ATTRIB_MARKUP_ATTRIB(fFilterPrivacy)
		BO_ATTRIB_MARKUP_ATTRIB(fEmbedFactoids)
		BO_ATTRIB_MARKUP_ATTRIB(mdFactoidDisplay)
		BO_ATTRIB_MARKUP_ATTRIB(fSavedDuringRecovery)
		BO_ATTRIB_MARKUP_ATTRIB(fCreatedViaMinimalSave)
		BO_ATTRIB_MARKUP_ATTRIB(fOpenedViaDataRecovery)
		BO_ATTRIB_MARKUP_ATTRIB(fOpenedViaSafeLoad)

		if(cb > 20)
		{
			BO_ATTRIB_MARKUP_COMPLEX(grbit1)
		}
		if(cb > 21)
		{
			BO_ATTRIB_MARKUP_COMPLEX(grbit2)
		}
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

