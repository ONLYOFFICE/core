#include "BinaryFileReaderWriter.h"

#include "../../Common/DocxFormat/Source/Base/Nullable.h"
#include "BinReaderWriterDefines.h"

#include "../../Common/Base64.h"

#include "./imagemanager.h"

#include "./XmlWriter.h"
#include "BinaryFileReaderWriter.h"
#include "../PPTXFormat/FileContainer.h"
#include "../PPTXFormat/DocxFormat/WritingElement.h"
#include "../../ASCOfficeDocxFile2/DocWrapper/DocxSerializer.h"
#include "FontPicker.h"

#include "../../DesktopEditor/common/File.h"

#define BYTE_SIZEOF		sizeof(BYTE)
#define UINT16_SIZEOF	sizeof(_UINT16)
#define UINT32_SIZEOF	sizeof(_UINT32)
#define DOUBLE_SIZEOF	sizeof(double)

#define CHAR_SIZEOF		sizeof(CHAR)
#define INT16_SIZEOF	sizeof(_INT16)
#define INT32_SIZEOF	sizeof(_INT32)
#define INT64_SIZEOF	sizeof(_INT64)

#define DOUBLE_MAIN		10000

#if defined(_WIN32) || defined (_WIN64)
	#include "../../Common/DocxFormat/Source/Base/unicode_util.h"
#endif

namespace NSBinPptxRW
{
	inline _INT32 __strlen(const char* str)
	{
		const char* s = str;
		for (; *s != 0; ++s);
		return (_INT32)(s - str);
	}
	inline _INT32 __wstrlen(const wchar_t* str)
	{
		const wchar_t* s = str;
		for (; *s != 0; ++s);
		return (_INT32)(s - str);
	}

	CMasterSlideInfo::CMasterSlideInfo() : m_arLayoutIndexes(), m_arLayoutImagesBase64()
	{
		m_lThemeIndex = 0;
		m_strImageBase64 = "";
	}

	CCommonWriter::CCommonWriter()
	{
		m_pNativePicker = NULL;
		m_pFontPicker = NULL;
		m_bDeleteFontPicker = true;
		m_pImageManager = new NSShapeImageGen::CImageManager();
	}
	CCommonWriter::~CCommonWriter()
	{
		m_pNativePicker = NULL;
		if(m_bDeleteFontPicker)
			RELEASEOBJECT(m_pFontPicker);
		RELEASEOBJECT(m_pImageManager);
	}
	void CCommonWriter::CreateFontPicker(COfficeFontPicker* pPicker)
	{
		if(m_bDeleteFontPicker)
			RELEASEOBJECT(m_pFontPicker);
		m_pNativePicker = NULL;
		if (pPicker != NULL)
		{
			m_pFontPicker = pPicker;
			m_bDeleteFontPicker = false;
		}
		else
		{
			m_pFontPicker = new COfficeFontPicker();
			m_bDeleteFontPicker = true;
		}

		m_pNativePicker = m_pFontPicker->GetNativePicker();
	}
	void CCommonWriter::CheckFontPicker()
	{
		if (NULL == m_pFontPicker)
			CreateFontPicker(NULL);
	}

	CImageManager2::CImageManager2() : m_mapImages(), m_lIndexNextImage(0), m_lIndexNextOle(0)
	{
        m_bIsWord = false;
	}
	CImageManager2::~CImageManager2()
	{
	}
	void CImageManager2::Clear()
	{
		m_mapImages.clear();
		m_lIndexNextImage = 1;
		m_lIndexNextOle = 1;
	}
	void CImageManager2::SetDstMedia(const CString& strDst)
	{
		m_strDstMedia = strDst;
	}
	CString CImageManager2::GetDstMedia()
	{
		return m_strDstMedia;
	}
	void CImageManager2::SetDstEmbed(const CString& strDst)
	{
		m_strDstEmbed = strDst;
	}
	CString CImageManager2::GetDstEmbed()
	{
		return m_strDstEmbed;
	}
	int CImageManager2::IsDisplayedImage(const CString& strInput)
	{
		int nRes = 0;
		//шаблон display[N]image.ext
		CString sFind1 = _T("display");
		int nIndex1 = strInput.Find(sFind1);
		if(-1 != nIndex1)
		{
			if(nIndex1 + sFind1.GetLength() < strInput.GetLength())
			{
				TCHAR cRes = strInput[nIndex1 + sFind1.GetLength()];
				if('1' <= cRes && cRes <= '6')
				{
					int nImageIndex = nIndex1 + sFind1.GetLength() + 1;
					if(nImageIndex == strInput.Find(_T("image"), nImageIndex))
						nRes = cRes - '0';
				}

			}
		}
		return nRes;
	}
	CImageManager2Info CImageManager2::GenerateImage(const CString& strInput, CString strBase64Image)
	{
		if (IsNeedDownload(strInput))
			return DownloadImage(strInput);

		std::map<CString, CImageManager2Info>::const_iterator pPair = m_mapImages.find ((_T("") == strBase64Image) ? strInput : strBase64Image);

		if (pPair != m_mapImages.end())
			return pPair->second;

		CString strExts = _T(".jpg");
		int nIndexExt = strInput.ReverseFind(TCHAR('.'));
		if (-1 != nIndexExt)
			strExts = strInput.Mid(nIndexExt);

		CString strOleImage = _T("");
		CString strOleImageProperty = _T("");
		CString strImage = strInput;
		int nDisplayType = IsDisplayedImage(strInput);
		if (0 != nDisplayType)
		{
			OOX::CPath oPath = strInput;
			CString strFolder = oPath.GetDirectory();
			CString strFileName = oPath.GetFilename();

			strFileName.Delete(strFileName.GetLength() - 4, 4);

			if(0 != (nDisplayType & 1))
			{
				CString strVector = strFolder + strFileName + _T(".wmf");
				if (OOX::CSystemUtility::IsFileExist(strVector))
				{
					strImage = strVector;
					strExts = _T(".wmf");
				}
			}
			if(0 != (nDisplayType & 2))
			{
				CString strVector = strFolder + strFileName + _T(".emf");
				if (OOX::CSystemUtility::IsFileExist(strVector))
				{
					strImage = strVector;
					strExts = _T(".emf");
				}
			}
			if(0 != (nDisplayType & 4))
			{
				CString strOle = strFolder + strFileName + _T(".bin");
				if (OOX::CSystemUtility::IsFileExist(strOle))
					strOleImage = strOle;
				CString strOleProperty = strFolder + strFileName + _T(".txt");
				if (OOX::CSystemUtility::IsFileExist(strOleProperty))
					strOleImageProperty = strOleProperty;
			}
		}
		CImageManager2Info oImageManagerInfo = GenerateImageExec(strImage, strExts, strOleImage, strOleImageProperty);

		if (_T("") == strBase64Image)
			m_mapImages[strInput] = oImageManagerInfo;
		else
			m_mapImages [strBase64Image] = oImageManagerInfo;
		return oImageManagerInfo;
	}
	CImageManager2Info CImageManager2::GenerateImageExec(const CString& strInput, const CString& sExts, const CString& strOleImage, const CString& strOleImageProperty)
	{
		CImageManager2Info oImageManagerInfo;
		CString strExts = sExts;
		CString strImage;
		strImage.Format(_T("image%d"), m_lIndexNextImage++);
		if ((_T(".jpg") == strExts) || (_T(".jpeg") == strExts) || (_T(".png") == strExts) || (_T(".emf") == strExts) || (_T(".wmf") == strExts))
		{
            OOX::CPath pathOutput = m_strDstMedia + FILE_SEPARATOR_STR + strImage + strExts;
			// теперь нужно скопировать картинку
            if (pathOutput.GetPath() != strInput)
                CDirectory::CopyFile(strInput, pathOutput.GetPath(), NULL, NULL);
		}
		else
		{
			// content types!!!
			strExts = _T(".png");
            OOX::CPath pathOutput = m_strDstMedia + FILE_SEPARATOR_STR + strImage + strExts;
            SaveImageAsPng(strInput, pathOutput.GetPath());
		}
		if (!m_bIsWord)
			strImage  = _T("../media/") + strImage + strExts;
		else
			strImage  = _T("media/") + strImage + strExts;

		if (_T("") != strOleImage)
		{
			CString strImageOle;
			strImageOle.Format(_T("oleObject%d.bin"), m_lIndexNextOle++);
			OOX::CPath pathOutputOle = m_strDstEmbed + FILE_SEPARATOR_STR + strImageOle;
			CString strOleImageOut = pathOutputOle.GetPath();
			CDirectory::CopyFile(strOleImage, strOleImageOut, NULL, NULL);

			if (!m_bIsWord)
				strImageOle = _T("../embeddings/") + strImageOle;
			else
				strImageOle = _T("embeddings/") + strImageOle;
			oImageManagerInfo.m_sOlePath = strImageOle;

			if(!strOleImageProperty.IsEmpty())
			{
				std::wstring sOleProperty;
				NSFile::CFileBinary::ReadAllTextUtf8(string2std_string(strOleImageProperty), sOleProperty);
				oImageManagerInfo.m_sOleProperty = std_string2string(sOleProperty);
			}
		}

		oImageManagerInfo.m_sImagePath = strImage;
		return oImageManagerInfo;
	}
	void CImageManager2::SaveImageAsPng(const CString& strFileSrc, const CString& strFileDst)
	{
		CBgraFrame oBgraFrame;
		if(oBgraFrame.OpenFile(string2std_string(strFileSrc)))
			oBgraFrame.SaveFile(string2std_string(strFileDst), _CXIMAGE_FORMAT_PNG);
	}

	void CImageManager2::SaveImageAsJPG(const CString& strFileSrc, const CString& strFileDst)
	{
		CBgraFrame oBgraFrame;
		if(oBgraFrame.OpenFile(string2std_string(strFileSrc)))
			oBgraFrame.SaveFile(string2std_string(strFileDst), _CXIMAGE_FORMAT_JPG);
	}

	bool CImageManager2::IsNeedDownload(const CString& strFile)
	{
		int n1 = strFile.Find(_T("www"));
		int n2 = strFile.Find(_T("http"));
		int n3 = strFile.Find(_T("ftp"));
		int n4 = strFile.Find(_T("https://"));

		if (((n1 >= 0) && (n1 < 10)) || ((n2 >= 0) && (n2 < 10)) || ((n3 >= 0) && (n3 < 10)) || ((n4 >= 0) && (n4 < 10)))
			return true;
		return false;
	}
	CImageManager2Info CImageManager2::DownloadImage(const CString& strUrl)
	{
		std::map<CString, CImageManager2Info>::const_iterator pPair = m_mapImages.find (strUrl);

		if (pPair != m_mapImages.end())
			return pPair->second;

		CString strExts = _T(".jpg");
		int nIndexExt = strUrl.ReverseFind(TCHAR('.'));
		if (-1 != nIndexExt)
			strExts = strUrl.Mid(nIndexExt);

		CString strImage;
		CString strOleImage;
		CString strOleImageProperty;
		int nDisplayType = IsDisplayedImage(strUrl);
		if(0 != nDisplayType)
		{
			CString strInputMetafile = strUrl.Left(strUrl.GetLength() - strExts.GetLength());
			CString sDownloadRes;
			//todo
			if(0 != (nDisplayType & 4))
			{
				strOleImage = DownloadImageExec(strInputMetafile + _T(".bin"));
				strOleImageProperty = DownloadImageExec(strInputMetafile + _T(".txt"));
			}

			if(0 != (nDisplayType & 1))
			{
				strImage = DownloadImageExec(strInputMetafile + _T(".wmf"));
				strExts = _T(".wmf");
			}
			else if(0 != (nDisplayType & 2))
			{
				strImage = DownloadImageExec(strInputMetafile + _T(".emf"));
				strExts = _T(".emf");
			}
			else
				strImage = DownloadImageExec(strUrl);
		}
		else
			strImage = DownloadImageExec(strUrl);
		CImageManager2Info oImageManagerInfo;
		if (!strImage.IsEmpty())
		{
			oImageManagerInfo = GenerateImageExec(strImage, strExts, strOleImage, strOleImageProperty);
			CDirectory::DeleteFile(strImage);
		}
		if (!strOleImage.IsEmpty())
			CDirectory::DeleteFile(strOleImage);
		if (!strOleImageProperty.IsEmpty())
			CDirectory::DeleteFile(strOleImageProperty);

		m_mapImages[strUrl] = oImageManagerInfo;
		return oImageManagerInfo;
	}
	CString CImageManager2::DownloadImageExec(const CString& strFile)
	{
#ifndef DISABLE_FILE_DOWNLOADER
        CFileDownloader oDownloader(strFile, false);
		oDownloader.Start( 1 );
		while ( oDownloader.IsRunned() )
		{
			::Sleep( 10 );
		}

		if ( oDownloader.IsFileDownloaded() )
		{
			return oDownloader.GetFilePath();
		}
#endif
		return _T("");
	}

	CBinaryFileWriter::CSeekTableEntry::CSeekTableEntry()
	{
		Type = 0;
		SeekPos = 0;
	}
	BYTE* CBinaryFileWriter::GetBuffer()
	{
		return m_pStreamData;
	}
	_UINT32 CBinaryFileWriter::GetPosition()
	{
		return m_lPosition;
	}
	void CBinaryFileWriter::SetPosition(const _UINT32& lPosition)
	{
		m_lPosition = lPosition;
		m_pStreamCur = m_pStreamData + m_lPosition;
	}
	void CBinaryFileWriter::Skip(const _UINT32& lSize)
	{
		CheckBufferSize(lSize);

		m_lPosition += lSize;
		m_pStreamCur = m_pStreamData + m_lPosition;
	}
	double CBinaryFileWriter::GetWidthMM()
	{
		if (m_lWidthCurShape == 0)
			return -1;
		return (double)m_lWidthCurShape / 36000;
	}
	double CBinaryFileWriter::GetHeightMM()
	{
		if (m_lHeightCurShape == 0)
			return -1;
		return (double)m_lHeightCurShape / 36000;
	}
	void CBinaryFileWriter::ClearShapeCurSizes()
	{
		m_lWidthCurShape = 0;
		m_lHeightCurShape = 0;
	}
	void CBinaryFileWriter::Clear()
	{
		m_lSize		= 0;
		m_lPosition = 0;

		m_pStreamData	= NULL;
		m_pStreamCur	= NULL;

		m_lStackPosition = 0;
		memset(m_arStack, 0, MAX_STACK_SIZE * sizeof(_UINT32));

		m_lWidthCurShape = 0;
		m_lHeightCurShape = 0;
	}

	void CBinaryFileWriter::SetMainDocument(BinDocxRW::CDocxSerializer* pMainDoc)
	{
		//RELEASEINTERFACE(m_pMainDocument);
		m_pMainDocument = pMainDoc;
		//ADDREFINTERFACE(m_pMainDocument);
	}

	void CBinaryFileWriter::ClearNoAttack()
	{
		m_lPosition = 0;
		m_pStreamCur	= m_pStreamData;

		m_lStackPosition = 0;
		memset(m_arStack, 0, MAX_STACK_SIZE * sizeof(_UINT32));
	}

	void CBinaryFileWriter::CheckBufferSize(_UINT32 lPlus)
	{
		if (NULL != m_pStreamData)
		{
			size_t nNewSize = m_lPosition + lPlus;

			if (nNewSize >= m_lSize)
			{
				while (nNewSize >= m_lSize)
				{
					m_lSize *= 2;
				}

				BYTE* pNew = new BYTE[m_lSize];
				memcpy(pNew, m_pStreamData, m_lPosition);

				RELEASEARRAYOBJECTS(m_pStreamData);
				m_pStreamData = pNew;

				m_pStreamCur = m_pStreamData + m_lPosition;
			}
		}
		else
		{
			m_lSize		= 1024 * 1024; // 1Mb
			m_pStreamData	= new BYTE[m_lSize];

			m_lPosition = 0;
			m_pStreamCur = m_pStreamData;

			CheckBufferSize(lPlus);
		}
	}

	void CBinaryFileWriter::WriteBYTE(const BYTE& lValue)
	{			
		CheckBufferSize(BYTE_SIZEOF);

		*m_pStreamCur = lValue; 
		m_lPosition += BYTE_SIZEOF;
		m_pStreamCur += BYTE_SIZEOF;
	}
	void CBinaryFileWriter::WriteSBYTE(const signed char& lValue)
	{
		CheckBufferSize(BYTE_SIZEOF);

		if (lValue < 0)
			*m_pStreamCur = (lValue + 256);
		else
			*m_pStreamCur = lValue;
		m_lPosition += BYTE_SIZEOF;
		m_pStreamCur += BYTE_SIZEOF;
	}
	void CBinaryFileWriter::WriteBOOL(const bool& bValue)
	{			
		WriteBYTE((bValue == true) ? 1 : 0);
	}
	void CBinaryFileWriter::WriteUSHORT(const _UINT16& lValue)
	{
		CheckBufferSize(UINT16_SIZEOF);
#ifdef _IOS
        memcpy(m_pStreamCur, &lValue, sizeof(_UINT16));
#else
        *((_UINT16*)m_pStreamCur) = lValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += UINT16_SIZEOF;
		m_pStreamCur += UINT16_SIZEOF;
	}
	void CBinaryFileWriter::WriteULONG(const _UINT32& lValue)
	{
		CheckBufferSize(UINT32_SIZEOF);
#ifdef _IOS
        memcpy(m_pStreamCur, &lValue, sizeof(_UINT32));
#else
        *((_UINT32*)m_pStreamCur) = lValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += UINT32_SIZEOF;
		m_pStreamCur += UINT32_SIZEOF;
	}
	void CBinaryFileWriter::WriteLONG(const _INT32& lValue)
	{
		CheckBufferSize(INT32_SIZEOF);
#ifdef _IOS
        memcpy(m_pStreamCur, &lValue, sizeof(_INT32));
#else
        *((_INT32*)m_pStreamCur) = lValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += INT32_SIZEOF;
		m_pStreamCur += INT32_SIZEOF;
	}
	void CBinaryFileWriter::WriteLONG64(const _INT64& lValue)
	{
		CheckBufferSize(INT64_SIZEOF);
#ifdef _IOS
        memcpy(m_pStreamCur, &lValue, sizeof(_INT64));
#else
        *((_INT64*)m_pStreamCur) = lValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += INT64_SIZEOF;
		m_pStreamCur += INT64_SIZEOF;
	}
	void CBinaryFileWriter::WriteINT(const _INT32& lValue)
	{
		CheckBufferSize(INT32_SIZEOF);
#ifdef _IOS
        memcpy(m_pStreamCur, &lValue, sizeof(_INT32));
#else
        *((_INT32*)m_pStreamCur) = lValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += INT32_SIZEOF;
		m_pStreamCur += INT32_SIZEOF;
	}
	void CBinaryFileWriter::WriteDouble64(const double& dValue)
	{
		_INT64 _val = (_INT64)(dValue * 100000);
		WriteLONG64(_val);
	}
	void CBinaryFileWriter::WriteDouble(const double& dValue)
	{
		_INT64 _val = (_INT64)(dValue * 100000);

		if (_val > 0x7fffffff)
		{
			WriteLONG(0x7fffffff);
		}
		else if ( _val < -0x7fffffff)
		{
			WriteLONG(-0x7fffffff);
		}		
		else
		{
			WriteLONG((long)_val);
		}
	}
	void CBinaryFileWriter::WriteDoubleReal(const double& dValue)
	{
		CheckBufferSize(DOUBLE_SIZEOF);
#ifdef _IOS
        memcpy(m_pStreamCur, &dValue, sizeof(double));
#else
		*((double*)m_pStreamCur) = dValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += DOUBLE_SIZEOF;
		m_pStreamCur += DOUBLE_SIZEOF;
	}
	void CBinaryFileWriter::WriteStringW(const WCHAR* sBuffer)
	{
		_INT32 lSize = __wstrlen(sBuffer);

        _INT32 lSizeMem =0;

        if (sizeof(wchar_t) == 4)
        {
            lSizeMem = lSize * sizeof(UTF16);

            CheckBufferSize(UINT32_SIZEOF + lSizeMem);
#ifdef _IOS
            memcpy(m_pStreamCur, &lSizeMem, sizeof(_UINT32));
#else
            *((_UINT32*)m_pStreamCur) = lSizeMem; // EXC_ARM_DA_ALIGN on ios
#endif
            m_lPosition += UINT32_SIZEOF;
            m_pStreamCur += UINT32_SIZEOF;

            UTF16* pStrUtf16 = (UTF16 *) m_pStreamCur;
            UTF32 *pStrUtf32 = (UTF32 *) sBuffer;

            // this values will be modificated
            UTF16 *pStrUtf16_Conv = pStrUtf16;
            const UTF32 *pStrUtf32_Conv = pStrUtf32;

            ConversionResult eUnicodeConversionResult =
            ConvertUTF32toUTF16 (&pStrUtf32_Conv
                                , &pStrUtf32[lSize]
                                , &pStrUtf16_Conv
                                , &pStrUtf16 [lSize]
                                , strictConversion);

            if (conversionOK != eUnicodeConversionResult)
            {
            }
        }
        else
        {
            lSizeMem = lSize * sizeof(wchar_t);

            CheckBufferSize(UINT32_SIZEOF + lSizeMem);
#ifdef _IOS
            memcpy(m_pStreamCur, &lSizeMem, sizeof(_UINT32));
#else
            *((_UINT32*)m_pStreamCur) = lSizeMem; // EXC_ARM_DA_ALIGN on ios
#endif
            m_lPosition += UINT32_SIZEOF;
            m_pStreamCur += UINT32_SIZEOF;

            memcpy(m_pStreamCur, sBuffer, lSizeMem);
        }

        m_lPosition += lSizeMem;
        m_pStreamCur += lSizeMem;
    }
	void CBinaryFileWriter::WriteBYTEArray(const BYTE* pBuffer, size_t len)
	{
		CheckBufferSize(len);
		memcpy(m_pStreamCur, pBuffer, len);
		m_lPosition += (_UINT32)len;
		m_pStreamCur += len;
	}
	void CBinaryFileWriter::WriteStringA(const char* sBuffer)
	{
		_INT32 lSize = __strlen(sBuffer);
		_INT32 lSizeMem = lSize * sizeof(char);

		CheckBufferSize(UINT32_SIZEOF + lSizeMem);
#ifdef _IOS
        memcpy(m_pStreamCur, &lSizeMem, sizeof(_UINT32));
#else
        *((_UINT32*)m_pStreamCur) = lSizeMem; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += UINT32_SIZEOF;
		m_pStreamCur += UINT32_SIZEOF;

		memcpy(m_pStreamCur, sBuffer, lSizeMem);
		m_lPosition += lSizeMem;
		m_pStreamCur += lSizeMem;
	}
	void CBinaryFileWriter::WriteStringA(CStringA& sBuffer)
	{
		char* pChars = sBuffer.GetBuffer();
		WriteStringA(pChars);
	}
	void CBinaryFileWriter::WriteStringW(CString& sBuffer)
	{
		WCHAR* pChars = sBuffer.GetBuffer();
		WriteStringW(pChars);
	}
    void CBinaryFileWriter::WriteStringW2(const WCHAR* sBuffer)
    {
        if (sizeof(wchar_t) == 4)
        {
            WriteStringW2_4(sBuffer);
        }else
        {
            WriteStringW2_2(sBuffer);
        }
    }
    void CBinaryFileWriter::WriteStringW2_2(const WCHAR* sBuffer)
    {
        _INT32 lSize = __wstrlen(sBuffer);
        _INT32 lSizeMem = lSize * sizeof(UTF16);

        CheckBufferSize(UINT32_SIZEOF + lSizeMem);
#ifdef _IOS
        memcpy(m_pStreamCur, &lSize, sizeof(_UINT32));
#else
        *((_UINT32*)m_pStreamCur) = lSize; // EXC_ARM_DA_ALIGN on ios
#endif
        m_lPosition += UINT32_SIZEOF;
        m_pStreamCur += UINT32_SIZEOF;

        memcpy(m_pStreamCur, sBuffer, lSizeMem);
        m_lPosition += lSizeMem;
        m_pStreamCur += lSizeMem;
    }

    void CBinaryFileWriter::WriteStringW2_4(const WCHAR* sBuffer)
	{
		_INT32 lSize = __wstrlen(sBuffer);
		_INT32 lSizeMem = lSize * sizeof(wchar_t);

		CheckBufferSize(UINT32_SIZEOF + lSizeMem);
#ifdef _IOS
        memcpy(m_pStreamCur, &lSize, sizeof(_UINT32));
#else
        *((_UINT32*)m_pStreamCur) = lSize; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += UINT32_SIZEOF;
		m_pStreamCur += UINT32_SIZEOF;

        UTF16* pStrUtf16 = (UTF16 *) m_pStreamCur;
        UTF32 *pStrUtf32 = (UTF32 *) sBuffer;

        // this values will be modificated
        UTF16 *pStrUtf16_Conv = pStrUtf16;
        const UTF32 *pStrUtf32_Conv = pStrUtf32;

        ConversionResult eUnicodeConversionResult =
        ConvertUTF32toUTF16 (&pStrUtf32_Conv
                            , &pStrUtf32[lSize]
                            , &pStrUtf16_Conv
                            , &pStrUtf16 [lSize]
                            , strictConversion);

        if (conversionOK != eUnicodeConversionResult)
        {
        }
        m_lPosition += lSizeMem;
		m_pStreamCur += lSizeMem;
	}
	void CBinaryFileWriter::WriteStringW2(CString& sBuffer)
	{
		WriteStringW2(sBuffer.GetBuffer());
	}
	void CBinaryFileWriter::WriteStringW3(const WCHAR* sBuffer)
	{   
        if (sizeof(wchar_t) == 4)
        {
            WriteStringW3_4(sBuffer);
        }else
        {
            WriteStringW3_2(sBuffer);
        }
	}
    void CBinaryFileWriter::WriteStringW3_2(const WCHAR* sBuffer)
    {
        _INT32 lSize = __wstrlen(sBuffer);

        _INT32 lSizeMem = lSize * sizeof(wchar_t);

        CheckBufferSize(lSizeMem);

        memcpy(m_pStreamCur, sBuffer, lSizeMem);
        m_lPosition += lSizeMem;
        m_pStreamCur += lSizeMem;
    }
    void CBinaryFileWriter::WriteStringW3_4(const WCHAR* sBuffer)
    {
        _INT32 lSize = __wstrlen(sBuffer);

        _INT32 lSizeMem = lSize * sizeof(UTF16);

        CheckBufferSize(lSizeMem);

        UTF16* pStrUtf16 = (UTF16 *) m_pStreamCur;
        UTF32 *pStrUtf32 = (UTF32 *) sBuffer;

        // this values will be modificated
        UTF16 *pStrUtf16_Conv = pStrUtf16;
        const UTF32 *pStrUtf32_Conv = pStrUtf32;

        ConversionResult eUnicodeConversionResult =
        ConvertUTF32toUTF16 (&pStrUtf32_Conv
                            , &pStrUtf32[lSize]
                            , &pStrUtf16_Conv
                            , &pStrUtf16 [lSize]
                            , strictConversion);

        if (conversionOK != eUnicodeConversionResult)
        {
        }

        m_lPosition += lSizeMem;
        m_pStreamCur += lSizeMem;
    }
    void CBinaryFileWriter::WriteStringW3(CString& sBuffer)
	{
		WriteStringW3(sBuffer.GetBuffer());
	}
	CBinaryFileWriter::CBinaryFileWriter()
	{
		m_strMainFolder = _T("");
		m_pMainDocument = NULL;
		m_pCommon = new CCommonWriter();
		m_pCommonRels = new NSCommon::smart_ptr<PPTX::CCommonRels>();
		ThemeDoc = new NSCommon::smart_ptr<PPTX::FileContainer>();
		ClrMapDoc = new NSCommon::smart_ptr<PPTX::WritingElement>();
		Clear();
	}
	CBinaryFileWriter::~CBinaryFileWriter()
	{
		RELEASEARRAYOBJECTS(m_pStreamData);
		//RELEASEINTERFACE(m_pMainDocument);
		RELEASEOBJECT(m_pCommon);
		RELEASEOBJECT(m_pCommonRels);
		RELEASEOBJECT(ThemeDoc);
		RELEASEOBJECT(ClrMapDoc);
	}

	void CBinaryFileWriter::StartRecord(_INT32 lType)
	{
		m_arStack[m_lStackPosition] = m_lPosition + 5; // sizeof(BYTE) + sizeof(_UINT32)
		m_lStackPosition++;
		WriteBYTE((BYTE)lType);
		WriteULONG(0);
	}
	void CBinaryFileWriter::EndRecord()
	{
		m_lStackPosition--;

        _UINT32 size_record = m_lPosition - m_arStack[m_lStackPosition];
        (*(_UINT32*)(m_pStreamData + m_arStack[m_lStackPosition] - 4)) = size_record ;
	}

	void CBinaryFileWriter::StartMainRecord(_INT32 lType)
	{
		CSeekTableEntry oEntry;
		oEntry.Type = lType;
		oEntry.SeekPos = m_lPosition;
		m_arMainTables.push_back(oEntry);
		//StartRecord(lType);
	}

	void CBinaryFileWriter::WriteReserved(size_t lCount)
	{
		CheckBufferSize(lCount);
		memset(m_pStreamCur, 0, lCount);
		m_pStreamCur += lCount;
		m_lPosition += (_UINT32)lCount;
	}

	void CBinaryFileWriter::WriteMainPart()
	{
		BYTE* pData = m_pStreamData;
		size_t nCount = m_arMainTables.size();

		for (size_t i = 0; i < nCount; i++)
		{
			*pData = (BYTE)m_arMainTables[i].Type;
			++pData;
#ifdef _IOS
            memcpy(pData, &m_arMainTables[i].SeekPos, sizeof(_INT32));
#else
            *((_INT32*)pData) = m_arMainTables[i].SeekPos; // EXC_ARM_DA_ALIGN on ios
#endif
			pData += 4;
		}
	}

	void CBinaryFileWriter::WriteString1(int type, const CString& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);

        _UINT32 lSize = (_UINT32)val.GetLength() , len = lSize;
        WriteULONG(lSize);

        len <<= 1;

		CString* s = const_cast<CString*>(&val);
		CheckBufferSize(len);

        WCHAR *buffer = s->GetBuffer();

        if (sizeof(buffer[0]) == 4)
        {
            UTF16* pStrUtf16 = (UTF16 *) m_pStreamCur;
            UTF32 *pStrUtf32 = (UTF32 *) buffer;

            // this values will be modificated
            UTF16 *pStrUtf16_Conv = pStrUtf16;
            const UTF32 *pStrUtf32_Conv = pStrUtf32;

            ConversionResult eUnicodeConversionResult =
            ConvertUTF32toUTF16 (&pStrUtf32_Conv
                                , &pStrUtf32[lSize]
                                , &pStrUtf16_Conv
                                , &pStrUtf16 [lSize]
                                , strictConversion);

            if (conversionOK != eUnicodeConversionResult)
            {
            }
        }else
        {

            memcpy(m_pStreamCur, s->GetBuffer(), len);
        }
		m_pStreamCur += len;
		m_lPosition += len;
	}
	void CBinaryFileWriter::WriteString2(int type, const NSCommon::nullable_string& val)
	{
		if (val.is_init())
			WriteString1(type, *val);
	}
	void CBinaryFileWriter::WriteString(const CString& val)
	{
        _UINT32 lSize = (_UINT32)val.GetLength() , len = lSize;
        WriteULONG(len);

        len <<= 1;

		CString* s = const_cast<CString*>(&val);
		CheckBufferSize(len);

        WCHAR *buffer = s->GetBuffer();

        if (sizeof(buffer[0]) == 4)
        {
            UTF16* pStrUtf16 = (UTF16 *) m_pStreamCur;
            UTF32 *pStrUtf32 = (UTF32 *) buffer;

            // this values will be modificated
            UTF16 *pStrUtf16_Conv = pStrUtf16;
            const UTF32 *pStrUtf32_Conv = pStrUtf32;

            ConversionResult eUnicodeConversionResult =
            ConvertUTF32toUTF16 (&pStrUtf32_Conv
                                , &pStrUtf32[lSize]
                                , &pStrUtf16_Conv
                                , &pStrUtf16 [lSize]
                                , strictConversion);

            if (conversionOK != eUnicodeConversionResult)
            {
            }
        }else
        {

            memcpy(m_pStreamCur, s->GetBuffer(), len);
        }
		m_pStreamCur += len;
		m_lPosition += len;
	}

	void CBinaryFileWriter::WriteString1Data(int type, const WCHAR* pData, _UINT32 len)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);

		WriteULONG(len);

        _UINT32 lSize = len;
        len <<= 1;

        CheckBufferSize(len);

        if (sizeof(wchar_t) == 4)
        {
            UTF16* pStrUtf16 = (UTF16 *) m_pStreamCur;
            UTF32 *pStrUtf32 = (UTF32 *) pData;

            // this values will be modificated
            UTF16 *pStrUtf16_Conv = pStrUtf16;
            const UTF32 *pStrUtf32_Conv = pStrUtf32;

            ConversionResult eUnicodeConversionResult =
            ConvertUTF32toUTF16 (&pStrUtf32_Conv
                                , &pStrUtf32[lSize]
                                , &pStrUtf16_Conv
                                , &pStrUtf16 [lSize]
                                , strictConversion);

            if (conversionOK != eUnicodeConversionResult)
            {
            }
        }else
        {
            memcpy(m_pStreamCur, (BYTE*)pData, len);
        }
		m_pStreamCur += len;
		m_lPosition += len;
	}

	void CBinaryFileWriter::WriteBool1(int type, const bool& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);
		WriteBYTE((val == true) ? 1 : 0);
	}
	void CBinaryFileWriter::WriteBool2(int type, const NSCommon::nullable_bool& val)
	{
		if (val.is_init())
			WriteBool1(type, *val);
	}

	void CBinaryFileWriter::WriteInt1(int type, const int& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);
		WriteINT(val);
	}
	void CBinaryFileWriter::WriteInt2(int type, const NSCommon::nullable_int& val)
	{
		if (val.is_init())
			WriteInt1(type, *val);
	}

	void CBinaryFileWriter::WriteDouble1(int type, const double& val)
	{
		int _val = (int)(val * 10000);
		WriteInt1(type, _val);
	}
	void CBinaryFileWriter::WriteDouble2(int type, const NSCommon::nullable_double& val)
	{
		if (val.is_init())
			WriteDouble1(type, *val);
	}

	void CBinaryFileWriter::WriteSize_t1(int type, const size_t& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);
		_UINT32 ival = (_UINT32)val;
		WriteULONG(ival);
	}
	void CBinaryFileWriter::WriteSize_t2(int type, const NSCommon::nullable_sizet& val)
	{
		if (val.is_init())
			WriteSize_t1(type, *val);
	}

	void CBinaryFileWriter::GetBase64File(const CString& sFile, CStringA& strDst64)
	{
		CFile oFile;
		HRESULT hr = oFile.OpenFile(sFile);

		if (S_OK != hr)
		{
			strDst64 = "";
			return;
		}

		DWORD dwLen = (DWORD)oFile.GetFileSize();
		BYTE* pBuffer = new BYTE[dwLen];

		oFile.SetPosition(0);
		oFile.ReadFile(pBuffer, dwLen);

		int nBase64BufferLen = Base64::Base64EncodeGetRequiredLength((int)dwLen, Base64::B64_BASE64_FLAG_NOCRLF);
        BYTE *pbBase64Buffer = new BYTE[nBase64BufferLen + 1 + 64];
        pbBase64Buffer[nBase64BufferLen] = '\0';
        //if (true == Base64::Base64Encode(pBuffer, (int)dwLen, pbBase64Buffer, &nBase64BufferLen, Base64::B64_BASE64_FLAG_NOCRLF))
        if (true == Base64_1::Base64Encode(pBuffer, (int)dwLen, pbBase64Buffer, &nBase64BufferLen))
		{
            //strDst64.SetString(pbBase64Buffer, nBase64BufferLen);
            strDst64 = (char*)pbBase64Buffer;
		}
		RELEASEARRAYOBJECTS(pbBase64Buffer);
		RELEASEARRAYOBJECTS(pBuffer);

		oFile.CloseFile();

		//DeleteFile(sFile);
	}

	void CBinaryFileWriter::WriteTheme64(_INT32 lIndex, const CString& sFile)
	{
		GetBase64File(sFile, m_pCommon->m_oRels[lIndex].m_strImageBase64);
	}

	void CBinaryFileWriter::WriteLayoutTheme64(_INT32 lIndexTheme, _INT32 lIndexLayout, const CString& sFile)
	{
		GetBase64File(sFile, m_pCommon->m_oRels[lIndexTheme].m_arLayoutImagesBase64[lIndexLayout]);
	}

	CString CBinaryFileWriter::GetFolderForGenerateImages()
	{
		return m_strMainFolder + _T("\\extract_themes");
	}

	// embedded fonts
	void CBinaryFileWriter::WriteEmbeddedFonts()
	{
		if (NULL == m_pCommon->m_pNativePicker)
			return;

		if (!m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts)
			return;

		StartMainRecord(NSBinPptxRW::NSMainTables::FontsEmbedded);

		// добавим мега шрифт
		m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckString(_T(".)abcdefghijklmnopqrstuvwxyz"));
		m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckFont(_T("Wingdings 3"), m_pCommon->m_pNativePicker->m_pFontManager);
		m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckFont(_T("Arial"), m_pCommon->m_pNativePicker->m_pFontManager);

		StartRecord(NSBinPptxRW::NSMainTables::FontsEmbedded);
		m_pCommon->m_pNativePicker->m_oEmbeddedFonts.WriteEmbeddedFonts(this);
		EndRecord();
	}

	// serialize ImageManagers
    bool CBinaryFileWriter::Serialize(NSBinPptxRW::CImageManager2* pManager, BYTE **ppArray, size_t& szCount)
	{
		pManager->Serialize(this);
        return GetSafearray(ppArray, szCount);
	}
    bool CBinaryFileWriter::Serialize(NSShapeImageGen::CImageManager* pManager, BYTE **ppArray, size_t& szCount)
	{
		pManager->Serialize(this);
        return GetSafearray(ppArray, szCount);
	}

    bool CBinaryFileWriter::GetSafearray(BYTE **ppArray, size_t& szCount)
    {
        if (NULL == ppArray)
            return false;

        _UINT32 lBinarySize = this->GetPosition();
        if (0 == lBinarySize)
            return false;

        *ppArray = new BYTE [lBinarySize];
        szCount = lBinarySize;

        memcpy(*ppArray, this->GetBuffer(), lBinarySize);
        return true;
    }
    /*
	LPSAFEARRAY CBinaryFileWriter::GetSafearray()
	{			
		_UINT32 lBinarySize = this->GetPosition();
		if (0 == lBinarySize)
			return NULL;

		SAFEARRAY* pArray = SafeArrayCreateVector(VT_UI1, lBinarySize);

		BYTE* pDataD = (BYTE*)pArray->pvData;
		BYTE* pDataS = this->GetBuffer();
		memcpy(pDataD, pDataS, lBinarySize);

		return pArray;
	}
    */
	CSlideMasterInfo::CSlideMasterInfo() : m_arLayouts()
	{
		m_lThemeIndex = -1;				
	}
	CSlideMasterInfo::CSlideMasterInfo(const CSlideMasterInfo& oSrc)
	{
		m_lThemeIndex	= oSrc.m_lThemeIndex;
		m_arLayouts		= oSrc.m_arLayouts;
	}

	CRelsGenerator::CRelsGenerator(CImageManager2* pManager) : m_lNextRelsID(1), m_mapImages()
	{
		m_pManager = pManager;
		m_pWriter = new CStringWriter();
	}
	CRelsGenerator::~CRelsGenerator()
	{
		RELEASEOBJECT(m_pWriter);
	}
	void CRelsGenerator::Clear()
	{
		m_pWriter->ClearNoAttack();
		m_lNextRelsID = 1;
		m_mapImages.clear();
		m_mapLinks.clear();
	}

	void CRelsGenerator::StartRels()
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));
	}

	void CRelsGenerator::StartTheme()
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));
	}

	void CRelsGenerator::StartMaster(int nIndexTheme, const CSlideMasterInfo& oInfo)
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));

		int nCountLayouts = (int)oInfo.m_arLayouts.size();
		for (int i = 0; i < nCountLayouts; ++i)
		{
			CString str = _T("");
			str.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout%d.xml\"/>"), 
				m_lNextRelsID++, oInfo.m_arLayouts[i] + 1);
			m_pWriter->WriteString(str);
		}

		CString s = _T("");
		s.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme%d.xml\"/>" ),
			m_lNextRelsID++, nIndexTheme + 1);
		m_pWriter->WriteString(s);
	}
	void CRelsGenerator::StartLayout(int nIndexTheme)
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));

		CString str = _T("");
		str.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"../slideMasters/slideMaster%d.xml\"/>"), 
			m_lNextRelsID++, nIndexTheme + 1);
		m_pWriter->WriteString(str);
	}
	void CRelsGenerator::StartSlide(int nIndexSlide, int nIndexLayout)
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));

		CString str = _T("");
		str.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout%d.xml\"/>"), 
			m_lNextRelsID++, nIndexLayout + 1);
		m_pWriter->WriteString(str);

		str = _T("");
		str.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide\" Target=\"../notesSlides/notesSlide%d.xml\"/>"), m_lNextRelsID++, nIndexSlide + 1);
		m_pWriter->WriteString(str);
	}
	void CRelsGenerator::StartNote(int nIndexSlide)
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));

		CString sNum = _T("");
		sNum.Format(_T("%d"), nIndexSlide + 1);
		CString strNoteSlideRels = _T("<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"../slides/slide") + sNum + _T(".xml\"/>");

		m_pWriter->WriteString(strNoteSlideRels);
		m_pWriter->WriteString(_T("<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster\" Target=\"../notesMasters/notesMaster1.xml\"/>"));

		m_lNextRelsID = 3;
	}
	void CRelsGenerator::WriteMasters(int nCount)
	{
		for (int i = 0; i < nCount; ++i)
		{
			CString strRels = _T("");
			strRels.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"slideMasters/slideMaster%d.xml\" />"), m_lNextRelsID++, i + 1);
			m_pWriter->WriteString(strRels);
		}
	}
	void CRelsGenerator::WriteThemes(int nCount)
	{
		for (int i = 0; i < nCount; ++i)
		{
			CString strRels = _T("");
			strRels.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme%d.xml\" />"), m_lNextRelsID++, i + 1);
			m_pWriter->WriteString(strRels);
		}
	}
	void CRelsGenerator::WriteSlides(int nCount)
	{
		for (int i = 0; i < nCount; ++i)
		{
			CString strRels = _T("");
			strRels.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"slides/slide%d.xml\" />"), m_lNextRelsID++, i + 1);
			m_pWriter->WriteString(strRels);
		}
	}
	void CRelsGenerator::WriteSlideComments(int nComment)
	{
		CString strRels = _T("");
		strRels.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments\" Target=\"../comments/comment%d.xml\"/>"), m_lNextRelsID++, nComment);
		m_pWriter->WriteString(strRels);
	}
	void CRelsGenerator::EndPresentationRels(const bool& bIsCommentsAuthors = false)
	{
		CString strRels0 = _T("");
		strRels0.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster\" Target=\"notesMasters/notesMaster1.xml\"/>"), m_lNextRelsID++);
		CString strRels1 = _T("");
		strRels1.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/presProps\" Target=\"presProps.xml\" />"), m_lNextRelsID++);
		CString strRels2 = _T("");
		strRels2.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/tableStyles\" Target=\"tableStyles.xml\" />"), m_lNextRelsID++);
		CString strRels3 = _T("");
		strRels3.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/viewProps\" Target=\"viewProps.xml\" />"), m_lNextRelsID++);

		m_pWriter->WriteString(strRels0);			
		m_pWriter->WriteString(strRels1);
		m_pWriter->WriteString(strRels2);
		m_pWriter->WriteString(strRels3);

		if (bIsCommentsAuthors)
		{
			CString strRels4 = _T("");
			strRels4.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/commentAuthors\" Target=\"commentAuthors.xml\"/>"), m_lNextRelsID++);
			m_pWriter->WriteString(strRels4);
		}
	}
	int CRelsGenerator::GetNextId()
	{
		return m_lNextRelsID;
	}
	void CRelsGenerator::CloseRels()
	{
		m_pWriter->WriteString(_T("</Relationships>"));
	}
	void CRelsGenerator::AddRels(const CString& strRels)
	{
		m_pWriter->WriteString(strRels);			
	}
	void CRelsGenerator::SaveRels(const CString& strFile)
	{
		CFile oFile;
		oFile.CreateFile(strFile);
		CString strMem = m_pWriter->GetData();
		oFile.WriteStringUTF8(strMem);
		oFile.CloseFile();
	}

	CRelsGeneratorInfo CRelsGenerator::WriteImage(const CString& strImagePath, CString strBase64Image = _T(""))
	{
		CImageManager2Info oImageManagerInfo = m_pManager->GenerateImage(strImagePath, strBase64Image);
		CString strImage = oImageManagerInfo.m_sImagePath;
		std::map<CString, CRelsGeneratorInfo>::iterator pPair = m_mapImages.find(strImage);

		if (m_mapImages.end() != pPair)
		{
			return pPair->second;				
		}
		CRelsGeneratorInfo oRelsGeneratorInfo;
		oRelsGeneratorInfo.m_nImageRId = m_lNextRelsID++;
		CString strRid = _T("");
		strRid.Format(_T("rId%d"), oRelsGeneratorInfo.m_nImageRId);

		CString strRels = _T("");
		strRels.Format(_T("<Relationship Id=\"%ls\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"%ls\"/>"),
			strRid, strImage);
		m_pWriter->WriteString(strRels);

		if(!oImageManagerInfo.m_sOlePath.IsEmpty())
		{
			oRelsGeneratorInfo.m_nOleRId = m_lNextRelsID++;
			oRelsGeneratorInfo.m_sOleProperty = oImageManagerInfo.m_sOleProperty;

			CString strRid = _T("");
			strRid.Format(_T("rId%d"), oRelsGeneratorInfo.m_nOleRId);

			CString strRels = _T("");
			strRels.Format(_T("<Relationship Id=\"%ls\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject\" Target=\"%ls\"/>"),
				strRid, oImageManagerInfo.m_sOlePath);
			m_pWriter->WriteString(strRels);
		}

		m_mapImages.insert(std::pair<CString, CRelsGeneratorInfo>(strImage, oRelsGeneratorInfo));
		return oRelsGeneratorInfo;
	}
	int CRelsGenerator::WriteChart(int nChartNumber, _INT32 lDocType = XMLWRITER_DOC_TYPE_PPTX)
	{
		CString strChart = _T("");

		if (lDocType == XMLWRITER_DOC_TYPE_DOCX)
		{
			strChart.Format(_T("charts/chart%d.xml"), nChartNumber);
		}
		else
		{
			strChart.Format(_T("../charts/chart%d.xml"), nChartNumber);
		}

		CString strRid = _T("");
		strRid.Format(_T("rId%d"), m_lNextRelsID++);

		CString strRels = _T("");
		strRels.Format(_T("<Relationship Id=\"%ls\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart\" Target=\"%ls\"/>"),
			strRid, strChart);
		m_pWriter->WriteString(strRels);

		return m_lNextRelsID - 1;
	}	

	int CRelsGenerator::WriteRels(const CString& bsType, const CString& bsTarget, const CString& bsTargetMode)
	{
		CString strRid = _T("");
		strRid.Format(_T("rId%d"), m_lNextRelsID++);

		CString strType = _T("Type=\"") + bsType + _T("\" ");
		CString strTarget = _T("Target=\"") + bsTarget + _T("\" ");
		CString strTargetMode = bsTargetMode.IsEmpty() ? _T("") : (_T("TargetMode=\"") + (CString)bsTargetMode + _T("\""));

		CString strRels = _T("<Relationship Id=\"") + strRid + _T("\" ") + strType + strTarget + strTargetMode + _T("/>");
		m_pWriter->WriteString(strRels);
		return m_lNextRelsID - 1;
	}

	int CRelsGenerator::WriteHyperlink(const CString& strLink, const bool& bIsActionInit)
	{
		std::map<CString, int>::iterator pPair = m_mapLinks.find(strLink);

		if (m_mapLinks.end() != pPair)
		{
			return pPair->second;				
		}

		m_mapLinks.insert(std::pair<CString, int>(strLink, m_lNextRelsID));
		CString strRid = _T("");
		strRid.Format(_T("rId%d"), m_lNextRelsID++);

		CString sLink = strLink;
		sLink.Replace(L"&",	L"&amp;");
		sLink.Replace(L"'",	L"&apos;");
		sLink.Replace(L"<",	L"&lt;");
		sLink.Replace(L">",	L"&gt;");
		sLink.Replace(L"\"",L"&quot;");

		bool bIsSlide = (0 == sLink.Find(_T("slide")));
		if (!bIsActionInit)
			bIsSlide = false;

		CString strRels = _T("");

		if (!bIsSlide)
		{
			strRels.Format(_T("<Relationship Id=\"%ls\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"%ls\" TargetMode=\"External\"/>"),
				strRid, sLink);
		}
		else
		{
			strRels.Format(_T("<Relationship Id=\"%ls\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"%ls\"/>"), 
				strRid ,sLink);
		}

		m_pWriter->WriteString(strRels);

		return m_lNextRelsID - 1;
	}		

	CBinaryFileReader::CBinaryFileReader()
	{
		m_pMainDocument = NULL;

		m_lNextId = 0;

		m_lChartNumber = 1;
		m_strContentTypes = _T("");

		m_lDocumentType = XMLWRITER_DOC_TYPE_PPTX;

		m_pRels = new CRelsGenerator();
		m_nCurrentRelsStack = -1;
	}
	CBinaryFileReader::~CBinaryFileReader()
	{
		//RELEASEINTERFACE(m_pMainDocument);
		RELEASEOBJECT(m_pRels);

		size_t nCountStackRels = m_stackRels.size();
		for (size_t i = 0; i < nCountStackRels; ++i)
		{
			CRelsGenerator* pCur = m_stackRels[i];
			RELEASEOBJECT(pCur);
		}
		m_stackRels.clear();
	}

	void CBinaryFileReader::SetMainDocument(BinDocxRW::CDocxSerializer* pMainDoc)
	{
		//RELEASEINTERFACE(m_pMainDocument);
		m_pMainDocument = pMainDoc;
		//ADDREFINTERFACE(m_pMainDocument);
	}

	void CBinaryFileReader::Init(BYTE* pData, _INT32 lStart, _INT32 lSize)
	{
		m_pData = pData;
		m_lSize = lSize + lStart;
		m_lPos = lStart;
		m_pDataCur = m_pData + m_lPos;
	}
	_INT32 CBinaryFileReader::GenerateNextId()
	{
		++m_lNextId;
		return m_lNextId;
	}

	int CBinaryFileReader::Seek(_INT32 _pos)
	{
		if (_pos > m_lSize)
			return 1;
		m_lPos = _pos;
		m_pDataCur = m_pData + m_lPos;
		return 0;
	}
	int CBinaryFileReader::Skip(_INT32 _skip)
	{
		if (_skip < 0)
			return 1;
		return Seek(m_lPos + _skip);
	}
	bool CBinaryFileReader::Peek(int nSizeToRead = 0)
	{
		return !(m_lPos + nSizeToRead > m_lSize);
	}

	// 1 bytes
	BYTE CBinaryFileReader::GetUChar()
	{
		if (m_lPos >= m_lSize)
			return 0;

		BYTE res = *m_pDataCur;
		++m_lPos;
		++m_pDataCur;
		return res;
	}
	signed char CBinaryFileReader::GetChar()
	{
		if (m_lPos >= m_lSize)
			return 0;

		BYTE res = *m_pDataCur;
		if (res > 127)
			res -= 256;
		++m_lPos;
		++m_pDataCur;
		return res;
	}

	// 1 bytes
	BYTE CBinaryFileReader::GetUChar_TypeNode()
	{
		if (m_lPos >= m_lSize)
			return NSBinPptxRW::g_nodeAttributeEnd;

		BYTE res = *m_pDataCur;
		++m_lPos;
		++m_pDataCur;
		return res;
	}

	bool CBinaryFileReader::GetBool()
	{
		int res = GetUChar();
		return (res == 1) ? true : false;
	}

	// 2 byte
	_UINT16 CBinaryFileReader::GetUShort()
	{
		if (m_lPos + 1 >= m_lSize)
			return 0;
#ifdef _IOS
        _UINT16 res = 0;
        memcpy(&res, m_pDataCur, sizeof(_UINT16));
#else
        _UINT16 res = *((_UINT16*)m_pDataCur);   // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPos += 2;
		m_pDataCur += 2;
		return res;		
	}

	// 4 byte
	_UINT32 CBinaryFileReader::GetULong()
	{
		if (m_lPos + 3 >= m_lSize)
			return 0;
#ifdef _IOS
        _UINT32 res = 0;
        memcpy(&res, m_pDataCur, sizeof(_UINT32));
#else
        _UINT32 res = *((_UINT32*)m_pDataCur);   // EXC_ARM_DA_ALIGN on ios
#endif
        m_lPos += 4;
		m_pDataCur += 4;
        return res;
	}
	_INT64 CBinaryFileReader::GetLong64()
	{
		if (m_lPos + 7 >= m_lSize)
			return 0;
#ifdef _IOS
        _INT64 res = 0;
        memcpy(&res, m_pDataCur, sizeof(_INT64));
#else
        _INT64 res = *((_INT64*)m_pDataCur);   // EXC_ARM_DA_ALIGN on ios
#endif
        m_lPos += 8;
		m_pDataCur += 8;
        return res;			
	}
	_INT32 CBinaryFileReader::GetLong()
	{
		return (_INT32)GetULong();			
	}
	double CBinaryFileReader::GetDouble()
	{
		return 1.0 * GetLong() / 100000;
	}
	double CBinaryFileReader::GetDouble64()
	{
		return 1.0 * GetLong64() / 100000;
	}	// 8 byte
	double CBinaryFileReader::GetDoubleReal()
	{
        if (m_lPos + DOUBLE_SIZEOF > m_lSize)
            return 0;
#ifdef _IOS
        double res = 0.0;
        memcpy(&res, m_pDataCur, sizeof(double));
#else
        double res = *((double*)m_pDataCur);    // EXC_ARM_DA_ALIGN on ios
#endif        
        m_lPos += DOUBLE_SIZEOF;
        m_pDataCur += DOUBLE_SIZEOF;
        return res;
	}

	//String
	CString CBinaryFileReader::GetString(_INT32 len)
	{
        len *= 2;
		return GetString3(len);		
	}
	CStringA CBinaryFileReader::GetString1(_INT32 len)
	{
        if (len < 1 )
            return "";
        if (m_lPos + len > m_lSize)
			return "";

		CStringA res((CHAR*)m_pDataCur, len);
		m_lPos += len;
		m_pDataCur += len;
		return res;
	}
	CString CBinaryFileReader::GetString2()
    {
        _INT32 len = GetLong();
		return GetString(len);
	}
    CString CBinaryFileReader::GetString3(_INT32 len)//len in byte for utf16
	{
        if (len < 1 )
            return _T("");
        if (m_lPos + len > m_lSize)
            return _T("");

        _UINT32 lSize = len >>1; //string in char

        if (sizeof(wchar_t) == 4)
        {
            wchar_t * sBuffer = new wchar_t[lSize + 1];
            memset(sBuffer, 0, lSize + 1);

            UTF16* pStrUtf16 = (UTF16 *) m_pDataCur;
            UTF32 *pStrUtf32 = (UTF32 *) sBuffer;

            // this values will be modificated
            const UTF16 *pStrUtf16_Conv = pStrUtf16;
            UTF32 *pStrUtf32_Conv = pStrUtf32;

            ConversionResult eUnicodeConversionResult =
            ConvertUTF16toUTF32 (&pStrUtf16_Conv
                                , &pStrUtf16[lSize]
                                , &pStrUtf32_Conv
                                , &pStrUtf32 [lSize]
                                , strictConversion);

            if (conversionOK != eUnicodeConversionResult)
            {
                delete []sBuffer;
                return _T("");
            }

            CString res((WCHAR*)sBuffer, lSize);

            delete []sBuffer;
            m_lPos += len;
            m_pDataCur += len;

            return res;
        }
        else
        {
            CString res((WCHAR*)m_pDataCur, lSize);

            m_lPos += len;
            m_pDataCur += len;

            return res;
        }
	}

    bool CBinaryFileReader::GetArray(BYTE **pBuffer, _INT32 len)
	{
		if (0 == len)
            return false;
		if (m_lPos + len > m_lSize)
            return false;

        *pBuffer = new BYTE [len];

        memcpy(pBuffer, m_pDataCur, len);

		m_lPos += len;
		m_pDataCur += len;

        return true;
	}
    /*LPSAFEARRAY CBinaryFileReader::GetArray(_INT32 len)
    {
        if (0 == len)
            return NULL;
        if (m_lPos + len > m_lSize)
            return NULL;

        SAFEARRAY* pArray = SafeArrayCreateVector(VT_UI1, (_UINT32)len);

        BYTE* pDataD = (BYTE*)pArray->pvData;
        memcpy(pDataD, m_pDataCur, len);

        m_lPos += len;
        m_pDataCur += len;

        return pArray;
    }
    */

	CStringA CBinaryFileReader::GetString2A()
	{
		_INT32 len = GetULong();
		return GetString1(len);
	}

	void CBinaryFileReader::SkipRecord()
	{
		_INT32 _len = GetULong();
		Skip(_len);
	}

	_INT32 CBinaryFileReader::GetPos()
	{
		return m_lPos;
	}

	_INT32 CBinaryFileReader::GetSize()
	{
		return m_lSize;
	}

	BYTE* CBinaryFileReader::GetData()
	{
		return m_pData;
	}
	BYTE* CBinaryFileReader::GetPointer(int nSize = 0)
	{
        if (nSize < 0) return 0;

		if (m_lPos + nSize > m_lSize)
			return 0;

		BYTE* res = (BYTE*)m_pDataCur;
		m_lPos += nSize;
		m_pDataCur += nSize;
		return res;
	}

    void CBinaryFileReader::Deserialize(NSBinPptxRW::CImageManager2* pManager, BYTE* pData, _INT32 nSize/*, LPSAFEARRAY pArray*/)
	{
        //BYTE* pData = (BYTE*)pArray->pvData;
        //this->Init(pData, 0, pArray->rgsabound[0].cElements);
        this->Init(pData, 0, nSize);

		pManager->Deserialize(this);
	}
    void CBinaryFileReader::Deserialize(NSShapeImageGen::CImageManager* pManager, BYTE* pData, _INT32 nSize/*, LPSAFEARRAY pArray*/)
	{
        //BYTE* pData = (BYTE*)pArray->pvData;
        //this->Init(pData, 0, pArray->rgsabound[0].cElements);
        this->Init(pData, 0, nSize);

		pManager->Deserialize(this);
	}
}
