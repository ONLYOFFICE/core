#pragma once

#include "BiffRecord.h"

namespace XLS
{;


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

	BIFF_BOOL fLeftToRight;
	BIFF_BOOL fPortrait;
	BIFF_BOOL fNoPls;
	BIFF_BOOL fNoColor;
	BIFF_BOOL fDraft;
	BIFF_BOOL fNotes;
	BIFF_BOOL fNoOrient;
	BIFF_BOOL fUsePage;
	BIFF_BOOL fEndNotes;
	BIFF_BYTE iErrors;

	BIFF_WORD iRes;
	BIFF_WORD iVRes;
	BIFF_DOUBLE numHdr;
	BIFF_DOUBLE numFtr;
	BIFF_WORD iCopies;

public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fNoPls)
		//if(!fNoPls)
		//{
		//	//BO_ATTRIB_MARKUP_ATTRIB(iPaperSize)
		//	//BO_ATTRIB_MARKUP_ATTRIB(iScale)
		//	//BO_ATTRIB_MARKUP_ATTRIB(iRes)
		//	//BO_ATTRIB_MARKUP_ATTRIB(iVRes)
		//	//BO_ATTRIB_MARKUP_ATTRIB(iCopies)
		//	//BO_ATTRIB_MARKUP_ATTRIB(fNoOrient)
		//	if(!fNoOrient)
		//	{
		//		//BO_ATTRIB_MARKUP_ATTRIB(fPortrait)
		//	}
		//}
		////BO_ATTRIB_MARKUP_ATTRIB(fUsePage)
		//if(fUsePage)
		//{
		//	//BO_ATTRIB_MARKUP_ATTRIB(iPageStart)
		//}
		//BO_ATTRIB_MARKUP_ATTRIB(iFitWidth)
		//BO_ATTRIB_MARKUP_ATTRIB(iFitHeight)

		//BO_ATTRIB_MARKUP_ATTRIB(fLeftToRight)
		//BO_ATTRIB_MARKUP_ATTRIB(fNoColor)
		//BO_ATTRIB_MARKUP_ATTRIB(fDraft)
		//BO_ATTRIB_MARKUP_ATTRIB(fNotes)
		//BO_ATTRIB_MARKUP_ATTRIB(fEndNotes)

		//BO_ATTRIB_MARKUP_ATTRIB(iErrors)

		//BO_ATTRIB_MARKUP_ATTRIB(numHdr)
		//BO_ATTRIB_MARKUP_ATTRIB(numFtr)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

