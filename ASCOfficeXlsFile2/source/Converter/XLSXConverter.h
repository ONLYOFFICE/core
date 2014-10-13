#pragma once

#include <Converter/Converter.h>

namespace XLSX
{;


class XLSXConverter : public Converter
{
public:
	virtual const bool convertToX(Document& doc);
	virtual const bool convertFromX(Document& doc);
};

} // namespace XLSX

