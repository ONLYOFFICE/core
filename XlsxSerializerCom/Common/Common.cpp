#include "Common.h"

#include "BinReaderWriterDefines.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"
#include "../../Common/Base64.h"
#include "../../DesktopEditor/common/Types.h"
#include "../../ASCOfficeDocxFile2/BinReader/FileDownloader.h"
#include "../../DesktopEditor/common/file.h"

namespace SerializeCommon
{
	CString DownloadImage(const CString& strFile)
	{
		CFileDownloader oDownloader(strFile, FALSE);
		oDownloader.Start( 1 );
		while ( oDownloader.IsRunned() )
		{
			::Sleep( 10 );
		}
		CString strFileName;
		if ( oDownloader.IsFileDownloaded() )
		{
			strFileName = oDownloader.GetFilePath();
		}
		return strFileName;
	}
	VOID convertBase64ToImage (NSFile::CFileBinary& oFile, CString &pBase64)
	{
		INT nUTF8Len = WideCharToMultiByte (CP_UTF8, 0, pBase64, pBase64.GetLength (), NULL, NULL, NULL, NULL);
		CHAR *pUTF8String = new CHAR [nUTF8Len + 1];
		memset(pUTF8String, 0, sizeof (CHAR) * (nUTF8Len + 1));

		WideCharToMultiByte (CP_UTF8, 0, pBase64, -1, pUTF8String, nUTF8Len, NULL, NULL);
		CStringA sUnicode; sUnicode = pUTF8String;
		delete[] pUTF8String;

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
		wchar_t path_buffer[_MAX_PATH];
		wchar_t drive[_MAX_DRIVE];
		wchar_t dir[_MAX_DIR];
		wchar_t fname[_MAX_FNAME];
		wchar_t ext[_MAX_EXT];
		_wsplitpath(sSourcePath, drive, dir, fname, ext);
		_tmakepath(path_buffer, drive, dir, fname, sTargetExt);
		return CString(path_buffer);
	}
	void ReadFileType(CString& sXMLOptions, BYTE& result, UINT& nCodePage, WCHAR& wcDelimiter)
	{
		result = BinXlsxRW::c_oFileTypes::XLSX;
		nCodePage = CP_UTF8;
		wcDelimiter = _T(',');
		nullable<SimpleTypes::CUnsignedDecimalNumber<>> fileType;
		nullable<SimpleTypes::CUnsignedDecimalNumber<>> codePage;
		nullable<CString> delimiter;

		// Read options
		XmlUtils::CXmlLiteReader oReader;
		if (TRUE != oReader.FromString(sXMLOptions) || TRUE != oReader.IsValid())
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
				WritingElement_ReadAttributes_End(oReader)
				result = (BYTE)fileType->GetValue();
				nCodePage = (UINT)codePage->GetValue();
				const CString& sDelimiter = delimiter.get();
				if (0 < sDelimiter.GetLength())
					wcDelimiter = sDelimiter.GetAt(0);
				break;
			}
		}

		return;
	}
}