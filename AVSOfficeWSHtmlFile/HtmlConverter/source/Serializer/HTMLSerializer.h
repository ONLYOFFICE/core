#pragma once

#include "Serializer.h"

namespace HTML
{;


class HTMLSerializer : public Serializer
{
public:
	virtual const bool read(Document& doc, const _bstr_t& from);
	virtual const bool write(const Document& doc, const _bstr_t& file_path);
};


} // namespace XLSX



