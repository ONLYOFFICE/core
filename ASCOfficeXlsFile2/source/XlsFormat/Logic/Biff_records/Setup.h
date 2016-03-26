#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Setup record in BIFF8
class Setup: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Setup)
	BASE_OBJECT_DEFINE_CLASS_NAME(Setup)
public:
	Setup();
	~Setup();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSetup;

//-----------------------------
	_UINT16 iPaperSize;
	_UINT16 iScale;
	_INT16 iPageStart;
	_UINT16 iFitWidth;
	_UINT16 iFitHeight;

	bool fLeftToRight;
	bool fPortrait;
	bool fNoPls;
	bool fNoColor;
	bool fDraft;
	bool fNotes;
	bool fNoOrient;
	bool fUsePage;
	bool fEndNotes;
	unsigned char	iErrors;

	_UINT16 iRes;
	_UINT16 iVRes;
	BIFF_DOUBLE numHdr;
	BIFF_DOUBLE numFtr;
	_UINT16 iCopies;

};

} // namespace XLS

