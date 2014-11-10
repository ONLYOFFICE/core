#ifndef WEB_SETTINGS_WRITER
#define WEB_SETTINGS_WRITER

#include "../../XlsxSerializerCom/Common/Common.h"

namespace Writers
{
	class WebSettingsWriter
	{
		XmlUtils::CStringWriter  m_oWriter;
		CString	m_sDir;
	public:
		WebSettingsWriter(CString sDir):m_sDir(sDir)
		{
		}
		void Write()
		{
			CString s_Common;

			s_Common = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?> \
<w:webSettings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"> \
<w:optimizeForBrowser/> \
</w:webSettings>");

			m_oWriter.WriteString(s_Common);

            OOX::CPath fileName = m_sDir + FILE_SEPARATOR_STR +_T("word") + FILE_SEPARATOR_STR + _T("webSettings.xml");

            CFile oFile;
            oFile.CreateFile(fileName.GetPath());
			oFile.WriteStringUTF8(m_oWriter.GetData());
			oFile.CloseFile();
		}
	};
}
#endif	// #ifndef WEB_SETTINGS_WRITER
