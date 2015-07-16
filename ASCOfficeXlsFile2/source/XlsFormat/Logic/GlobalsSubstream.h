#pragma once

#include "CompositeObject.h"

namespace XLS
{;

class CFStreamCacheReader;
class BOF;

// Logical representation of Globals substream 
class GlobalsSubstream;
typedef boost::shared_ptr<GlobalsSubstream>		GlobalsSubstreamPtr;

class GlobalsSubstream: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(GlobalsSubstream)
public:
	GlobalsSubstream(const unsigned short code_page);
	~GlobalsSubstream();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeGlobalsSubstream;
	
	BaseObjectPtr m_Theme;
	BaseObjectPtr m_Formating;
	BaseObjectPtr m_Template;
	
	unsigned short code_page_;
};

} // namespace XLS
