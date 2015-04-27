#pragma once

#include "WordDocument.h"

#include "TableMapping.h"

#include "StyleSheetMapping.h"
#include "FontTableMapping.h"
#include "FootnotesMapping.h"
#include "EndnotesMapping.h"
#include "NumberingMapping.h"
#include "CommentsMapping.h"
#include "SettingsMapping.h"
#include "MainDocumentMapping.h"
#include "WordprocessingDocument.h"
#include "ConversionContext.h"

namespace DocFileFormat
{
	class Converter
	{
	public:
		Converter();	  
		~Converter();

		long LoadAndConvert(const CString& strSrcFile, const CString& strDstDirectory, const ProgressCallback* progress);
	private:
		long Convert(WordDocument* doc, WordprocessingDocument* docx, const ProgressCallback* progress);	
	};
}
