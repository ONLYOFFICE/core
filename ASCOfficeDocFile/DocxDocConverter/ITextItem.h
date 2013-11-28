#pragma once

#include "PapxInFkp.h"
#include "Run.h"

namespace AVSDocFileFormat
{
	struct ITextItem;  

	typedef NSCommon::smart_ptr<ITextItem> TextItemPtr;

	struct ITextItem : public IVirtualConstructor
	{
		virtual wstring GetAllText() const = 0;
		virtual operator wstring() const = 0;
		virtual vector<PapxInFkp> GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const = 0;
		virtual vector<Chpx> GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const = 0;
		virtual vector<TextItemPtr> GetAllParagraphsCopy() const = 0;
		virtual vector<ITextItem*> GetAllParagraphs() = 0;
		virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const = 0;
		virtual vector<IParagraphItemPtr> GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const = 0;

		virtual ~ITextItem() {}
	};
}