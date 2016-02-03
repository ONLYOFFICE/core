#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

#pragma pack(1)

struct CFExFilterParams : public BiffStructure_NoVtbl
{
public:
	//void toXML(BiffStructurePtr & parent);

private:
	bool fTop : 1;
	bool fPercent : 1;
	unsigned short iParam;

private:
	unsigned char reserved2_1;
	_UINT32 reserved2_2;
	_UINT32 reserved2_3;
	_UINT32 reserved2_4;
};


struct CFExTextTemplateParams : public BiffStructure_NoVtbl
{
public:
	//void toXML(BiffStructurePtr & parent);

private:
	unsigned short ctp;

private:
	_UINT16 reserved_1;
	_UINT32 reserved_2;
	_UINT32 reserved_3;
	_UINT32 reserved_4;
};


struct CFExDateTemplateParams : public BiffStructure_NoVtbl
{
public:
	//void toXML(BiffStructurePtr & parent);

private:
	unsigned short dateOp;

private:
	_UINT16 reserved_1;
	_UINT32 reserved_2;
	_UINT32 reserved_3;
	_UINT32 reserved_4;
};


struct CFExAveragesTemplateParams : public BiffStructure_NoVtbl
{
public:
	//void toXML(BiffStructurePtr & parent);

private:
	unsigned short iParam;

private:
	unsigned short reserved_1;
	_UINT32 reserved_2;
	_UINT32 reserved_3;
	_UINT32 reserved_4;
};


struct CFExDefaultTemplateParams : public BiffStructure_NoVtbl
{
public:
	//void toXML(BiffStructurePtr & parent);

private:
	_UINT32 unused1;
	_UINT32 unused2;
	_UINT32 unused3;
	_UINT32 unused4;
};


class CFExTemplateParams : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CFExTemplateParams)
	//virtual void toXML(BiffStructurePtr & parent) {};
	//virtual const bool fromXML(BiffStructurePtr & parent) {return false;};
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent, const unsigned char icfTemplate);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeCFExTemplateParams;

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

