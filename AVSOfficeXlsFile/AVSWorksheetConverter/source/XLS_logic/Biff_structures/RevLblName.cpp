#include "stdafx.h"
#include "RevLblName.h"
#include <XLS_bin/CFRecord.h>
#include <Exception/WrongBiffRecord.h>

namespace XLS
{;


RevLblName::RevLblName()
{
}


RevLblName::RevLblName(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RevLblName::clone()
{
	return BiffStructurePtr(new RevLblName(*this));
}


void RevLblName::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	parent->setAttribute(L"name", st); 
}


const bool RevLblName::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### RevLblName record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! RevLblName record has no BiffStructure::fromXML() implemented.");
	return false;
}


void RevLblName::store(CFRecord& record)
{
#pragma message("####################### RevLblName record has no BiffStructure::store() implemented")
	Log::error(" Error!!! RevLblName record has no BiffStructure::store() implemented.");
	//record << something;
}


void RevLblName::load(CFRecord& record)
{
	XLNameUnicodeString st_raw;
	BYTE iBuiltin;
	record >> iBuiltin >> st_raw;
	switch(iBuiltin)
	{
		case 0x00:
			st = static_cast<_bstr_t>(st_raw);
			break;
		case 0x01:
			st = L"_xlnm.Consolidate_Area";
			break;
		case 0x02:
			st = L"_xlnm.Auto_Open";
			break;
		case 0x03:
			st = L"_xlnm.Auto_Close";
			break;
		case 0x04:
			st = L"_xlnm.Extract";
			break;
		case 0x05:
			st = L"_xlnm.Database";
			break;
		case 0x06:
			st = L"_xlnm.Criteria";
			break;
		case 0x07:
			st = L"_xlnm.Print_Area";
			break;
		case 0x08:
			st = L"_xlnm.Print_Titles";
			break;
		case 0x09:
			st = L"_xlnm.Recorder";
			break;
		case 0x0A:
			st = L"_xlnm.Data_Form";
			break;
		case 0x0B:
			st = L"_xlnm.Auto_Activate";
			break;
		case 0x0C:
			st = L"_xlnm.Auto_Deactivate";
			break;
		case 0x0D:
			st = L"_xlnm.Sheet_Title";
			break;
		case 0x0E:
			st = L"_xlnm._FilterDatabase";
			break;
		default:
			throw EXCEPT::RT::WrongBiffRecord("Unsupported value of built-in name.", record.getTypeString());
	}
}


const std::wstring RevLblName::toString() const
{
	return static_cast<wchar_t*>(st);
}


} // namespace XLS

