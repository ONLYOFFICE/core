#include "BinaryFileReaderWriter.h"

#include "../../Common/DocxFormat/Source/Base/Nullable.h"
#include "BinReaderWriterDefines.h"

#include "../../Common/Base64.h"
#include "ImageManager.h"
#include "./XmlWriter.h"
#include "BinaryFileReaderWriter.h"
#include "../PPTXFormat/FileContainer.h"
#include "../PPTXFormat/DocxFormat/WritingElement.h"
#include "../../ASCOfficeDocxFile2/DocWrapper/DocxSerializer.h"

#define BYTE_SIZEOF		sizeof(BYTE)
#define USHORT_SIZEOF	sizeof(USHORT)
#define ULONG_SIZEOF	sizeof(ULONG)
#define INT_SIZEOF		sizeof(int)
#define DOUBLE_SIZEOF	sizeof(double)

#define CHAR_SIZEOF		sizeof(CHAR)
#define SHORT_SIZEOF	sizeof(SHORT)
#define LONG_SIZEOF		sizeof(LONG)

#define DOUBLE_MAIN		10000

namespace NSBinPptxRW
{
	inline LONG __strlen(const char* str)
	{
		const char* s = str;
		for (; *s != 0; ++s);
		return (LONG)(s - str);
	}
	inline LONG __wstrlen(const wchar_t* str)
	{
		const wchar_t* s = str;
		for (; *s != 0; ++s);
		return (LONG)(s - str);
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
		m_pImageManager = new NSShapeImageGen::CImageManager();
	}
	CCommonWriter::~CCommonWriter()
	{
		m_pNativePicker = NULL;
		RELEASEINTERFACE(m_pFontPicker);
		RELEASEOBJECT(m_pImageManager);
	}
	void CCommonWriter::CreateFontPicker(IOfficeFontPicker* pPicker)
	{
		RELEASEINTERFACE(m_pFontPicker);
		m_pNativePicker = NULL;
		if (pPicker != NULL)
		{
			m_pFontPicker = pPicker;
			ADDREFINTERFACE(m_pFontPicker);
		}

		if (NULL == m_pFontPicker)
		{
			CoCreateInstance(__uuidof(COfficeFontPicker), NULL, CLSCTX_ALL, __uuidof(IOfficeFontPicker), (void**)&m_pFontPicker);
		}
		VARIANT var;
		m_pFontPicker->GetAdditionalParam(L"NativePicker", &var);

		m_pNativePicker = (NSFontCutter::CFontDstManager*)var.pvRecord;
	}
	void CCommonWriter::CheckFontPicker()
	{
		if (NULL == m_pFontPicker)
			CreateFontPicker(NULL);
	}

	CImageManager2::CImageManager2() : m_mapImages(), m_lIndexNextImage(0)
	{
		m_bIsWord = FALSE;
	}
	CImageManager2::~CImageManager2()
	{
	}
	void CImageManager2::Clear()
	{
		m_mapImages.RemoveAll();
		m_lIndexNextImage = 0;
	}
	void CImageManager2::SetDstMedia(const CString& strDst)
	{
		m_strDstMedia = strDst;
	}
	CString CImageManager2::GetDstMedia()
	{
		return m_strDstMedia;
	}

	CString CImageManager2::GenerateImage(const CString& strInput, CString strBase64Image)
	{
		CAtlMap<CString, CString>::CPair* pPair = NULL;

		if (_T("") == strBase64Image)
			pPair = m_mapImages.Lookup(strInput);
		else 
			pPair = m_mapImages.Lookup(strBase64Image);

		if (NULL != pPair)
			return pPair->m_value;

		if (IsNeedDownload(strInput))
			return DownloadImage(strInput);

		CString strExts = _T(".jpg");
		int nIndexExt = strInput.ReverseFind(TCHAR('.'));
		if (-1 != nIndexExt)
			strExts = strInput.Mid(nIndexExt);

		if (strExts == _T(".tmp"))
			strExts = _T(".png");

		CString strMetafileImage = _T("");
		if (strExts == _T(".svg"))
		{
			OOX::CPath oPath = strInput;
			CString strFolder = oPath.GetDirectory();
			CString strFileName = oPath.GetFilename();

			strFileName.Delete(strFileName.GetLength() - 4, 4);

			CString str1 = strFolder + strFileName + _T(".emf");
			CString str2 = strFolder + strFileName + _T(".wmf");

			if (OOX::CSystemUtility::IsFileExist(str1))
			{
				strMetafileImage = str1;
				strExts = _T(".emf");
			}
			else if (OOX::CSystemUtility::IsFileExist(str2))
			{
				strMetafileImage = str2;
				strExts = _T(".wmf");
			}
		}

		CString strImage = _T("");
		if ((_T(".jpg") == strExts) || (_T(".jpeg") == strExts) || (_T(".png") == strExts) || (_T(".emf") == strExts) || (_T(".wmf") == strExts))
		{
			strImage.Format(_T("image%d"), m_lIndexNextImage++);

			CString strOutput = m_strDstMedia + _T("\\") + strImage + strExts;	

			if (!m_bIsWord)
				strImage  = _T("../media/") + strImage + strExts;
			else
				strImage  = _T("media/") + strImage + strExts;

			if (_T("") == strBase64Image)
				m_mapImages.SetAt(strInput, strImage);
			else
				m_mapImages.SetAt(strBase64Image, strImage);

			// теперь нужно скопировать картинку
			if (_T("") != strMetafileImage)
				CDirectory::CopyFile(strMetafileImage, strOutput, NULL, NULL);
			else if (strOutput != strInput)
				CDirectory::CopyFile(strInput, strOutput, NULL, NULL);
		}
		else
		{
			// content types!!!
			strExts = _T(".png");
			strImage.Format(_T("image%d"), m_lIndexNextImage++);

			CString strOutput = m_strDstMedia + _T("\\") + strImage + strExts;		

			if (!m_bIsWord)
				strImage  = _T("../media/") + strImage + strExts;
			else
				strImage  = _T("media/") + strImage + strExts;

			if (_T("") == strBase64Image)
				m_mapImages.SetAt(strInput, strImage);
			else
				m_mapImages.SetAt(strBase64Image, strImage);

			SaveImageAsPng(strInput, strOutput);
		}
		return strImage;
	}

	void CImageManager2::SaveImageAsPng(const CString& strFileSrc, const CString& strFileDst)
	{
		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(std::wstring(strFileSrc.GetString())))
			return;

		CxImage img;

		if( img.Decode( oFile.GetFileNative(), 0 ) )
		{
			img.Encode( oFile.GetFileNative(), _CXIMAGE_FORMAT_PNG );
		}
		oFile.CloseFile();
	}

	void CImageManager2::SaveImageAsJPG(const CString& strFileSrc, const CString& strFileDst)
	{
		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(std::wstring(strFileSrc.GetString())))
			return;

		CxImage img;

		if( img.Decode( oFile.GetFileNative(), 0 ) )
		{
			img.Encode( oFile.GetFileNative(), _CXIMAGE_FORMAT_JPG );
		}
		oFile.CloseFile();
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
	CString CImageManager2::DownloadImage(const CString& strFile)
	{
		CFileDownloader oDownloader(strFile, TRUE);
		oDownloader.Start( 1 );
		while ( oDownloader.IsRunned() )
		{
			::Sleep( 10 );
		}

		if ( oDownloader.IsFileDownloaded() )
		{
			return GenerateImage( oDownloader.GetFilePath(), strFile );
		}
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
	ULONG CBinaryFileWriter::GetPosition()
	{
		return m_lPosition;
	}
	void CBinaryFileWriter::SetPosition(const ULONG& lPosition)
	{
		m_lPosition = lPosition;
		m_pStreamCur = m_pStreamData + m_lPosition;
	}
	void CBinaryFileWriter::Skip(const ULONG& lSize)
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
		memset(m_arStack, 0, MAX_STACK_SIZE * sizeof(ULONG));

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
		memset(m_arStack, 0, MAX_STACK_SIZE * sizeof(ULONG));
	}

	void CBinaryFileWriter::CheckBufferSize(size_t lPlus)
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
	void CBinaryFileWriter::WriteBOOL(const bool& bValue)
	{			
		WriteBYTE((bValue == true) ? 1 : 0);
	}
	void CBinaryFileWriter::WriteUSHORT(const USHORT& lValue)
	{
		CheckBufferSize(USHORT_SIZEOF);

		*((USHORT*)m_pStreamCur) = lValue; 
		m_lPosition += USHORT_SIZEOF;
		m_pStreamCur += USHORT_SIZEOF;
	}
	void CBinaryFileWriter::WriteULONG(const ULONG& lValue)
	{
		CheckBufferSize(ULONG_SIZEOF);

		*((ULONG*)m_pStreamCur) = lValue; 
		m_lPosition += ULONG_SIZEOF;
		m_pStreamCur += ULONG_SIZEOF;
	}
	void CBinaryFileWriter::WriteLONG(const long& lValue)
	{
		CheckBufferSize(LONG_SIZEOF);

		*((long*)m_pStreamCur) = lValue; 
		m_lPosition += LONG_SIZEOF;
		m_pStreamCur += LONG_SIZEOF;
	}
	void CBinaryFileWriter::WriteINT(const int& lValue)
	{
		CheckBufferSize(INT_SIZEOF);

		*((int*)m_pStreamCur) = lValue; 
		m_lPosition += INT_SIZEOF;
		m_pStreamCur += INT_SIZEOF;
	}
	void CBinaryFileWriter::WriteDouble(const double& dValue)
	{
		long _val = (long)(dValue * 100000);
		WriteLONG(_val);
	}
	void CBinaryFileWriter::WriteDoubleReal(const double& dValue)
	{
		CheckBufferSize(DOUBLE_SIZEOF);

		*((double*)m_pStreamCur) = dValue; 
		m_lPosition += DOUBLE_SIZEOF;
		m_pStreamCur += DOUBLE_SIZEOF;
	}
	void CBinaryFileWriter::WriteStringW(const WCHAR* sBuffer)
	{
		LONG lSize = __wstrlen(sBuffer);
		LONG lSizeMem = lSize * sizeof(wchar_t);

		CheckBufferSize(ULONG_SIZEOF + lSizeMem);

		*((ULONG*)m_pStreamCur) = lSizeMem; 
		m_lPosition += ULONG_SIZEOF;
		m_pStreamCur += ULONG_SIZEOF;

		memcpy(m_pStreamCur, sBuffer, lSizeMem);
		m_lPosition += lSizeMem;
		m_pStreamCur += lSizeMem;
	}
	void CBinaryFileWriter::WriteBYTEArray(const BYTE* pBuffer, size_t len)
	{
		CheckBufferSize(len);
		memcpy(m_pStreamCur, pBuffer, len);
		m_lPosition += (ULONG)len;
		m_pStreamCur += len;
	}
	void CBinaryFileWriter::WriteStringA(const char* sBuffer)
	{
		LONG lSize = __strlen(sBuffer);
		LONG lSizeMem = lSize * sizeof(char);

		CheckBufferSize(ULONG_SIZEOF + lSizeMem);

		*((ULONG*)m_pStreamCur) = lSizeMem; 
		m_lPosition += ULONG_SIZEOF;
		m_pStreamCur += ULONG_SIZEOF;

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
		LONG lSize = __wstrlen(sBuffer);
		LONG lSizeMem = lSize * sizeof(wchar_t);

		CheckBufferSize(ULONG_SIZEOF + lSizeMem);

		*((ULONG*)m_pStreamCur) = lSize; 
		m_lPosition += ULONG_SIZEOF;
		m_pStreamCur += ULONG_SIZEOF;

		memcpy(m_pStreamCur, sBuffer, lSizeMem);
		m_lPosition += lSizeMem;
		m_pStreamCur += lSizeMem;
	}
	void CBinaryFileWriter::WriteStringW2(CString& sBuffer)
	{
		WriteStringW2(sBuffer.GetBuffer());
	}
	void CBinaryFileWriter::WriteStringW3(const WCHAR* sBuffer)
	{
		LONG lSize = __wstrlen(sBuffer);
		LONG lSizeMem = lSize * sizeof(wchar_t);

		CheckBufferSize(lSizeMem);

		memcpy(m_pStreamCur, sBuffer, lSizeMem);
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
		m_pCommonRels = new NSCommon::smart_ptr<PPTX::FileContainer>();
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

	void CBinaryFileWriter::StartRecord(LONG lType)
	{
		m_arStack[m_lStackPosition] = m_lPosition + 5; // sizeof(BYTE) + sizeof(ULONG)
		m_lStackPosition++;
		WriteBYTE((BYTE)lType);
		WriteULONG(0);
	}
	void CBinaryFileWriter::EndRecord()
	{
		m_lStackPosition--;
		(*(LONG*)(m_pStreamData + m_arStack[m_lStackPosition] - 4)) = m_lPosition - m_arStack[m_lStackPosition];
	}

	void CBinaryFileWriter::StartMainRecord(LONG lType)
	{
		CSeekTableEntry oEntry;
		oEntry.Type = lType;
		oEntry.SeekPos = m_lPosition;
		m_arMainTables.Add(oEntry);
		//StartRecord(lType);
	}

	void CBinaryFileWriter::WriteReserved(size_t lCount)
	{
		CheckBufferSize(lCount);
		memset(m_pStreamCur, 0, lCount);
		m_pStreamCur += lCount;
		m_lPosition += (ULONG)lCount;
	}

	void CBinaryFileWriter::WriteMainPart()
	{
		BYTE* pData = m_pStreamData;
		size_t nCount = m_arMainTables.GetCount();

		for (size_t i = 0; i < nCount; i++)
		{
			*pData = (BYTE)m_arMainTables[i].Type;
			++pData;
			*((LONG*)pData) = m_arMainTables[i].SeekPos;
			pData += 4;
		}
	}

	void CBinaryFileWriter::WriteString1(int type, const CString& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);

		ULONG len = (ULONG)val.GetLength();
		WriteULONG(len);

		len <<= 1;

		CString* s = const_cast<CString*>(&val);
		CheckBufferSize(len);

		memcpy(m_pStreamCur, s->GetBuffer(), len);
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
		ULONG len = (ULONG)val.GetLength();
		WriteULONG(len);

		len <<= 1;

		CString* s = const_cast<CString*>(&val);
		CheckBufferSize(len);

		memcpy(m_pStreamCur, s->GetBuffer(), len);
		m_pStreamCur += len;
		m_lPosition += len;
	}

	void CBinaryFileWriter::WriteString1Data(int type, const WCHAR* pData, ULONG len)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);

		WriteULONG(len);

		len <<= 1;

		CheckBufferSize(len);

		memcpy(m_pStreamCur, (BYTE*)pData, len);
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
		ULONG ival = (ULONG)val;
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
		LPSTR pbBase64Buffer = new CHAR[nBase64BufferLen];
		if (TRUE == Base64::Base64Encode(pBuffer, (int)dwLen, pbBase64Buffer, &nBase64BufferLen, Base64::B64_BASE64_FLAG_NOCRLF))
		{
			strDst64.SetString(pbBase64Buffer, nBase64BufferLen);
		}

		RELEASEARRAYOBJECTS(pBuffer);

		oFile.CloseFile();

		//DeleteFile(sFile);
	}

	void CBinaryFileWriter::WriteTheme64(LONG lIndex, const CString& sFile)
	{
		GetBase64File(sFile, m_pCommon->m_oRels[lIndex].m_strImageBase64);
	}

	void CBinaryFileWriter::WriteLayoutTheme64(LONG lIndexTheme, LONG lIndexLayout, const CString& sFile)
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
	LPSAFEARRAY CBinaryFileWriter::Serialize(NSBinPptxRW::CImageManager2* pManager)
	{
		pManager->Serialize(this);
		return GetSafearray();
	}
	LPSAFEARRAY CBinaryFileWriter::Serialize(NSShapeImageGen::CImageManager* pManager)
	{
		pManager->Serialize(this);
		return GetSafearray();
	}

	LPSAFEARRAY CBinaryFileWriter::GetSafearray()
	{			
		ULONG lBinarySize = this->GetPosition();
		if (0 == lBinarySize)
			return NULL;

		SAFEARRAY* pArray = SafeArrayCreateVector(VT_UI1, lBinarySize);

		BYTE* pDataD = (BYTE*)pArray->pvData;
		BYTE* pDataS = this->GetBuffer();
		memcpy(pDataD, pDataS, lBinarySize);

		return pArray;
	}
	CSlideMasterInfo::CSlideMasterInfo() : m_arLayouts()
	{
		m_lThemeIndex = -1;				
	}
	CSlideMasterInfo::CSlideMasterInfo(const CSlideMasterInfo& oSrc)
	{
		m_lThemeIndex	= oSrc.m_lThemeIndex;
		m_arLayouts.Copy(oSrc.m_arLayouts);
	}

	CRelsGenerator::CRelsGenerator(CImageManager2* pManager = NULL) : m_lNextRelsID(1), m_mapImages()
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
		m_mapImages.RemoveAll();
		m_mapLinks.RemoveAll();
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

		int nCountLayouts = (int)oInfo.m_arLayouts.GetCount();
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

	int CRelsGenerator::WriteImage(const CString& strImagePath, CString strBase64Image = _T(""))
	{
		CString strImage = m_pManager->GenerateImage(strImagePath, strBase64Image);
		CAtlMap<CString, int>::CPair* pPair = m_mapImages.Lookup(strImage);

		if (NULL != pPair)
		{
			return pPair->m_value;				
		}

		m_mapImages.SetAt(strImage, m_lNextRelsID);
		CString strRid = _T("");
		strRid.Format(_T("rId%d"), m_lNextRelsID++);

		CString strRels = _T("");
		strRels.Format(_T("<Relationship Id=\"%s\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"%s\"/>"),
			strRid, strImage);
		m_pWriter->WriteString(strRels);

		return m_lNextRelsID - 1;
	}
	int CRelsGenerator::WriteChart(int nChartNumber, LONG lDocType = XMLWRITER_DOC_TYPE_PPTX)
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
		strRels.Format(_T("<Relationship Id=\"%s\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart\" Target=\"%s\"/>"),
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
		CAtlMap<CString, int>::CPair* pPair = m_mapLinks.Lookup(strLink);

		if (NULL != pPair)
		{
			return pPair->m_value;				
		}

		m_mapLinks.SetAt(strLink, m_lNextRelsID);
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
			strRels.Format(_T("<Relationship Id=\"%s\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"%s\" TargetMode=\"External\"/>"),
				strRid, sLink);
		}
		else
		{
			strRels.Format(_T("<Relationship Id=\"%s\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"%s\"/>"), 
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

		size_t nCountStackRels = m_stackRels.GetCount();
		for (size_t i = 0; i < nCountStackRels; ++i)
		{
			CRelsGenerator* pCur = m_stackRels[i];
			RELEASEOBJECT(pCur);
		}
		m_stackRels.RemoveAll();
	}

	void CBinaryFileReader::SetMainDocument(BinDocxRW::CDocxSerializer* pMainDoc)
	{
		//RELEASEINTERFACE(m_pMainDocument);
		m_pMainDocument = pMainDoc;
		//ADDREFINTERFACE(m_pMainDocument);
	}

	void CBinaryFileReader::Init(BYTE* pData, LONG lStart, LONG lSize)
	{
		m_pData = pData;
		m_lSize = lSize + lStart;
		m_lPos = lStart;
		m_pDataCur = m_pData + m_lPos;
	}
	LONG CBinaryFileReader::GenerateNextId()
	{
		++m_lNextId;
		return m_lNextId;
	}

	int CBinaryFileReader::Seek(LONG _pos)
	{
		if (_pos > m_lSize)
			return 1;
		m_lPos = _pos;
		m_pDataCur = m_pData + m_lPos;
		return 0;
	}
	int CBinaryFileReader::Skip(LONG _skip)
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
	bool CBinaryFileReader::GetBool()
	{
		int res = GetUChar();
		return (res == 1) ? true : false;
	}

	// 2 byte
	USHORT CBinaryFileReader::GetUShort()
	{
		if (m_lPos + 1 >= m_lSize)
			return 0;

		USHORT res = *((USHORT*)m_pDataCur);
		m_lPos += 2;
		m_pDataCur += 2;
		return res;		
	}

	// 4 byte
	ULONG CBinaryFileReader::GetULong()
	{
		if (m_lPos + 3 >= m_lSize)
			return 0;

		ULONG res = *((ULONG*)m_pDataCur);
		m_lPos += 4;
		m_pDataCur += 4;
		return res;
	}

	LONG CBinaryFileReader::GetLong()
	{
		return (LONG)GetULong();			
	}
	double CBinaryFileReader::GetDouble()
	{
		return 1.0 * GetLong() / 100000;
	}
	// 8 byte
	double CBinaryFileReader::GetDoubleReal()
	{
		if (m_lPos + DOUBLE_SIZEOF > m_lSize)
			return 0;

		double res = *((double*)m_pDataCur);
		m_lPos += DOUBLE_SIZEOF;
		m_pDataCur += DOUBLE_SIZEOF;
		return res;
	}

	//String
	CString CBinaryFileReader::GetString(LONG len)
	{
		len *= 2;
		return GetString3(len);		
	}
	CStringA CBinaryFileReader::GetString1(LONG len)
	{
		if (m_lPos + len > m_lSize)
			return "";

		CStringA res((CHAR*)m_pDataCur, len);
		m_lPos += len;
		m_pDataCur += len;
		return res;
	}
	CString CBinaryFileReader::GetString2()
	{
		LONG len = GetULong();
		return GetString(len);
	}
	CString CBinaryFileReader::GetString3(LONG len)
	{
		if (m_lPos + len > m_lSize)
			return _T("");

		CString res((WCHAR*)m_pDataCur, len >> 1);
		m_lPos += len;
		m_pDataCur += len;
		return res;			
	}

	LPSAFEARRAY CBinaryFileReader::GetArray(LONG len)
	{
		if (0 == len)
			return NULL;
		if (m_lPos + len > m_lSize)
			return NULL;

		SAFEARRAY* pArray = SafeArrayCreateVector(VT_UI1, (ULONG)len);

		BYTE* pDataD = (BYTE*)pArray->pvData;
		memcpy(pDataD, m_pDataCur, len);

		m_lPos += len;
		m_pDataCur += len;

		return pArray;
	}

	CStringA CBinaryFileReader::GetString2A()
	{
		LONG len = GetULong();
		return GetString1(len);
	}

	void CBinaryFileReader::SkipRecord()
	{
		LONG _len = GetULong();
		Skip(_len);
	}

	LONG CBinaryFileReader::GetPos()
	{
		return m_lPos;
	}

	LONG CBinaryFileReader::GetSize()
	{
		return m_lSize;
	}

	BYTE* CBinaryFileReader::GetData()
	{
		return m_pData;
	}
	BYTE* CBinaryFileReader::GetPointer(int nSize = 0)
	{
		if (m_lPos + nSize > m_lSize)
			return 0;

		BYTE* res = (BYTE*)m_pDataCur;
		m_lPos += nSize;
		m_pDataCur += nSize;
		return res;
	}

	void CBinaryFileReader::Deserialize(NSBinPptxRW::CImageManager2* pManager, LPSAFEARRAY pArray)
	{
		BYTE* pData = (BYTE*)pArray->pvData;
		this->Init(pData, 0, pArray->rgsabound[0].cElements);

		pManager->Deserialize(this);
	}
	void CBinaryFileReader::Deserialize(NSShapeImageGen::CImageManager* pManager, LPSAFEARRAY pArray)
	{
		BYTE* pData = (BYTE*)pArray->pvData;
		this->Init(pData, 0, pArray->rgsabound[0].cElements);

		pManager->Deserialize(this);
	}
}