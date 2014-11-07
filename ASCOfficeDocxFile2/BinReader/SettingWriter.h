#ifndef SETTING_WRITER
#define SETTING_WRITER

#include "../../XlsxSerializerCom/Common/Common.h"

namespace Writers
{
	static CString g_string_set_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><w:settings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\">");
	static CString g_string_set_Default = _T("<w:zoom w:percent=\"100\"/><w:characterSpacingControl w:val=\"doNotCompress\"/><w:compat><w:compatSetting w:name=\"compatibilityMode\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"14\"/><w:compatSetting w:name=\"overrideTableStyleFontSizeAndJustification\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"enableOpenTypeFeatures\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"doNotFlipMirrorIndents\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/></w:compat><w:themeFontLang w:val=\"en-US\" w:eastAsia=\"zh-CN\"/><w:shapeDefaults><o:shapedefaults v:ext=\"edit\" spidmax=\"1026\"/><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout></w:shapeDefaults><w:decimalSymbol w:val=\".\"/><w:listSeparator w:val=\",\"/>");
	static CString g_string_set_End = _T("</w:settings>");

	class SettingWriter
	{
		CString m_sDir;
		XmlUtils::CStringWriter	m_oSettingWriter;
		HeaderFooterWriter& m_oHeaderFooterWriter;
	public:
		SettingWriter(CString sDir, HeaderFooterWriter& oHeaderFooterWriter):m_sDir(sDir),m_oHeaderFooterWriter(oHeaderFooterWriter)
		{
		}
		void Write()
		{
			Prepare();
			OOX::CPath filePath = m_sDir + _T("\\word\\settings.xml");

			CFile oFile;
			oFile.CreateFile(filePath.GetPath());
			oFile.WriteStringUTF8(g_string_set_Start);
			oFile.WriteStringUTF8(m_oSettingWriter.GetData());
			oFile.WriteStringUTF8(g_string_set_Default);
			oFile.WriteStringUTF8(g_string_set_End);
			oFile.CloseFile();
		}
		
		void AddSetting(CString sSetting)
		{
			m_oSettingWriter.WriteString(sSetting);
		}
		void Prepare()
		{
			if(BinDocxRW::g_nCurFormatVersion < 5)
			{
				bool bevenAndOddHeaders = false;
				for(int i = 0, length = m_oHeaderFooterWriter.m_aHeaders.size(); i < length; ++i)
				{
					HdrFtrItem* pHeader = m_oHeaderFooterWriter.m_aHeaders[i];
					if(SimpleTypes::hdrftrEven == pHeader->eType)
						bevenAndOddHeaders = true;
				}
				for(int i = 0, length = m_oHeaderFooterWriter.m_aFooters.size(); i < length; ++i)
				{
					HdrFtrItem* pFooter = m_oHeaderFooterWriter.m_aFooters[i];
					if(SimpleTypes::hdrftrEven == pFooter->eType)
						bevenAndOddHeaders = true;
				}
				if(bevenAndOddHeaders)
					AddSetting(_T("<w:evenAndOddHeaders/>"));
			}
		}
	};
}
#endif	// #ifndef SETTING_WRITER