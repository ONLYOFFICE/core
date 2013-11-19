#pragma once

#include "WordDocument.h"
#include "StyleSheetMapping.h"
#include "MainDocumentMapping.h"
#include "FontTableMapping.h"
#include "FootnotesMapping.h"
#include "EndnotesMapping.h"
#include "NumberingMapping.h"
#include "CommentsMapping.h"
#include "SettingsMapping.h"
#include "WordprocessingDocument.h"
#include "ConversionContext.h"

extern AVSOfficeCriticalSection g_oCriticalSection;

namespace DocFileFormat
{
	class Converter
	{
	public:
		Converter();	  
		~Converter();

		long LoadAndConvert(const CString& strSrcFile, const CString& strDstFile, const ProgressCallback* progress);
	private:
		long Convert(WordDocument* doc, WordprocessingDocument* docx, const ProgressCallback* progress);	
	};
}