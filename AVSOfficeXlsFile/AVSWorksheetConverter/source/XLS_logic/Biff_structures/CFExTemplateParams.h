#pragma once

#include "BiffStructure.h"
//#include <XLS_logic/Biff_structures/Phs.h>
//#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

#pragma pack(1)

struct CFExFilterParams : public BiffStructure_NoVtbl
{
public:
	void toXML(MSXML2::IXMLDOMElementPtr parent);

private:
	bool fTop : 1;
	bool fPercent : 1;
	WORD iParam;

private:
	BYTE reserved2_1;
	DWORD reserved2_2;
	DWORD reserved2_3;
	DWORD reserved2_4;
};


struct CFExTextTemplateParams : public BiffStructure_NoVtbl
{
public:
	void toXML(MSXML2::IXMLDOMElementPtr parent);

private:
	WORD ctp;

private:
	WORD reserved_1;
	DWORD reserved_2;
	DWORD reserved_3;
	DWORD reserved_4;
};


struct CFExDateTemplateParams : public BiffStructure_NoVtbl
{
public:
	void toXML(MSXML2::IXMLDOMElementPtr parent);

private:
	WORD dateOp;

private:
	WORD reserved_1;
	DWORD reserved_2;
	DWORD reserved_3;
	DWORD reserved_4;
};


struct CFExAveragesTemplateParams : public BiffStructure_NoVtbl
{
public:
	void toXML(MSXML2::IXMLDOMElementPtr parent);

private:
	WORD iParam;

private:
	WORD reserved_1;
	DWORD reserved_2;
	DWORD reserved_3;
	DWORD reserved_4;
};


struct CFExDefaultTemplateParams : public BiffStructure_NoVtbl
{
public:
	void toXML(MSXML2::IXMLDOMElementPtr parent);

private:
	DWORD unused1;
	DWORD unused2;
	DWORD unused3;
	DWORD unused4;
};


class CFExTemplateParams : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CFExTemplateParams)
	virtual void toXML(MSXML2::IXMLDOMElementPtr parent) {};
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent) {return false;};
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent, const BYTE icfTemplate);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	union
	{
		CFExFilterParams filter;
		CFExTextTemplateParams text;
		CFExDateTemplateParams date;
		CFExAveragesTemplateParams averages;
		CFExDefaultTemplateParams defaults;
	} data;
};
#pragma pack()

} // namespace XLS

