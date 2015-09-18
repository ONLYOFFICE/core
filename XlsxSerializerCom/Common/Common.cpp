#include "Common.h"

#include "BinReaderWriterDefines.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"
#include "../../Common/Base64.h"
#include "../../DesktopEditor/common/Types.h"
#ifndef DISABLE_FILE_DOWNLOADER
#include "../../Common/FileDownloader.h"
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
		oDownloader.Start( 1 );
		while ( oDownloader.IsRunned() )
		{
			::Sleep( 10 );
		}
		CString strFileName;
		if ( oDownloader.IsFileDownloaded() )
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
