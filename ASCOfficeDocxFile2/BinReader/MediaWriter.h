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

            OOX::CPath filePath = m_sDir + FILE_SEPARATOR_STR + _T("word") + FILE_SEPARATOR_STR + _T("media");

            m_sMediaDir = filePath.GetPath();
		}
		CString AddImageGetNewPath()
		{
			OOX::CSystemUtility::CreateDirectories(m_sMediaDir);

			CString sNewImgName;sNewImgName.Format(_T("image%d.jpg"), (nImageCount + 1));
            CString sNewImg = m_sMediaDir + FILE_SEPARATOR_STR + sNewImgName;
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
            OOX::CPath pathNewImg = AddImageGetNewPath();

            NSFile::CFileBinary::Copy(string2std_string(sImg), string2std_string(pathNewImg.GetPath()));
            CString sFilename = NSSystemPath::GetFileName(string2std_string(pathNewImg.GetPath())).c_str();
			m_aImageNames.push_back(sFilename);
		}
	};
}
#endif	// #ifndef MEDIA_WRITER
