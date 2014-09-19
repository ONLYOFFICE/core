#ifndef FILE_WRITER
#define FILE_WRITER

#include "NumberingWriter.h"
#include "FontTableWriter.h"
#include "HeaderFooterWriter.h"
#include "DocumentWriter.h"
#include "MediaWriter.h"
#include "StylesWriter.h"
#include "SettingWriter.h"
#include "CommentsWriter.h"
#include "ChartWriter.h"

namespace Writers
{
	class FileWriter
	{
	public:
		NSBinPptxRW::CDrawingConverter* m_pDrawingConverter;
		LPSAFEARRAY m_pArray;
		CString m_sThemePath;
		bool m_bSaveChartAsImg;
		ContentTypesWriter m_oContentTypesWriter;
		FontTableWriter m_oFontTableWriter;
		DocumentWriter m_oDocumentWriter;
		MediaWriter m_oMediaWriter;
		StylesWriter m_oStylesWriter;
		NumberingWriter m_oNumberingWriter;
		HeaderFooterWriter m_oHeaderFooterWriter;
		SettingWriter m_oSettingWriter;
		CommentsWriter m_oCommentsWriter;
		ChartWriter m_oChartWriter;
		int m_nDocPrIndex;
	public:
		FileWriter(CString sDirOutput,CString sFontDir, int nVersion, bool bSaveChartAsImg, NSBinPptxRW::CDrawingConverter* pDrawingConverter, LPSAFEARRAY pArray, CString sThemePath):
										m_pDrawingConverter(pDrawingConverter),m_pArray(pArray),m_sThemePath(sThemePath),m_bSaveChartAsImg(bSaveChartAsImg),
										m_oContentTypesWriter(sDirOutput), m_oFontTableWriter(sDirOutput, sFontDir),
										m_oHeaderFooterWriter(sDirOutput, m_oContentTypesWriter),
										m_oMediaWriter(sDirOutput),
										m_oStylesWriter(sDirOutput, nVersion),
										m_oNumberingWriter(sDirOutput, m_oContentTypesWriter),
										m_oDocumentWriter(sDirOutput, m_oHeaderFooterWriter),
										m_oSettingWriter(sDirOutput, m_oHeaderFooterWriter),
										m_oCommentsWriter(sDirOutput, m_oContentTypesWriter),
										m_oChartWriter(sDirOutput, m_oContentTypesWriter),
										m_nDocPrIndex(0)
		{
		}
	public: int getNextDocPr()
			{
				m_nDocPrIndex++;
				return m_nDocPrIndex;
			}
	};
}
#endif	// #ifndef FILE_WRITER