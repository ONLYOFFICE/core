/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include "../../../XlsxFormat/Xlsx.h"
#include "../../../Common/SimpleTypes_Shared.h"
#include "../../../../Common/Base64.h"
#include "../../../../DesktopEditor/common/Types.h"
#include "../../../../DesktopEditor/raster/ImageFileFormatChecker.h"

#ifndef DISABLE_FILE_DOWNLOADER
	#include "../../../../Common/Network/FileTransporter/include/FileTransporter.h"
#endif
#include "../../../../DesktopEditor/common/File.h"

namespace SerializeCommon
{
    std::wstring DownloadImage(const std::wstring& strFile)
	{
#ifndef DISABLE_FILE_DOWNLOADER
        std::wstring strFileName;
		
        NSNetwork::NSFileTransport::CFileDownloader oDownloader(strFile, false);
		if ( oDownloader.DownloadSync() )
		{
			strFileName = oDownloader.GetFilePath();
			
			CImageFileFormatChecker checker;
			if (false == checker.isImageFile(strFileName))
			{
				strFileName.clear();
			}  
		}
		return strFileName;
#else
		return L"";
#endif
	}
    VOID convertBase64ToImage (NSFile::CFileBinary& oFile, std::wstring &pBase64)
	{
		BYTE* pUtf8 = NULL;
		long nUtf8Size;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pBase64.c_str(), pBase64.length(), pUtf8, nUtf8Size);
        std::string sUnicode((char*)pUtf8, nUtf8Size);
		RELEASEARRAYOBJECTS(pUtf8);

		//Убираем "data:image/jpg;base64,"
		int nShift = 0;
        int nIndex = sUnicode.find("base64,");
		if(-1 != nIndex)
		{
			nShift = nIndex + 7;
		}
		// Получаем размер файла
        LONG lFileSize = sUnicode.length () - nShift;
		INT nDstLength = lFileSize;
		BYTE *pBuffer = new BYTE [lFileSize];
		memset(pBuffer, 0, lFileSize);
        Base64::Base64Decode (sUnicode.c_str() + nShift, lFileSize, pBuffer, &nDstLength);

		CImageFileFormatChecker checker;
		std::wstring detectImageExtension = checker.DetectFormatByData(pBuffer, nDstLength);

		if (false == detectImageExtension.empty())
		{
			oFile.WriteFile(pBuffer, nDstLength);
		}

		RELEASEARRAYOBJECTS (pBuffer);
	}

	long Round(double val)
	{
		return (long)(val+ 0.5);
	}
    std::wstring changeExtention(const std::wstring& sSourcePath, const std::wstring& sTargetExt)
	{
        int nIndex = sSourcePath.rfind('.');
		if(-1 != nIndex)
            return sSourcePath.substr(0, nIndex + 1) + sTargetExt;
		return sSourcePath;
	}
	void ReadFileType(const std::wstring& sXMLOptions, BYTE& result, UINT& nCodePage, std::wstring& sDelimiter, BYTE& cSaveFileType)
	{
		result = BinXlsxRW::c_oFileTypes::XLSX;
		nCodePage = 46;		//default 46 временно CP_UTF8
		sDelimiter = L";"; // default
		cSaveFileType = BinXlsxRW::c_oFileTypes::XLSX;// default

		nullable<SimpleTypes::CUnsignedDecimalNumber> fileType;
		nullable<SimpleTypes::CUnsignedDecimalNumber> codePage;
		nullable<SimpleTypes::CUnsignedDecimalNumber> saveFileType;
        nullable<std::wstring> delimiter;

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
			std::wstring sName = oReader.GetName();
			if (L"fileOptions" == sName)
			{
				WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if (oReader, L"fileType", fileType)
				WritingElement_ReadAttributes_Read_else_if (oReader, L"codePage", codePage)
				WritingElement_ReadAttributes_Read_else_if (oReader, L"delimiter", delimiter)
				WritingElement_ReadAttributes_Read_else_if (oReader, L"saveFileType", saveFileType)
				WritingElement_ReadAttributes_End(oReader)
				
				if (fileType.IsInit())
					result = (BYTE)fileType->GetValue();
				if (codePage.IsInit())
					nCodePage = (UINT)codePage->GetValue();
				if (saveFileType.IsInit())
					cSaveFileType = (BYTE)saveFileType->GetValue();
				if (delimiter.IsInit())
				{
					sDelimiter = delimiter.get();
				}
				break;
			}
		}

		return;
	}

	CommentData::CommentData()
	{
		bSolved = false;
		bDocument = false;
	}
	CommentData::~CommentData()
	{
		for(size_t i = 0, length = aReplies.size(); i < length; ++i)
			delete aReplies[i];
		aReplies.clear();
	}
}
