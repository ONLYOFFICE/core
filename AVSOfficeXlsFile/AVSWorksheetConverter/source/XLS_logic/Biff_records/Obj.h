#pragma once

#include "BiffRecordContinued.h"
#include <XLS_logic/Biff_structures/FtCmo.h>
#include <XLS_logic/Biff_structures/FtCf.h>
#include <XLS_logic/Biff_structures/FtPioGrbit.h>
#include <XLS_logic/Biff_structures/FtSbs.h>
#include <XLS_logic/Biff_structures/FtNts.h>
#include <XLS_logic/Biff_structures/FtMacro.h>
#include <XLS_logic/Biff_structures/FtPictFmla.h>
#include <XLS_logic/Biff_structures/ObjLinkFmla.h>
#include <XLS_logic/Biff_structures/FtCblsData.h>
#include <XLS_logic/Biff_structures/FtRboData.h>
#include <XLS_logic/Biff_structures/FtEdoData.h>
#include <XLS_logic/Biff_structures/FtLbsData.h>
#include <XLS_logic/Biff_structures/FtGboData.h>

namespace XLS
{;


// Logical representation of Obj record in BIFF8
class Obj: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Obj)
	BASE_OBJECT_DEFINE_CLASS_NAME(Obj)
public:
	Obj();
	~Obj();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	FtCmo cmo;
	FtCf pictFormat;
	FtPioGrbit pictFlags;
	FtSbs sbs;
	FtNts nts;
	FtMacro macro;
	FtPictFmla pictFmla;
	ObjLinkFmla linkFmla;
	FtCblsData checkBox;
	FtRboData radioButton;
	FtEdoData edit;
	FtLbsData list;
	FtGboData gbo;


public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(cmo)
		if(0x08 == cmo.ot)
		{
			BO_ATTRIB_MARKUP_COMPLEX(pictFormat)
			BO_ATTRIB_MARKUP_COMPLEX(pictFlags)
		}
		if(0x10 == cmo.ot || 0x11 == cmo.ot || 0x12 == cmo.ot || 0x14 == cmo.ot)
		{
			BO_ATTRIB_MARKUP_COMPLEX(sbs)
		}
		if(0x19 == cmo.ot)
		{
			BO_ATTRIB_MARKUP_COMPLEX(nts)
		}
		if(false) // TODO: Find out the condition
		{
			BO_ATTRIB_MARKUP_COMPLEX(macro)
		}
		if(0x08 == cmo.ot)
		{
			//BO_ATTRIB_MARKUP_COMPLEX(pictFmla)
		}
		if(0x0B == cmo.ot || 0x0C == cmo.ot || 0x10 == cmo.ot || 0x11 == cmo.ot || 0x12 == cmo.ot /*|| 0x14 == cmo.ot*/)
		{
			BO_ATTRIB_MARKUP_COMPLEX(linkFmla)
		}
		if(0x0B == cmo.ot || 0x0C == cmo.ot)
		{
			BO_ATTRIB_MARKUP_COMPLEX(checkBox)
		}
		if(0x0C == cmo.ot)
		{
			BO_ATTRIB_MARKUP_COMPLEX(radioButton)
		}
		if(0x0D == cmo.ot)
		{
			BO_ATTRIB_MARKUP_COMPLEX(edit)
		}
		if(0x12 == cmo.ot || 0x14 == cmo.ot)
		{
			BO_ATTRIB_MARKUP_COMPLEX(list)
		}
		if(0x13 == cmo.ot)
		{
			BO_ATTRIB_MARKUP_COMPLEX(gbo)
		}
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

