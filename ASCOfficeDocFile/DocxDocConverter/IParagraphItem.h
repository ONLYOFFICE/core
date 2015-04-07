#pragma once

#include "Chpx.h"

#include "../../Common/DocxFormat/Source/Base/SmartPtr.h"
#include "../Common/IVirtualConstructor.h"

namespace ASCDocFileFormat
{
	class IParagraphItem;
	typedef NSCommon::smart_ptr<IParagraphItem> IParagraphItemPtr;

	class IParagraphItem : public IVirtualConstructor
	{
	public:
		virtual unsigned long GetTextSize() const = 0;
		virtual std::wstring GetAllText() const = 0;
		virtual vector<Chpx> GetRunProperties( vector<unsigned int>* runOffsets ) const = 0;
		virtual unsigned int PrlSize () const = 0;
		virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* runOffsets ) const = 0;

		virtual ~IParagraphItem() {}
	};
}