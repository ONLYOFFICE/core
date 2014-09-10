#ifndef MEDIA_WRITER
#define MEDIA_WRITER

#include "../../XlsxSerializerCom/Common/Common.h"

namespace Writers
{
	class MediaWriter
	{
		XmlUtils::CStringWriter	m_oWriter;
		CString	m_sDir;
		CString	m_sMediaDir;
	public:
		std::vector<CString> m_aImageNames;
		long nImageCount;
	public:
		MediaWriter(CString sDir):m_sDir(sDir)
		{
			nImageCount = 0;
			m_sMediaDir = m_sDir + _T("\\word\\media");
		}
		void AddImage(CString& sImg)
		{
			CreateDirectory(m_sMediaDir, NULL);

			//TCHAR tExt[256];
			//TCHAR tFilename[256];
			//_tsplitpath( sImg, NULL, NULL, tFilename, tExt );
			//CString sExt = CString(tExt);
			//CString sFilename = CString(tFilename);
			CString sNewImgName;sNewImgName.Format(_T("image%d.jpg"), (nImageCount + 1));
			CString sNewImg = m_sMediaDir;
			sNewImg += _T("\\") + sNewImgName;
			
			CopyFile(sImg, sNewImg, FALSE);
			m_aImageNames.push_back(sNewImgName);

			//CString sNewImgRel;sNewImgRel = _T("media\\") + sNewImgName;
			//CorrectString(sNewImgRel);
			//m_aImageRels.Add(m_poDocumentRelsWriter->AddRels(_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image"), sNewImgRel, false));
			nImageCount++;
		}
	};
}
#endif	// #ifndef MEDIA_WRITER