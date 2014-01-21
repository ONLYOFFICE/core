#pragma once

#include "BiffRecordContinued.h"
#include <XLS_logic/Biff_structures/ControlInfo.h>
#include <XLS_logic/Biff_structures/FontIndex.h>
#include <XLS_logic/Biff_structures/ObjFmla.h>
#include <XLS_logic/Biff_structures/TxORuns.h>

namespace XLS
{;


// Logical representation of TxO record in BIFF8
class TxO: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(TxO)
	BASE_OBJECT_DEFINE_CLASS_NAME(TxO)
public:
	TxO();
	~TxO();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

public:
	BIFF_BYTE hAlignment;
	BIFF_BYTE vAlignment;
	BIFF_BYTE fLockText;
	BIFF_BYTE fJustLast;
	BIFF_BYTE fSecretEdit;
	BIFF_WORD rot;
	ControlInfo controlInfo;
	BIFF_WORD cchText;
	BIFF_WORD cbRuns;
	FontIndex ifntEmpty;
	ObjFmla fmla;
	TxORuns TxOruns;
	XLUnicodeStringNoCch commentText;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(hAlignment)
		BO_ATTRIB_MARKUP_ATTRIB(vAlignment)
		BO_ATTRIB_MARKUP_ATTRIB(fLockText)
		BO_ATTRIB_MARKUP_ATTRIB(fJustLast)
		BO_ATTRIB_MARKUP_ATTRIB(fSecretEdit)
		BO_ATTRIB_MARKUP_ATTRIB(rot)
		BO_ATTRIB_MARKUP_ATTRIB(cchText)
		BO_ATTRIB_MARKUP_ATTRIB(cbRuns)
		if (cchText == 0) {
			BO_ATTRIB_MARKUP_ATTRIB(ifntEmpty)
		}
		if ( cbRuns )
		{
			BO_ATTRIB_MARKUP_COMPLEX(TxOruns)
		}
		BO_ATTRIB_MARKUP_ATTRIB(commentText)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

