#ifndef COMMENTS_WRITER
#define COMMENTS_WRITER

#include "Common.h"

namespace Writers
{
	static CString g_string_comment_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:comments xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\">");
	static CString g_string_comment_End = _T("</w:comments>");
	static CString g_string_commentExt_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w15:commentsEx xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\">");
	static CString g_string_commentExt_End = _T("</w15:commentsEx>");
	static CString g_string_people_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w15:people xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\">");
	static CString g_string_people_End = _T("</w15:people>");

	class CommentsWriter
	{
		CString	m_sDir;
		ContentTypesWriter& m_oContentTypesWriter;
	public:
		CString m_sComment;
		CString m_sCommentExt;
		CString m_sPeople;
	public:
		CommentsWriter(CString sDir, ContentTypesWriter& oContentTypesWriter):m_sDir(sDir),m_oContentTypesWriter(oContentTypesWriter)
		{
		}
		void setElements(CString& sComment, CString& sCommentExt, CString& sPeople)
		{
			m_sComment = sComment;
			m_sCommentExt = sCommentExt;
			m_sPeople = sPeople;
		}
		void Write()
		{
			if(false == m_sComment.IsEmpty())
			{
				CFile oFile;
				oFile.CreateFile(m_sDir + _T("\\word\\comments.xml"));
				oFile.WriteStringUTF8(g_string_comment_Start);
				oFile.WriteStringUTF8(m_sComment);
				oFile.WriteStringUTF8(g_string_comment_End);
				oFile.CloseFile();

				//Content_Types
				m_oContentTypesWriter.AddOverride(CString(_T("/word/comments.xml")), CString(_T("application/vnd.openxmlformats-officedocument.wordprocessingml.comments+xml")));

				//Rels
				//m_oDocumentRelsWriter.AddRels(_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments"), _T("comments.xml"));
			}
			if(false == m_sCommentExt.IsEmpty())
			{
				CFile oFile;
				oFile.CreateFile(m_sDir + _T("\\word\\commentsExtended.xml"));
				oFile.WriteStringUTF8(g_string_commentExt_Start);
				oFile.WriteStringUTF8(m_sCommentExt);
				oFile.WriteStringUTF8(g_string_commentExt_End);
				oFile.CloseFile();

				//Content_Types
				m_oContentTypesWriter.AddOverride(CString(_T("/word/commentsExtended.xml")), CString(_T("application/vnd.openxmlformats-officedocument.wordprocessingml.commentsExtended+xml")));
			}
			if(false == m_sPeople.IsEmpty())
			{
				CFile oFile;
				oFile.CreateFile(m_sDir + _T("\\word\\people.xml"));
				oFile.WriteStringUTF8(g_string_people_Start);
				oFile.WriteStringUTF8(m_sPeople);
				oFile.WriteStringUTF8(g_string_people_End);
				oFile.CloseFile();

				//Content_Types
				m_oContentTypesWriter.AddOverride(CString(_T("/word/people.xml")), CString(_T("application/vnd.openxmlformats-officedocument.wordprocessingml.people+xml")));
			}
		}
	};
}
#endif	// #ifndef COMMENTS_WRITER