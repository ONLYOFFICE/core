#ifndef DOCUMENT_WRITER
#define DOCUMENT_WRITER

#include "Common.h"
#include "../BinWriter/BinReaderWriterDefines.h";
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
			if(g_nCurFormatVersion < 5)
			{
				bool bTitlePage = false;
				for(int i = 0, length = m_oHeaderFooterWriter.m_aHeaders.size(); i < length; ++i)
				{
					HdrFtrItem* pHeader = m_oHeaderFooterWriter.m_aHeaders[i];
					if(false == pHeader->rId.IsEmpty())
					{
						if(SimpleTypes::hdrftrFirst == pHeader->eType)
						{
							sResult += _T("<w:headerReference w:type=\"first\" r:id=\"") + pHeader->rId + _T("\"/>");
							bTitlePage = true;
						}
						else if(SimpleTypes::hdrftrEven == pHeader->eType)
						{
							sResult += _T("<w:headerReference w:type=\"even\" r:id=\"") + pHeader->rId + _T("\"/>");
						}
						else
						{
							sResult += _T("<w:headerReference w:type=\"default\" r:id=\"") + pHeader->rId + _T("\"/>");
						}
					}
				}
				for(int i = 0, length = m_oHeaderFooterWriter.m_aFooters.size(); i < length; ++i)
				{
					HdrFtrItem* pFooter = m_oHeaderFooterWriter.m_aFooters[i];
					if(false == pFooter->rId.IsEmpty())
					{
						if(SimpleTypes::hdrftrFirst == pFooter->eType)
						{
							sResult += _T("<w:footerReference w:type=\"first\" r:id=\"") + pFooter->rId + _T("\"/>");
							bTitlePage = true;
						}
						else if(SimpleTypes::hdrftrEven == pFooter->eType)
						{
							sResult += _T("<w:footerReference w:type=\"even\" r:id=\"") + pFooter->rId + _T("\"/>");
						}
						else
						{
							sResult += _T("<w:footerReference w:type=\"default\" r:id=\"") + pFooter->rId + _T("\"/>");
						}
					}
				}
				if(true == bTitlePage)
					sResult += _T("<w:titlePg/>");
			}
			return sResult;
		}
	};
}
#endif	// #ifndef DOCUMENT_WRITER