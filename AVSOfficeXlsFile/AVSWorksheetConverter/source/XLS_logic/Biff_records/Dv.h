#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/BiffString.h>
#include <XLS_logic/Biff_structures/DVParsedFormula.h>
#include <XLS_logic/Biff_structures/SqRefU.h>

namespace XLS
{;


// Logical representation of Dv record in BIFF8
class Dv: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Dv)
	BASE_OBJECT_DEFINE_CLASS_NAME(Dv)
public:
	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BYTE valType;
	BIFF_BYTE errStyle;
	BIFF_BOOL fStrLookup;
	BIFF_BOOL fAllowBlank;
	BIFF_BOOL fSuppressCombo;
	BIFF_BYTE mdImeMode;
	BIFF_BOOL fShowInputMsg;
	BIFF_BOOL fShowErrorMsg;
	BIFF_BYTE typOperator;

	XLUnicodeString PromptTitle;
	XLUnicodeString ErrorTitle;
	XLUnicodeString Prompt;
	XLUnicodeString Error;
	DVParsedFormula formula1;
	DVParsedFormula formula2;
	SqRefU sqref;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(valType)
		BO_ATTRIB_MARKUP_ATTRIB(errStyle)
		BO_ATTRIB_MARKUP_ATTRIB(fStrLookup)
		BO_ATTRIB_MARKUP_ATTRIB(fAllowBlank)
		BO_ATTRIB_MARKUP_ATTRIB(fSuppressCombo)
		BO_ATTRIB_MARKUP_ATTRIB(mdImeMode)
		BO_ATTRIB_MARKUP_ATTRIB(fShowInputMsg)
		BO_ATTRIB_MARKUP_ATTRIB(fShowErrorMsg)
		BO_ATTRIB_MARKUP_ATTRIB(typOperator)

		BO_ATTRIB_MARKUP_ATTRIB(PromptTitle)
		BO_ATTRIB_MARKUP_ATTRIB(ErrorTitle)
		BO_ATTRIB_MARKUP_ATTRIB(Prompt)
		BO_ATTRIB_MARKUP_ATTRIB(Error)
		BO_ATTRIB_MARKUP_COMPLEX(formula1)
		BO_ATTRIB_MARKUP_COMPLEX(formula2)
		BO_ATTRIB_MARKUP_COMPLEX(formula2)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

