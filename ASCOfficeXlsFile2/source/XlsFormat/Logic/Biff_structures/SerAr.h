#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class SerAr;
typedef boost::shared_ptr<SerAr> SerArPtr;

class SerAr : public BiffStructure
{
public:
	//void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);

	static SerArPtr createSerAr(const unsigned char type);

	virtual const std::wstring toString() const = 0;

};

typedef boost::shared_ptr<SerAr> SerArPtr;

} // namespace XLS
