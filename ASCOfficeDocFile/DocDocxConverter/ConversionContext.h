#pragma once

#include "WordDocument.h"
#include "WordprocessingDocument.h"

namespace DocFileFormat
{
	class ConversionContext
	{
	public:
		ConversionContext (WordDocument* doc, WordprocessingDocument* docx)
		{
			_doc = doc;
			_docx = docx;
		}

		virtual ~ConversionContext()
		{

		}

		// Adds a new RSID to the set
		inline void AddRsid(const wstring& rsid)
		{
			if (AllRsids.find(rsid) == AllRsids.end())
				AllRsids.insert(rsid);
		}

		inline WordDocument* GetDocument()
		{
			return _doc;
		}

		inline WordprocessingDocument* GetXmlDocument()
		{
			return _docx;
		}
	
	public:
		WordprocessingDocument*		_docx;	
		WordDocument*				_doc;	
		/// A set thta contains all revision ids.
		set<wstring>				AllRsids;
	};
}