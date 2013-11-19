#include "stdafx.h"
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

/*
const bool CompositeObject::doSomething(BinProcessor& proc)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);
	
	BinReaderProcessor reader_proc(reader, own_tag, is_mandatory);
	if(loadContent(reader_proc))
	{
		return true;
	}
	parent->removeChild(own_tag); // When nothing was read for this composite. For example, if loadFirstMandatory returns false
	return false;
}

*/

const bool CompositeObject::read(CFStreamCacheReader& reader, MSXML2::IXMLDOMElementPtr parent, const bool is_mandatory)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);	
	
	BinReaderProcessor reader_proc(reader, own_tag, is_mandatory);
	if(loadContentRead(reader_proc))
	{
		toXML(own_tag);
		return true;
	}
	parent->removeChild(own_tag); // When nothing was read for this composite. For example, if loadFirstMandatory returns false	

	return false;
}


const bool CompositeObject::write(BinWriterProcessor& proc, const bool is_mandatory)
{
	MSXML2::IXMLDOMElementPtr own_tag = proc.getChildNode(getClassName(), is_mandatory);
	if(!own_tag)
	{
		return false;
	}
	fromXML(own_tag);
	BinWriterProcessor writer_proc(proc, own_tag, is_mandatory); // Creates new processor with empty tags counters and own tag.
	return loadContentWrite(writer_proc);
}


const bool CompositeObject::loadContentRead(BinReaderProcessor& proc)
{
	return loadContent(proc);
}

const bool CompositeObject::loadContentWrite(BinWriterProcessor& proc)
{
	return loadContent(proc);
}

void CompositeObject::toXML(MSXML2::IXMLDOMElementPtr own_tag)
{
}


void CompositeObject::fromXML(MSXML2::IXMLDOMElementPtr own_tag)
{
}


const bool ABNFParenthesis::read(CFStreamCacheReader& reader, MSXML2::IXMLDOMElementPtr parent, const bool is_mandatory)
{
	BinReaderProcessor reader_proc(reader, parent, is_mandatory);
	return loadContentRead(reader_proc);
};


const bool ABNFParenthesis::write(BinWriterProcessor& proc, const bool is_mandatory)
{
	//BinWriterProcessor writer_proc(proc, false);
	return loadContentWrite(BinWriterProcessor(proc, is_mandatory));
};


} // namespace XLS
