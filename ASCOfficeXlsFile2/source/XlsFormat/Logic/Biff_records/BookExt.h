#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BookExt_Conditional11.h>
#include <Logic/Biff_structures/BookExt_Conditional12.h>

namespace XLS
{


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

	static const ElementType	type = typeBookExt;

//-----------------------------
	BIFF_DWORD cb;

	bool fDontAutoRecover;
	bool fHidePivotList;
	bool fFilterPrivacy;
	bool fEmbedFactoids;
	BIFF_BYTE mdFactoidDisplay;
	bool fSavedDuringRecovery;
	bool fCreatedViaMinimalSave;
	bool fOpenedViaDataRecovery;
	bool fOpenedViaSafeLoad;

	BookExt_Conditional11 grbit1;
	BookExt_Conditional12 grbit2;

		//if(cb > 20)
		//{
		//	//BO_ATTRIB_MARKUP_COMPLEX(grbit1)
		//}
		//if(cb > 21)
		//{
		//	//BO_ATTRIB_MARKUP_COMPLEX(grbit2)
		//}

};

} // namespace XLS

