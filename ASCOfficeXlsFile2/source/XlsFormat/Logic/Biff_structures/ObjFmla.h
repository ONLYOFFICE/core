#pragma once

#include "BiffStructure.h"
#include "ObjectParsedFormula.h"
#include "PictFmlaEmbedInfo.h"

namespace XLS
{;

class CFRecord;

class ObjFmla : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ObjFmla)
public:
	ObjFmla(const bool is_part_of_FtPictFmla);
	BiffStructurePtr clone();

	static const ElementType	type = typeObjFmla;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	ObjectParsedFormula fmla;

	bool is_part_of_FtPictFmla_;
	bool fmla_found;
	PictFmlaEmbedInfo embedInfo;

};

} // namespace XLS

