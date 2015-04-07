#ifndef FILE_WRITER
#define FILE_WRITER

#include "NumberingWriter.h"
#include "fontTableWriter.h"
#include "HeaderFooterWriter.h"
#include "DocumentWriter.h"
#include "MediaWriter.h"
#include "StylesWriter.h"
#include "SettingWriter.h"
#include "CommentsWriter.h"
#include "ChartWriter.h"
#include "DocumentRelsWriter.h"
#include "webSettingsWriter.h"

#if defined(_WIN32) || defined (_WIN64)
	#include "DefaultThemeWriterWin.h"
#else
	#include "DefaultThemeWriterLinux.h"
#endif
namespace BinDocxRW {
	class CComments;
}
namespace Writers
{
	class FileWriter
	{
	public:
		NSBinPptxRW::CDrawingConverter* m_pDrawingConverter;
		CString m_sThemePath;
		bool m_bSaveChartAsImg;
		
		ContentTypesWriter		m_oContentTypesWriter;
		FontTableWriter			m_oFontTableWriter;
		DocumentWriter			m_oDocumentWriter;
		MediaWriter				m_oMediaWriter;
		StylesWriter			m_oStylesWriter;
		NumberingWriter			m_oNumberingWriter;
		HeaderFooterWriter		m_oHeaderFooterWriter;
		SettingWriter			m_oSettingWriter;
		CommentsWriter			m_oCommentsWriter;
		ChartWriter				m_oChartWriter;
		DocumentRelsWriter		m_oDocumentRelsWriter;
		WebSettingsWriter		m_oWebSettingsWriter;
		DefaultThemeWriter		m_oDefaultTheme;
		
		int m_nDocPrIndex;
		BinDocxRW::CComments* m_pComments;
	public:
		FileWriter(CString sDirOutput,CString sFontDir, int nVersion, bool bSaveChartAsImg, NSBinPptxRW::CDrawingConverter* pDrawingConverter, CString sThemePath):
										m_pDrawingConverter(pDrawingConverter),m_sThemePath(sThemePath),m_bSaveChartAsImg(bSaveChartAsImg),
										m_oContentTypesWriter(sDirOutput), m_oFontTableWriter(sDirOutput, sFontDir),
										m_oHeaderFooterWriter(sDirOutput, m_oContentTypesWriter),
										m_oMediaWriter(sDirOutput),
										m_oStylesWriter(sDirOutput, nVersion),
										m_oNumberingWriter(sDirOutput, m_oContentTypesWriter),
										m_oDocumentWriter(sDirOutput, m_oHeaderFooterWriter),
										m_oSettingWriter(sDirOutput, m_oHeaderFooterWriter),
										m_oCommentsWriter(sDirOutput, m_oContentTypesWriter),
										m_oChartWriter(sDirOutput, m_oContentTypesWriter),
										m_oDocumentRelsWriter(sDirOutput),
										m_oWebSettingsWriter(sDirOutput),
										m_nDocPrIndex(0),
										m_pComments(NULL)
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
