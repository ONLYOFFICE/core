#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/DVParsedFormula.h>
#include <Logic/Biff_structures/SqRefU.h>

namespace XLS
{


// Logical representation of Dv record in BIFF8
class Dv: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Dv)
	BASE_OBJECT_DEFINE_CLASS_NAME(Dv)
public:
	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDv;

//-----------------------------
	unsigned char	valType;
	unsigned char	errStyle;
	bool fStrLookup;
	bool fAllowBlank;
	bool fSuppressCombo;
	unsigned char	mdImeMode;
	bool fShowInputMsg;
	bool fShowErrorMsg;
	unsigned char	typOperator;

	XLUnicodeString PromptTitle;
	XLUnicodeString ErrorTitle;
	XLUnicodeString Prompt;
	XLUnicodeString Error;
	DVParsedFormula formula1;
	DVParsedFormula formula2;
	SqRefU sqref;

};

} // namespace XLS

