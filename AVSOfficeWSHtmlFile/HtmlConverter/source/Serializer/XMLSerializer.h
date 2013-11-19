#pragma once

#include "Serializer.h"

namespace XLS
{;


class XMLSerializer : public Serializer
{
public:
	virtual const bool read(Document& doc, const _bstr_t& from);
	virtual const bool write(const Document& doc, const _bstr_t& to);
};




} // namespace XLS

