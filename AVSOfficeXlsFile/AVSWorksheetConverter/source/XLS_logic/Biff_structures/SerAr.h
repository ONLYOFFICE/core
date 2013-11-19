#pragma once

#include "BiffStructTagged.h"

namespace XLS
{;

class SerAr;
typedef boost::shared_ptr<SerAr> SerArPtr;

class SerAr : public BiffStructTagged
{
public:
	void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);

	static SerArPtr createSerAr(const BYTE type);

	virtual const std::wstring toString() const = 0;

};

typedef boost::shared_ptr<SerAr> SerArPtr;

} // namespace XLS
