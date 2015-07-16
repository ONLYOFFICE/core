#include "precompiled_xls.h"
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
	unsigned short flags = 0;
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
	unsigned char cch = static_cast<std::wstring >(Name).length();
	record << cch;
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cce*/);
	record.reserveNunBytes(2);
	record << itab;
	record.reserveNunBytes(4);

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
		
		unsigned char index;
		if(Name == std::wstring (L"_xlnm.Consolidate_Area"))
		{
			index = 0;
		}
		else if(Name == std::wstring (L"_xlnm.Auto_Open"))
		{
			index = 1;
		}
		else if(Name == std::wstring (L"_xlnm.Auto_Close"))
		{
			index = 2;
		}
		else if(Name == std::wstring (L"_xlnm.Extract"))
		{
			index = 3;
		}
		else if(Name == std::wstring (L"_xlnm.Database"))
		{
			index = 4;
		}
		else if(Name == std::wstring (L"_xlnm.Criteria"))
		{
			index = 5;
		}
		else if(Name == std::wstring (L"_xlnm.Print_Area"))
		{
			index = 6;
		}
		else if(Name == std::wstring (L"_xlnm.Print_Titles"))
		{
			index = 7;
		}
		else if(Name == std::wstring (L"_xlnm.Recorder"))
		{
			index = 8;
		}
		else if(Name == std::wstring (L"_xlnm.Data_Form"))
		{
			index = 9;
		}
		else if(Name == std::wstring (L"_xlnm.Auto_Activate"))
		{
			index = 10;
		}
		else if(Name == std::wstring (L"_xlnm.Auto_Deactivate"))
		{
			index = 11;
		}
		else if(Name == std::wstring (L"_xlnm.Sheet_Title"))
		{
			index = 12;
		}
		else if(Name == std::wstring (L"_xlnm._FilterDatabase"))
		{
			index = 13;
		}
		else
		{
			throw;// EXCEPT::RT::WrongBiffRecord(L"Unsupported value of built-in name.", record.getTypeString());
		}
				
		// fix
		Name_bin = Name;
		(static_cast<std::wstring>(Name_bin)).at(0) = index;
		record << Name_bin;
		
	}
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_Lbl);
}


void Lbl::readFields(CFRecord& record)
{
	unsigned short flags;
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
	unsigned char cch;
	unsigned short cce;
	record >> cch >> cce;
	record.skipNunBytes(2);
	record >> itab;
	record.skipNunBytes(4);
	Name_bin.setSize(cch); // this is to process built-in string values
	record >> Name_bin;
	rgce.load(record, cce);

	if(!fBuiltin)
	{
		Name = Name_bin.getEscaped_ST_Xstring();
	}
	else
	{
		unsigned char index = (static_cast<std::wstring>(Name_bin)).c_str()[0];
		switch(index)
		{
			case 0x00:
				Name =  (L"_xlnm.Consolidate_Area");
				break;
			case 0x01:
				Name =  (L"_xlnm.Auto_Open");
				break;
			case 0x02:
				Name =  (L"_xlnm.Auto_Close");
				break;
			case 0x03:
				Name =  (L"_xlnm.Extract");
				break;
			case 0x04:
				Name =  (L"_xlnm.Database");
				break;
			case 0x05:
				Name =  (L"_xlnm.Criteria");
				break;
			case 0x06:
				Name =  (L"_xlnm.Print_Area");
				break;
			case 0x07:
				Name =  (L"_xlnm.Print_Titles");
				break;
			case 0x08:
				Name =  (L"_xlnm.Recorder");
				break;
			case 0x09:
				Name =  (L"_xlnm.Data_Form");
				break;
			case 0x0A:
				Name =  (L"_xlnm.Auto_Activate");
				break;
			case 0x0B:
				Name =  (L"_xlnm.Auto_Deactivate");
				break;
			case 0x0C:
				Name =  (L"_xlnm.Sheet_Title");
				break;
			case 0x0D:
				Name =  (L"_xlnm._FilterDatabase");
				break;
			default:
				throw;// EXCEPT::RT::WrongBiffRecord(L"Unsupported value of built-in name.", record.getTypeString());
		}
	}
}

const XLUnicodeStringNoCch Lbl::getName() const
{
	return Name_bin;
}

} // namespace XLS

