#pragma once

#include "Serializer.h"

namespace XLSX
{;


class XLSXSerializer : public Serializer
{
public:
	virtual const bool read(Document& doc, const _bstr_t& from);
	virtual const bool write(const Document& doc, const _bstr_t& folder_path);
};


} // namespace XLSX



