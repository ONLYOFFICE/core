#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

class CFRecord;

class PictFmlaEmbedInfo : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PictFmlaEmbedInfo)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typePictFmlaEmbedInfo;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	XLUnicodeStringNoCch strClass;
};

} // namespace XLS

