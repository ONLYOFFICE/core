#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/NameParsedFormula.h>

namespace XLS
{;


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

	const XLUnicodeStringNoCch getName() const;

private:
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

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fHidden)
		BO_ATTRIB_MARKUP_ATTRIB(fFunc)
		BO_ATTRIB_MARKUP_ATTRIB(fOB)
		BO_ATTRIB_MARKUP_ATTRIB(fProc)
		BO_ATTRIB_MARKUP_ATTRIB(fCalcExp)
		BO_ATTRIB_MARKUP_ATTRIB(fBuiltin)
		BO_ATTRIB_MARKUP_ATTRIB(fGrp)
		BO_ATTRIB_MARKUP_ATTRIB(fPublished)
		BO_ATTRIB_MARKUP_ATTRIB(fWorkbookParam)
		BO_ATTRIB_MARKUP_ATTRIB(chKey)
		BO_ATTRIB_MARKUP_ATTRIB(itab)
		BO_ATTRIB_MARKUP_ATTRIB(Name)
		BO_ATTRIB_MARKUP_COMPLEX(rgce)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

