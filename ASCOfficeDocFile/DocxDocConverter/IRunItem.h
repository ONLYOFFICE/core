#pragma once

#include "Prl.h"

namespace AVSDocFileFormat
{
	struct IRunItem : public IVirtualConstructor
	{
		virtual unsigned long GetTextSize() const = 0;
		virtual wstring GetAllText() const = 0;
		virtual vector<Prl> GetRunProperties() const = 0;
		virtual unsigned int PrlSize () const = 0;
		virtual ~IRunItem() {}
	};
}