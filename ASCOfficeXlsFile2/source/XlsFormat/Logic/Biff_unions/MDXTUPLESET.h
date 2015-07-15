#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of MDXTUPLESET union of records 
class MDXTUPLESET: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MDXTUPLESET)
public:
	MDXTUPLESET();
	~MDXTUPLESET();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeMDXTUPLESET;
};

} // namespace XLS

