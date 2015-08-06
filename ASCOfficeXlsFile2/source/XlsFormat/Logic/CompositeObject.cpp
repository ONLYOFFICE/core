
#include "CompositeObject.h"
#include "BinProcessor.h"


namespace XLS
{;

CompositeObject::CompositeObject()
{
}


CompositeObject::~CompositeObject()
{
}


const bool CompositeObject::read(CFStreamCacheReader& reader, BaseObject* parent, const bool is_mandatory)
{
	BinReaderProcessor reader_proc(reader, this, is_mandatory);
	if(loadContentRead(reader_proc))
	{
		parent->add_child(this->clone());
		return true;
	}

	return false;
}




const bool CompositeObject::loadContentRead(BinReaderProcessor& proc)
{
	return loadContent(proc);
}



const bool ABNFParenthesis::read(CFStreamCacheReader& reader, BaseObject* parent, const bool is_mandatory)
{
	BinReaderProcessor reader_proc(reader, parent, is_mandatory);
	bool res = loadContentRead(reader_proc);

	//parent->add_child(this);
	return res;
};



} // namespace XLS
