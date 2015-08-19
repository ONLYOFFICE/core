#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/BorderFillInfo.h>

namespace XLS
{


// Logical representation of Font record in BIFF8
class Font: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Font)
	BASE_OBJECT_DEFINE_CLASS_NAME(Font)
public:
	Font();
	~Font();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFont;

	int serialize				(std::wostream & stream);
	int serialize_properties	(std::wostream & stream, bool rPr = false);
	int serialize_rPr			(std::wostream & stream);

	void set_color_ext			(FillInfoExt & color_ext);

//-----------------------------
	BIFF_WORD dyHeight; // measured in twips (1/20 of of a printer's point)

	BIFF_BOOL fItalic;
	BIFF_BOOL fStrikeOut;
	BIFF_BOOL fOutline;
	BIFF_BOOL fShadow;
	BIFF_BOOL fCondense;
	BIFF_BOOL fExtend;

	BIFF_WORD icv;
	BIFF_WORD bls;
	BIFF_WORD sss;
	BIFF_BYTE uls;
	BIFF_BYTE bFamily;
	BIFF_BYTE bCharSet;

	FillInfoExt				color_ext;
	ShortXLUnicodeString	fontName;
	
};

} // namespace XLS

