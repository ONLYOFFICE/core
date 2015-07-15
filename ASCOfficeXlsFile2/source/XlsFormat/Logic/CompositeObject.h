#pragma once

#include "BaseObject.h"
#include "BinProcessor.h"

namespace XLS
{;

class CFStreamCacheReader;

// Logical representation of composite objects
//
// Another new function is to buffer objects, that had to be read but was absent in the bin stream.
// The buffered records shall be checked for existence later.
// This feature is specially for files saved in OpenOffice Calc those don't conform to the Microsoft specification
// Call them "wanted objects"
//
class CompositeObject : public BaseObject
{
public:
	CompositeObject();
	~CompositeObject();

	//virtual const bool doSomething(BinProcessor& proc);
	virtual const bool read(CFStreamCacheReader& reader, BaseObject* parent, const bool mandatory); // Read self and children
	//virtual const bool write(BinWriterProcessor& proc, const bool is_mandatory); // Write self and children
	virtual const bool loadContent(BinProcessor& proc) = 0;

	virtual const bool loadContentRead(BinReaderProcessor& proc);

	static const ElementType	type = typeCompositeObject;
	
	virtual ElementType get_type() { return type; }
};

// This class helps to process parenthesis in ABNF specification
class ABNFParenthesis : public CompositeObject
{
public:
	ABNFParenthesis(){};

	virtual const bool read(CFStreamCacheReader& reader, BaseObject* parent, const bool is_mandatory);

	static const ElementType	type = typeABNFParenthesis;
	

};


} // namespace XLS
