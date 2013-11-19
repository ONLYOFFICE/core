#pragma once
#include "Common.h"

namespace Writers
{
	static CString g_string_set_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><w:settings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\">");
	static CString g_string_set_Default = _T("<w:zoom w:percent=\"100\"/><w:characterSpacingControl w:val=\"doNotCompress\"/><w:compat/><w:themeFontLang w:val=\"en-US\" w:eastAsia=\"zh-CN\"/><w:shapeDefaults><o:shapedefaults v:ext=\"edit\" spidmax=\"1026\"/><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout></w:shapeDefaults><w:decimalSymbol w:val=\".\"/><w:listSeparator w:val=\",\"/>");
	static CString g_string_set_End = _T("</w:settings>");

	class SettingWriter
	{
		CString m_sDir;
		CStringWriter	m_oSettingWriter;
		HeaderFooterWriter& m_oHeaderFooterWriter;
	public:
		SettingWriter(CString sDir, HeaderFooterWriter& oHeaderFooterWriter):m_sDir(sDir),m_oHeaderFooterWriter(oHeaderFooterWriter)
		{
		}
		void Write()
		{
			Prepare();
			CFile oFile;
			oFile.CreateFile(m_sDir + _T("\\word\\settings.xml"));
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
			bool bevenAndOddHeaders = false;
			if(!m_oHeaderFooterWriter.m_oHeaderEven.rId.IsEmpty())
				bevenAndOddHeaders = true;
			if(!m_oHeaderFooterWriter.m_oFooterEven.rId.IsEmpty())
				bevenAndOddHeaders = true;
			if(bevenAndOddHeaders)
				AddSetting(_T("<w:evenAndOddHeaders/>"));
		}
	};
}