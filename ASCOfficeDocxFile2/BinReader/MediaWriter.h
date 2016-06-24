/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
