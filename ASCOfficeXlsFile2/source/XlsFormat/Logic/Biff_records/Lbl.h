#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/NameParsedFormula.h>

namespace XLS
{


// Logical representation of Lbl record in BIFF8
class Lbl: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Lbl)
	BASE_OBJECT_DEFINE_CLASS_NAME(Lbl)
public:
	Lbl();
	~Lbl();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeLbl;


	const XLUnicodeStringNoCch getName() const;

//-----------------------------
	BIFF_BOOL fHidden;
	BIFF_BOOL fFunc;
	BIFF_BOOL fOB;
	BIFF_BOOL fProc;
	BIFF_BOOL fCalcExp;
	BIFF_BOOL fBuiltin;
	BIFF_BYTE fGrp;
	BIFF_BOOL fPublished;
	BIFF_BOOL fWorkbookParam;
	BIFF_BYTE chKey;
	BIFF_WORD itab;
	XLUnicodeStringNoCch Name_bin;
	BIFF_BSTR Name;
	NameParsedFormula rgce;

};

} // namespace XLS

