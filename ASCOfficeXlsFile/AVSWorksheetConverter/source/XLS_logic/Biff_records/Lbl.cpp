#include "stdafx.h"
#include "Lbl.h"

namespace XLS
{;

Lbl::Lbl()
:	rgce(false)
{
}


Lbl::~Lbl()
{
}


BaseObjectPtr Lbl::clone()
{
	return BaseObjectPtr(new Lbl(*this));
}


void Lbl::writeFields(CFRecord& record)
{
	WORD flags = 0;
	SETBIT(flags, 0, fHidden);	
	SETBIT(flags, 1, fFunc);
	SETBIT(flags, 2, fOB);
	SETBIT(flags, 3, fProc);
	SETBIT(flags, 4, fCalcExp);
	SETBIT(flags, 5, fBuiltin);
	SETBITS(flags, 6, 11, fGrp);
	SETBIT(flags, 13, fPublished);
	SETBIT(flags, 14, fWorkbookParam);

	record << flags << chKey;
	BYTE cch = static_cast<_bstr_t>(Name).length();
	record << cch;
	record.registerDelayedDataReceiver(NULL, sizeof(WORD)/*cce*/);
	record.reserveNBytes(2);
	record << itab;
	record.reserveNBytes(4);

	if(!fBuiltin)
	{
		Name_bin = Name;
		record << Name_bin;
	}
	else
	{		
		//Name_bin.setSize(1); // this is to process built-in string values		

		// fix
		Name_bin.setSize(cch); // this is to process built-in string values		
		
		BYTE index;
		if(_bstr_t("_xlnm.Consolidate_Area") == Name)
		{
			index = 0;
		}
		else if(_bstr_t("_xlnm.Auto_Open") == Name)
		{
			index = 1;
		}
		else if(_bstr_t("_xlnm.Auto_Close") == Name)
		{
			index = 2;
		}
		else if(_bstr_t("_xlnm.Extract") == Name)
		{
			index = 3;
		}
		else if(_bstr_t("_xlnm.Database") == Name)
		{
			index = 4;
		}
		else if(_bstr_t("_xlnm.Criteria") == Name)
		{
			index = 5;
		}
		else if(_bstr_t("_xlnm.Print_Area") == Name)
		{
			index = 6;
		}
		else if(_bstr_t("_xlnm.Print_Titles") == Name)
		{
			index = 7;
		}
		else if(_bstr_t("_xlnm.Recorder") == Name)
		{
			index = 8;
		}
		else if(_bstr_t("_xlnm.Data_Form") == Name)
		{
			index = 9;
		}
		else if(_bstr_t("_xlnm.Auto_Activate") == Name)
		{
			index = 10;
		}
		else if(_bstr_t("_xlnm.Auto_Deactivate") == Name)
		{
			index = 11;
		}
		else if(_bstr_t("_xlnm.Sheet_Title") == Name)
		{
			index = 12;
		}
		else if(_bstr_t("_xlnm._FilterDatabase") == Name)
		{
			index = 13;
		}
		else
		{
			throw EXCEPT::RT::WrongBiffRecord("Unsupported value of built-in name.", record.getTypeString());
		}
				
		// fix
		Name_bin = Name;
		(static_cast<char*>(static_cast<_bstr_t>(Name_bin)))[0] = index;
		record << Name_bin;
		
	}
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_Lbl);
}


void Lbl::readFields(CFRecord& record)
{
	WORD flags;
	record >> flags >> chKey;
	fHidden = GETBIT(flags, 0);
	fFunc = GETBIT(flags, 1);
	fOB = GETBIT(flags, 2);
	fProc = GETBIT(flags, 3);
	fCalcExp = GETBIT(flags, 4);
	fBuiltin = GETBIT(flags, 5);
	fGrp = GETBITS(flags, 6, 11);
	fPublished = GETBIT(flags, 13);
	fWorkbookParam = GETBIT(flags, 14);
	BYTE cch;
	WORD cce;
	record >> cch >> cce;
	record.skipNBytes(2);
	record >> itab;
	record.skipNBytes(4);
	Name_bin.setSize(cch); // this is to process built-in string values
	record >> Name_bin;
	rgce.load(record, cce);

	if(!fBuiltin)
	{
		Name = Name_bin.getEscaped_ST_Xstring();
	}
	else
	{
		BYTE index = (static_cast<char*>(static_cast<_bstr_t>(Name_bin)))[0];
		switch(index)
		{
			case 0x00:
				Name = _bstr_t("_xlnm.Consolidate_Area");
				break;
			case 0x01:
				Name = _bstr_t("_xlnm.Auto_Open");
				break;
			case 0x02:
				Name = _bstr_t("_xlnm.Auto_Close");
				break;
			case 0x03:
				Name = _bstr_t("_xlnm.Extract");
				break;
			case 0x04:
				Name = _bstr_t("_xlnm.Database");
				break;
			case 0x05:
				Name = _bstr_t("_xlnm.Criteria");
				break;
			case 0x06:
				Name = _bstr_t("_xlnm.Print_Area");
				break;
			case 0x07:
				Name = _bstr_t("_xlnm.Print_Titles");
				break;
			case 0x08:
				Name = _bstr_t("_xlnm.Recorder");
				break;
			case 0x09:
				Name = _bstr_t("_xlnm.Data_Form");
				break;
			case 0x0A:
				Name = _bstr_t("_xlnm.Auto_Activate");
				break;
			case 0x0B:
				Name = _bstr_t("_xlnm.Auto_Deactivate");
				break;
			case 0x0C:
				Name = _bstr_t("_xlnm.Sheet_Title");
				break;
			case 0x0D:
				Name = _bstr_t("_xlnm._FilterDatabase");
				break;
			default:
				throw EXCEPT::RT::WrongBiffRecord("Unsupported value of built-in name.", record.getTypeString());
		}
	}
}

const XLUnicodeStringNoCch Lbl::getName() const
{
	return Name_bin;
}

} // namespace XLS

