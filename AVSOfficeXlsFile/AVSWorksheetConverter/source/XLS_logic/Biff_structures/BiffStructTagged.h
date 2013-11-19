#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;


class BiffStructTagged : public BiffStructure
{
public:
	// Makes XML data of classes those own their own tags
	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	// Reads XML data of classes those own their own tags
	// 'position' is the current number of this structure among other co-named structures in siblings
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent, const size_t position = 1, const bool is_mandatory = false);

private:
	// Sets attributes of just created node
	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag) {};
	// Gets attributes of already parsed node
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag) {};
};


typedef std::vector<BiffStructTagged> BiffStructTaggedPtr;


} // namespace XLS
