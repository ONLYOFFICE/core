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
		CString AddImageGetNewPath()
		{
			if( !NSDirectory::Exists(string2std_string(m_sMediaDir)) )
				OOX::CSystemUtility::CreateDirectories(m_sMediaDir);

			CString sNewImgName;sNewImgName.Format(_T("image%d.jpg"), (nImageCount + 1));
            CString sNewImg = m_sMediaDir + _T("/") + sNewImgName;
			nImageCount++;
			return sNewImg;
		}
		void AddImage2(FILE* pFile)
		{
			long size = ftell(pFile);
			if(size > 0)
			{
				rewind(pFile);
				BYTE* pData = new BYTE[size];
				DWORD dwSizeRead = (DWORD)fread((void*)pData, 1, size, pFile);
				if(dwSizeRead > 0)
				{
					CString sNewImagePath = AddImageGetNewPath();
					NSFile::CFileBinary oFile;
					oFile.CreateFileW(string2std_string(sNewImagePath));
					oFile.WriteFile(pData, dwSizeRead);
					oFile.CloseFile();
					CString sFilename = NSSystemPath::GetFileName(string2std_string(sNewImagePath)).c_str();
					m_aImageNames.push_back(sFilename);
				}
				RELEASEARRAYOBJECTS(pData);
			}
		}
		void AddImage(const CString& sImg)
		{
			CString sNewImg = AddImageGetNewPath();

			NSFile::CFileBinary::Copy(string2std_string(sImg), string2std_string(sNewImg));
			CString sFilename = NSSystemPath::GetFileName(string2std_string(sNewImg)).c_str();
			m_aImageNames.push_back(sFilename);
		}
	};
}
#endif	// #ifndef MEDIA_WRITER
