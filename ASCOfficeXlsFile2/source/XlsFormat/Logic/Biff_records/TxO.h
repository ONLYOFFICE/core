#pragma once

#include "BiffRecordContinued.h"
#include <Logic/Biff_structures/ControlInfo.h>
#include <Logic/Biff_structures/FontIndex.h>
#include <Logic/Biff_structures/ObjFmla.h>
#include <Logic/Biff_structures/TxORuns.h>
#include <Logic/Biff_records/MsoDrawing.h>

namespace XLS
{


// Logical representation of TxO record in BIFF8
class TxO: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(TxO)
	BASE_OBJECT_DEFINE_CLASS_NAME(TxO)
public:
	TxO(MsoDrawingPtr mso_drawing) : mso_drawing_(mso_drawing), fmla(false)
	{
		cbRuns		= 0;
		sp_enabled	= false;
	}
	~TxO();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeTxO;

	int serialize		(std::wostream & _stream);	
	int serialize_rPr	(std::wostream & _stream, int iFmt);

	GlobalWorkbookInfoPtr	pGlobalWorkbookInfoPtr;

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

	bool					sp_enabled;

	MsoDrawingPtr			mso_drawing_;

};

} // namespace XLS

