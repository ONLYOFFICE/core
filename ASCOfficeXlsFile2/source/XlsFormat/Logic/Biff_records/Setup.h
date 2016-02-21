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
	BIFF_WORD iPaperSize;
	BIFF_WORD iScale;
	BIFF_SHORT iPageStart;
	BIFF_WORD iFitWidth;
	BIFF_WORD iFitHeight;

	bool fLeftToRight;
	bool fPortrait;
	bool fNoPls;
	bool fNoColor;
	bool fDraft;
	bool fNotes;
	bool fNoOrient;
	bool fUsePage;
	bool fEndNotes;
	BIFF_BYTE iErrors;

	BIFF_WORD iRes;
	BIFF_WORD iVRes;
	BIFF_DOUBLE numHdr;
	BIFF_DOUBLE numFtr;
	BIFF_WORD iCopies;

};

} // namespace XLS

