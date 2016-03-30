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
	bool fHidden;
	bool fFunc;
	bool fOB;
	bool fProc;
	bool fCalcExp;
	bool fBuiltin;
	unsigned char	fGrp;
	bool fPublished;
	bool fWorkbookParam;
	
	unsigned char	chKey;
	_UINT16 itab;
	XLUnicodeStringNoCch Name_bin;
	BIFF_BSTR Name;
	NameParsedFormula rgce;

};

} // namespace XLS

