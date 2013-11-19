#pragma once

#include <Converter/Converter.h>

namespace XLS
{;


class XLSConverter : public Converter
{
public:
	XLSConverter();

	virtual const bool convertToX(Document& doc);
	virtual const bool convertFromX(Document& doc);
};

} // namespace XLS

