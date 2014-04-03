#pragma once
#include "Common.h"

namespace Writers
{
	class DocumentWriter : public ContentWriter
	{
		XmlUtils::CStringWriter	m_oWriter;
		HeaderFooterWriter& m_oHeaderFooterWriter;
	public:
		CString	m_sDir;
	public:
		DocumentWriter(CString sDir, HeaderFooterWriter& oHeaderFooterWriter):m_sDir(sDir), m_oHeaderFooterWriter(oHeaderFooterWriter)
		{
		}
		void Write()
		{
			CFile oFile;
			oFile.CreateFile(m_sDir + _T("\\word\\document.xml"));
			oFile.WriteStringUTF8(CString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 wp14\"><w:body>")));
			oFile.WriteStringUTF8(m_oContent.GetData());
			oFile.WriteStringUTF8(CString(_T("<w:sectPr >")));
			oFile.WriteStringUTF8(WriteSectPrHdrFtr());
			oFile.WriteStringUTF8(m_oSecPr.GetData());
			oFile.WriteStringUTF8(CString(_T("</w:sectPr>")));
			oFile.WriteStringUTF8(CString(_T("</w:body></w:document>")));
			oFile.CloseFile();
		}
		CString WriteSectPrHdrFtr()
		{
			CString sResult;
			bool bTitlePage = false;
			if(!m_oHeaderFooterWriter.m_oHeaderFirst.rId.IsEmpty())
			{
				sResult += _T("<w:headerReference w:type=\"first\" r:id=\"") + m_oHeaderFooterWriter.m_oHeaderFirst.rId + _T("\"/>");
				bTitlePage = true;
			}
			if(!m_oHeaderFooterWriter.m_oHeaderEven.rId.IsEmpty())
				sResult += _T("<w:headerReference w:type=\"even\" r:id=\"") + m_oHeaderFooterWriter.m_oHeaderEven.rId + _T("\"/>");
			if(!m_oHeaderFooterWriter.m_oHeaderOdd.rId.IsEmpty())
				sResult += _T("<w:headerReference w:type=\"default\" r:id=\"") + m_oHeaderFooterWriter.m_oHeaderOdd.rId + _T("\"/>");
			if(!m_oHeaderFooterWriter.m_oFooterFirst.rId.IsEmpty())
			{
				sResult += _T("<w:footerReference w:type=\"first\" r:id=\"") + m_oHeaderFooterWriter.m_oFooterFirst.rId + _T("\"/>");
				bTitlePage = true;
			}
			if(!m_oHeaderFooterWriter.m_oFooterEven.rId.IsEmpty())
				sResult += _T("<w:footerReference w:type=\"even\" r:id=\"") + m_oHeaderFooterWriter.m_oFooterEven.rId + _T("\"/>");
			if(!m_oHeaderFooterWriter.m_oFooterOdd.rId.IsEmpty())
				sResult += _T("<w:footerReference w:type=\"default\" r:id=\"") + m_oHeaderFooterWriter.m_oFooterOdd.rId + _T("\"/>");
			if(true == bTitlePage)
				sResult += _T("<w:titlePg/>");
			return sResult;
		}
	};
}