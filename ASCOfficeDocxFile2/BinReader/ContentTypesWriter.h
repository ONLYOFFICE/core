#ifndef CONTENT_TYPES_WRITER
#define CONTENT_TYPES_WRITER

#include "../../XlsxSerializerCom/Common/Common.h"

namespace Writers
{
	static CString g_string_ct_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">");
	static CString g_string_ct_Ext = _T("<Default Extension=\"bin\" ContentType=\"application/vnd.openxmlformats-officedocument.oleObject\"/><Default Extension=\"bmp\" ContentType=\"image/bmp\"/><Default Extension=\"jpg\" ContentType=\"image/jpeg\"/><Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/><Default Extension=\"jpe\" ContentType=\"image/jpeg\"/><Default Extension=\"png\" ContentType=\"image/png\"/><Default Extension=\"gif\" ContentType=\"image/gif\"/><Default Extension=\"emf\" ContentType=\"image/x-emf\"/><Default Extension=\"wmf\" ContentType=\"image/x-wmf\"/><Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/><Default Extension=\"xml\" ContentType=\"application/xml\"/>");
	static CString g_string_ct_Override = _T("<Override PartName=\"/word/document.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml\"/><Override PartName=\"/word/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml\"/><Override PartName=\"/word/settings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml\"/><Override PartName=\"/word/webSettings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml\"/><Override PartName=\"/word/fontTable.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml\"/><Override PartName=\"/word/theme/theme1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/><Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\"/><Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\"/>");
	static CString g_string_ct_End = _T("</Types>");

	class ContentTypesWriter
	{
		XmlUtils::CStringWriter	m_oWriter;
		CString	m_sDir;
		XmlUtils::CStringWriter	m_oAdditional;
	public:
		ContentTypesWriter(CString sDir):m_sDir(sDir)
		{
		}
		void Write()
		{
			m_oWriter.WriteString(g_string_ct_Start);
			m_oWriter.WriteString(g_string_ct_Ext);
			m_oWriter.WriteString(g_string_ct_Override);
			m_oWriter.Write(m_oAdditional);
			m_oWriter.WriteString(g_string_ct_End);

			OOX::CPath filePath = m_sDir + _T("/[Content_Types].xml");

			CFile oFile;
			oFile.CreateFile(filePath.GetPath());

			oFile.WriteStringUTF8(m_oWriter.GetData());
			oFile.CloseFile();
		}
		void AddOverride(const CString& PartName, const CString& ContentType)
		{
            CString sOverride;sOverride.Format(_T("<Override PartName=\"%ls\" ContentType=\"%ls\"/>"),PartName , ContentType);
			m_oAdditional.WriteString(sOverride);
		}
		void AddOverrideRaw(const CString& sXml)
		{
			m_oAdditional.WriteString(sXml);
		}
	};
}
#endif	// #ifndef CONTENT_TYPES_WRITER
