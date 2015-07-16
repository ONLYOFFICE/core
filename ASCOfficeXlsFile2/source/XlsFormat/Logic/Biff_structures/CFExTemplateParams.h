#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

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
	unsigned int reserved2_2;
	unsigned int reserved2_3;
	unsigned int reserved2_4;
};


struct CFExTextTemplateParams : public BiffStructure_NoVtbl
{
public:
	//void toXML(BiffStructurePtr & parent);

private:
	unsigned short ctp;

private:
	unsigned short reserved_1;
	unsigned int reserved_2;
	unsigned int reserved_3;
	unsigned int reserved_4;
};


struct CFExDateTemplateParams : public BiffStructure_NoVtbl
{
public:
	//void toXML(BiffStructurePtr & parent);

private:
	unsigned short dateOp;

private:
	unsigned short reserved_1;
	unsigned int reserved_2;
	unsigned int reserved_3;
	unsigned int reserved_4;
};


struct CFExAveragesTemplateParams : public BiffStructure_NoVtbl
{
public:
	//void toXML(BiffStructurePtr & parent);

private:
	unsigned short iParam;

private:
	unsigned short reserved_1;
	unsigned int reserved_2;
	unsigned int reserved_3;
	unsigned int reserved_4;
};


struct CFExDefaultTemplateParams : public BiffStructure_NoVtbl
{
public:
	//void toXML(BiffStructurePtr & parent);

private:
	unsigned int unused1;
	unsigned int unused2;
	unsigned int unused3;
	unsigned int unused4;
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

