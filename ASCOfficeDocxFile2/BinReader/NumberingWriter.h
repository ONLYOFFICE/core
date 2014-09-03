#ifndef NUMBERING_WRITER
#define NUMBERING_WRITER

#include "ContentTypesWriter.h"
#include "DocumentRelsWriter.h"

namespace Writers
{
	static CString g_string_n_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:numbering xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 wp14\">");
	static CString g_string_n_End = _T("</w:numbering>");

	class NumberingWriter
	{
		XmlUtils::CStringWriter	m_oWriter;
		CString	m_sDir;
		ContentTypesWriter& m_oContentTypesWriter;
	public:
		XmlUtils::CStringWriter	m_oANum;
		XmlUtils::CStringWriter	m_oNumList;
	public:
		NumberingWriter(CString sDir, ContentTypesWriter& oContentTypesWriter):m_sDir(sDir),m_oContentTypesWriter(oContentTypesWriter)
		{
		}
		bool IsEmpty()
		{
			return 0 == m_oANum.GetCurSize();
		}
		void Write()
		{
			if(false == IsEmpty())
			{
				m_oWriter.WriteString(g_string_n_Start);
				m_oWriter.Write(m_oANum);
				m_oWriter.Write(m_oNumList);
				m_oWriter.WriteString(g_string_n_End);

				CFile oFile;
				oFile.CreateFile(m_sDir + _T("\\word\\numbering.xml"));
				oFile.WriteStringUTF8(m_oWriter.GetData());
				oFile.CloseFile();

				//ContentType
				m_oContentTypesWriter.AddOverride(CString(_T("/word/numbering.xml")), CString(_T("application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml")));
				//Rels
				//m_oDocumentRelsWriter.AddRels(_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering"), _T("numbering.xml"));
			}
		}
	};
}
#endif	// #ifndef NUMBERING_WRITER