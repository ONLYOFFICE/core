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
#include "Common.h"

#include "BinReaderWriterDefines.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"
#include "../../Common/Base64.h"
#include "../../DesktopEditor/common/Types.h"
#ifndef DISABLE_FILE_DOWNLOADER
#include "../../Common/FileDownloader/FileDownloader.h"
#endif
#include "../../DesktopEditor/common/File.h"

#ifndef CP_UTF8
#define CP_UTF8 65001
#endif

namespace SerializeCommon
{
	CString DownloadImage(const CString& strFile)
	{
#ifndef DISABLE_FILE_DOWNLOADER
        CFileDownloader oDownloader(string2std_string(strFile), false);
		CString strFileName;
		if ( oDownloader.DownloadSync() )
		{
			strFileName = std_string2string(oDownloader.GetFilePath());
		}
		return strFileName;
#else
		return L"";
#endif
	}
	VOID convertBase64ToImage (NSFile::CFileBinary& oFile, CString &pBase64)
	{
		BYTE* pUtf8 = NULL;
		long nUtf8Size;
		NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pBase64.GetString(), pBase64.GetLength(), pUtf8, nUtf8Size);
		CStringA sUnicode((char*)pUtf8, nUtf8Size);
		RELEASEARRAYOBJECTS(pUtf8);

		//Убираем "data:image/jpg;base64,"
		int nShift = 0;
		int nIndex = sUnicode.Find("base64,");
		if(-1 != nIndex)
		{
			nShift = nIndex + 7;
		}
		// Получаем размер файла
		LONG lFileSize = sUnicode.GetLength () - nShift;
		INT nDstLength = lFileSize;
		BYTE *pBuffer = new BYTE [lFileSize];
		memset(pBuffer, 0, lFileSize);
		Base64::Base64Decode ((LPCSTR)sUnicode.GetBuffer () + nShift, lFileSize, pBuffer, &nDstLength);

		// Пишем в файл
		oFile.WriteFile(pBuffer, nDstLength);

		RELEASEARRAYOBJECTS (pBuffer);
	}

	long Round(double val)
	{
		return (long)(val+ 0.5);
	}
	CString changeExtention(const CString& sSourcePath, const CString& sTargetExt)
	{
		int nIndex = sSourcePath.ReverseFind('.');
		if(-1 != nIndex)
			return sSourcePath.Left(nIndex + 1) + sTargetExt;
		return sSourcePath;
	}
    void ReadFileType(const CString& sXMLOptions, BYTE& result, UINT& nCodePage, WCHAR& wcDelimiter, BYTE& cSaveFileType)
	{
		result = BinXlsxRW::c_oFileTypes::XLSX;
		nCodePage = CP_UTF8;
		wcDelimiter = _T(',');
		cSaveFileType = BinXlsxRW::c_oFileTypes::XLSX;

		nullable<SimpleTypes::CUnsignedDecimalNumber<>> fileType;
		nullable<SimpleTypes::CUnsignedDecimalNumber<>> codePage;
		nullable<SimpleTypes::CUnsignedDecimalNumber<>> saveFileType;
		nullable<CString> delimiter;

		// Read options
		XmlUtils::CXmlLiteReader oReader;
        if (true != oReader.FromString(sXMLOptions) || true != oReader.IsValid())
			return;

		oReader.ReadNextNode(); // XmlOptions
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while(oReader.ReadNextSiblingNode(nCurDepth))
		{
			CWCharWrapper sName = oReader.GetName();
			if (_T("fileOptions") == sName)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if (oReader, _T("fileType"), fileType)
				WritingElement_ReadAttributes_Read_else_if (oReader, _T("codePage"), codePage)
				WritingElement_ReadAttributes_Read_else_if (oReader, _T("delimiter"), delimiter)
				WritingElement_ReadAttributes_Read_else_if (oReader, _T("saveFileType"), saveFileType)
				WritingElement_ReadAttributes_End(oReader)
				
				if (fileType.IsInit())
					result = (BYTE)fileType->GetValue();
				if (codePage.IsInit())
					nCodePage = (UINT)codePage->GetValue();
				if (saveFileType.IsInit())
					cSaveFileType = (BYTE)saveFileType->GetValue();
				if (delimiter.IsInit())
				{
					const CString& sDelimiter = delimiter.get();
					if (0 < sDelimiter.GetLength())
						wcDelimiter = sDelimiter.GetAt(0);
				}
				break;
			}
		}

		return;
	}
}
