#pragma once

#include "BiffRecordContinued.h"
#include <Logic/Biff_structures/ControlInfo.h>
#include <Logic/Biff_structures/FontIndex.h>
#include <Logic/Biff_structures/ObjFmla.h>
#include <Logic/Biff_structures/TxORuns.h>

namespace ODRAW
{
	class OfficeArtRecord;
	typedef boost::shared_ptr<OfficeArtRecord> OfficeArtRecordPtr;
}

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

	static const ElementType	type = typeTxO;

	BIFF_BYTE				hAlignment;
	BIFF_BYTE				vAlignment;
	BIFF_BYTE				fLockText;
	BIFF_BYTE				fJustLast;
	BIFF_BYTE				fSecretEdit;
	BIFF_WORD				rot;
	ControlInfo				controlInfo;
	BIFF_WORD				cchText;
	BIFF_WORD				cbRuns;
	FontIndex				ifntEmpty;
	ObjFmla					fmla;
	TxORuns					TxOruns;
	XLUnicodeStringNoCch	commentText;

	ODRAW::OfficeArtRecordPtr		m_OfficeArtSpContainer; 

};

} // namespace XLS

