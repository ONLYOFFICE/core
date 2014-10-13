#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


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
private:
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

	ShortXLUnicodeString fontName;
	

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(dyHeight)
		BO_ATTRIB_MARKUP_ATTRIB(fItalic)
		BO_ATTRIB_MARKUP_ATTRIB(fStrikeOut)
		BO_ATTRIB_MARKUP_ATTRIB(fOutline)
		BO_ATTRIB_MARKUP_ATTRIB(fShadow)
		BO_ATTRIB_MARKUP_ATTRIB(fCondense)
		BO_ATTRIB_MARKUP_ATTRIB(fExtend)
		BO_ATTRIB_MARKUP_ATTRIB(icv)
		BO_ATTRIB_MARKUP_ATTRIB(bls)
		BO_ATTRIB_MARKUP_ATTRIB(sss)
		BO_ATTRIB_MARKUP_ATTRIB(uls)
		BO_ATTRIB_MARKUP_ATTRIB(bFamily)
		BO_ATTRIB_MARKUP_ATTRIB(bCharSet)
		BO_ATTRIB_MARKUP_ATTRIB(fontName)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

