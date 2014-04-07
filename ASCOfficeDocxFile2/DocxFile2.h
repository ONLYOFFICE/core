#pragma once

#include "resource.h"       // main symbols
#include "..\Common\ASCUtils.h"
#include "..\Common\MappingFile.h"
#include "BinWriter/BinWriters.h"
#include "BinReader/Readers.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

int g_nCurFormatVersion = 0;

#define BUFFER_GROW_SIZE 1 * 1024 * 1024

// IAVSOfficeDocxFile2
[ object, uuid("98B1ECA8-9575-4eec-B327-8F8BA3FA232C"), dual, pointer_default(unique) ]
__interface IAVSOfficeDocxFile2: IDispatch
{
	[id(1)] HRESULT OpenFile([in] BSTR bsInputDir, [in] BSTR bsFileDst);
	[id(2)] HRESULT GetJfdoc([in] BSTR bsInputDir, [out] BSTR* bsJfdoc);

	[id(3)] HRESULT SetMediaDir([in] BSTR bsMediaDir);
	[id(4)] HRESULT SetFontDir([in] BSTR bsFontDir);

	[id(5)] HRESULT SetUseSystemFonts([in] BOOL useSystemFonts);
	[id(6)] HRESULT GetBinaryContent([in] BSTR bsTxContent, [out, satype("BYTE")] SAFEARRAY** ppBinary);

	[id(10)] HRESULT Write([in] BSTR bstrFileIn, [in] BSTR bstrFileOut);
	[id(20)] HRESULT GetXmlContent([in, satype("BYTE")] SAFEARRAY* pBinaryObj, [in] LONG lStart, [in] LONG lLength, [out] BSTR* bsXml);

	[id(2000 + 0)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001 + 1)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// _IAVSOfficeDocxFile2Events
[uuid("C3CF75C3-28FE-4b2c-A770-5952ADF4EAC2"), dispinterface]
__interface _IAVSOfficeDocxFile2Events
{
};

// CAVSOfficeDocxFile2
[ coclass, default(IAVSOfficeDocxFile2), threading(apartment), event_source(com), vi_progid("DocxFile2"), progid("DocxFile2.Rend.1"), version(1.0), uuid("CD07583A-6362-454f-A14E-542AE706FFBC") ]
class ATL_NO_VTABLE CAVSOfficeDocxFile2 : public IAVSOfficeDocxFile2
{
public:
	__event __interface _IAVSOfficeDocxFile2Events;

private:
	CString mediaDir;
	CString fontDir;
	CString m_strEmbeddedFontsDirectory;
	BinDocxRW::BinaryFileWriter* m_oBinaryFileWriter;
	CString m_sFontDir;
	bool m_bSaveChartAsImg;
	Writers::FileWriter* m_pCurFileWriter;

	bool m_bIsNoBase64Save;
public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	
	CAVSOfficeDocxFile2()
	{
		m_oBinaryFileWriter = NULL;
		m_bSaveChartAsImg = false;
		m_bIsNoBase64Save = false;
	}
	~CAVSOfficeDocxFile2()
	{
	}

public:
	STDMETHOD(OpenFile)(BSTR bsInputDir, BSTR bsFileDst)
	{
		if (mediaDir == _T("")) {
			OOX::CPath path(bsFileDst);
			mediaDir = path.GetDirectory() + _T("/media/");
			CreateDirectoryW(mediaDir, NULL);
		}
		Streams::CBuffer oBuffer;
		oBuffer.Create(BUFFER_GROW_SIZE, BUFFER_GROW_SIZE);
		Streams::CBufferedStream oBufferedStream;
		oBufferedStream.SetBuffer(&oBuffer);

		DocWrapper::FontProcessor fp;
		fp.setFontDir(fontDir);
		PPTXFile::IOfficeFontPicker* pFontPicker = NULL;
		CoCreateInstance(__uuidof(PPTXFile::COfficeFontPicker), NULL, CLSCTX_ALL, __uuidof(PPTXFile::IOfficeFontPicker), (void**)(&pFontPicker));
		BSTR bstrFontDir1 = fontDir.AllocSysString();
		pFontPicker->Init(bstrFontDir1);
		SysFreeString(bstrFontDir1);
		NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager = NULL;
		if(false == m_strEmbeddedFontsDirectory.IsEmpty())
		{
			CreateDirectoryW(m_strEmbeddedFontsDirectory, NULL);

			BSTR bstrEmbeddedFontsDirectory = m_strEmbeddedFontsDirectory.AllocSysString();
			pFontPicker->SetEmbeddedFontsDirectory(bstrEmbeddedFontsDirectory);
			SysFreeString(bstrEmbeddedFontsDirectory);

			VARIANT vt;
			pFontPicker->GetAdditionalParam(_T("NativeCutter"), &vt);
			pEmbeddedFontsManager = (NSFontCutter::CEmbeddedFontsManager*)vt.pvRecord;

			//добавл€ем весь латинский алфавит дл€ списков.
			pEmbeddedFontsManager->CheckString(CString(_T("abcdefghijklmnopqrstuvwxyz")));

			//добавим мега шрифт
			pEmbeddedFontsManager->CheckFont(_T("Wingdings 3"), fp.getFontManager());
			pEmbeddedFontsManager->CheckFont(_T("Arial"), fp.getFontManager());
			//pEmbeddedFontsManager добавл€ютс€ все цифры
		}
		PPTXFile::IAVSOfficeDrawingConverter* pOfficeDrawingConverter;
		CoCreateInstance(__uuidof(PPTXFile::CAVSOfficeDrawingConverter), NULL, CLSCTX_ALL, __uuidof(PPTXFile::IAVSOfficeDrawingConverter), (void**)(&pOfficeDrawingConverter));

		BSTR bstrFontDir = fontDir.AllocSysString();
		pOfficeDrawingConverter->SetFontDir(bstrFontDir);
		SysFreeString(bstrFontDir);
		VARIANT vt;
		vt.vt = VT_UNKNOWN;
		vt.punkVal = pFontPicker;
		pOfficeDrawingConverter->SetAdditionalParam(_T("FontPicker"), vt);
		IUnknown* pThis = NULL;
		this->QueryInterface( __uuidof(IUnknown), (void**)&pThis );
		pOfficeDrawingConverter->SetMainDocument(pThis);
		RELEASEINTERFACE(pThis);
		BSTR bstrMediaDir = mediaDir.AllocSysString();
		pOfficeDrawingConverter->SetMediaDstPath(bstrMediaDir);
		SysFreeString(bstrMediaDir);

		m_oBinaryFileWriter = new BinDocxRW::BinaryFileWriter(oBufferedStream, fp, pEmbeddedFontsManager, pOfficeDrawingConverter);
		m_oBinaryFileWriter->intoBindoc(CString(bsInputDir));

		BYTE* pbBinBuffer = oBufferedStream.GetBuffer();
		int nBinBufferLen = oBufferedStream.GetPosition();

		if (m_bIsNoBase64Save)
		{
			CFile oFile;
			oFile.CreateFileW(bsFileDst);
			oFile.WriteFile(pbBinBuffer, nBinBufferLen);
			oFile.CloseFile();
		}
		else
		{
			int nBase64BufferLen = Base64::Base64EncodeGetRequiredLength(nBinBufferLen, Base64::B64_BASE64_FLAG_NOCRLF);
			BYTE* pbBase64Buffer = new BYTE[nBase64BufferLen];
			if(TRUE == Base64::Base64Encode(pbBinBuffer, nBinBufferLen, (LPSTR)pbBase64Buffer, &nBase64BufferLen, Base64::B64_BASE64_FLAG_NOCRLF))
			{
				CFile oFile;
				oFile.CreateFileW(bsFileDst);
				oFile.WriteStringUTF8(m_oBinaryFileWriter->WriteFileHeader(nBinBufferLen));
				oFile.WriteFile(pbBase64Buffer, nBase64BufferLen);
				oFile.CloseFile();
			}
		}
		RELEASEOBJECT(m_oBinaryFileWriter);
		RELEASEINTERFACE(pFontPicker);
		RELEASEINTERFACE(pOfficeDrawingConverter);
		return S_OK;
	}
	STDMETHOD(GetJfdoc)(BSTR bsInputDir, BSTR* bsJfdoc)
	{
		return S_OK;
	}
	STDMETHOD(SetMediaDir)(BSTR bsMediaDir)
	{
		mediaDir = bsMediaDir;
		return S_OK;
	}
	STDMETHOD(SetFontDir)(BSTR bsFontDir)
	{
		fontDir = bsFontDir;
		return S_OK;
	}

	STDMETHOD(SetUseSystemFonts)(BOOL useSystemFonts)
	{
		DocWrapper::FontProcessor::useSystemFonts = (useSystemFonts == TRUE);
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if (_T("EmbeddedFontsDirectory") == sParamName && ParamValue.vt == VT_BSTR)
		{		
			m_strEmbeddedFontsDirectory = ParamValue.bstrVal;
			return S_OK;
		}
		else if (_T("FontDir") == sParamName && ParamValue.vt == VT_BSTR)
		{
			m_sFontDir =  CString(ParamValue.bstrVal);
		}
		else if (_T("SaveChartAsImg") == sParamName && ParamValue.vt == VT_BOOL)
		{
			m_bSaveChartAsImg = VARIANT_TRUE == ParamValue.boolVal;
		}
		else if (_T("NoBase64Save") == sParamName && ParamValue.vt == VT_BOOL)
		{
			m_bIsNoBase64Save = (VARIANT_TRUE == ParamValue.boolVal);
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetBinaryContent)(BSTR bsTxContent, SAFEARRAY** ppBinary)
	{
		if(NULL == m_oBinaryFileWriter)
			return S_FALSE;
		Streams::CBuffer oBuffer;
		oBuffer.Create(BUFFER_GROW_SIZE, BUFFER_GROW_SIZE);
		Streams::CBufferedStream oBufferedStream;
		oBufferedStream.SetBuffer(&oBuffer);

		XmlUtils::CXmlLiteReader oReader;
		oReader.FromString(CString(bsTxContent));
		oReader.ReadNextNode();//v:textbox
		CString sRootName = oReader.GetName();
		if(_T("v:textbox") == sRootName)
			oReader.ReadNextNode();//w:txbxContent

		OOX::Logic::CSdtContent oSdtContent;
		oSdtContent.fromXML(oReader);

		BinDocxRW::BinaryCommonWriter oBinaryCommonWriter(oBufferedStream, m_oBinaryFileWriter->m_oEmbeddedFontsManager);
		int nCurPos = oBinaryCommonWriter.WriteItemWithLengthStart();
		BinDocxRW::BinaryDocumentTableWriter oBinaryDocumentTableWriter(oBufferedStream, m_oBinaryFileWriter->m_oEmbeddedFontsManager, m_oBinaryFileWriter->m_pTheme, m_oBinaryFileWriter->m_pSettings, m_oBinaryFileWriter->m_oFontProcessor, m_oBinaryFileWriter->m_pCurRels, m_oBinaryFileWriter->m_pOfficeDrawingConverter, NULL, NULL);
		oBinaryDocumentTableWriter.WriteDocumentContent(oSdtContent.m_arrItems);
		oBinaryCommonWriter.WriteItemWithLengthEnd(nCurPos);

		if (NULL != ppBinary)
		{
			long lBinarySize = oBufferedStream.GetPosition();
			SAFEARRAYBOUND	rgsabound[1];
			rgsabound[0].lLbound = 0;
			rgsabound[0].cElements = lBinarySize;
			LPSAFEARRAY pArray = SafeArrayCreate(VT_UI1, 1, rgsabound);

			BYTE* pDataD = (BYTE*)pArray->pvData;
			BYTE* pDataS = oBufferedStream.GetBuffer();
			memcpy(pDataD, pDataS, lBinarySize);

			*ppBinary = pArray;
		}
		return S_OK;
	}
	STDMETHOD(Write)(BSTR bstrFileIn, BSTR bstrDirectoryOut)
	{
		bool bResultOk = false;
		MemoryMapping::CMappingFile oMappingFile = MemoryMapping::CMappingFile();
		if(FALSE != oMappingFile.Open(CString(bstrFileIn)))
		{
			long nBase64DataSize = oMappingFile.GetSize();
			BYTE* pBase64Data = oMappingFile.GetData();

			//провер€ем формат
			bool bValidFormat = false;
			CString sSignature(BinDocxRW::g_sFormatSignature);
			int nSigLength = sSignature.GetLength();
			if(nBase64DataSize > nSigLength)
			{
				CStringA sCurSig((char*)pBase64Data, nSigLength);
				if((CStringA)sSignature == sCurSig)
				{
					bValidFormat = true;
				}
			}
			if(bValidFormat)
			{
				//„итаем из файла версию и длину base64
				int nIndex = nSigLength;
				int nType = 0;
				CStringA version = "";
				CStringA dst_len = "";
				while (true)
				{
					nIndex++;
					BYTE _c = pBase64Data[nIndex];
					if (_c == ';')
					{

						if(0 == nType)
						{
							nType = 1;
							continue;
						}
						else
						{
							nIndex++;
							break;
						}
					}
					if(0 == nType)
						version.AppendChar(_c);
					else
						dst_len.AppendChar(_c);
				}
				int nDataSize = atoi(dst_len);

				SAFEARRAYBOUND	rgsabound[1];
				rgsabound[0].lLbound = 0;
				rgsabound[0].cElements = nDataSize;
				LPSAFEARRAY pArray = SafeArrayCreate(VT_UI1, 1, rgsabound);
				if(FALSE != Base64::Base64Decode((LPCSTR)(pBase64Data + nIndex), nBase64DataSize - nIndex, (BYTE*)pArray->pvData, &nDataSize))
				{
					Streams::CBuffer oBuffer;
					Streams::CBufferedStream oBufferedStream;
					oBufferedStream.SetBuffer(&oBuffer);
					oBufferedStream.Create((BYTE*)pArray->pvData, nDataSize);

					CString sDirectoryOut = CString(bstrDirectoryOut);
					CString sThemePath;
					CString sMediaPath;
					CreateDocument(sDirectoryOut, sThemePath, sMediaPath);

					int nVersion = BinDocxRW::g_nFormatVersion;
					if(version.GetLength() > 0)
					{
						version = version.Right(version.GetLength() - 1);
						int nTempVersion = atoi(version);
						if(0 != nTempVersion)
						{
							g_nCurFormatVersion = nVersion = nTempVersion;
						}
					}
					PPTXFile::IAVSOfficeDrawingConverter* pDrawingConverter = NULL;
					CoCreateInstance(__uuidof(PPTXFile::CAVSOfficeDrawingConverter), NULL, CLSCTX_ALL, __uuidof(PPTXFile::IAVSOfficeDrawingConverter), (void**) &pDrawingConverter);
					IUnknown* pThis = NULL;
					this->QueryInterface( __uuidof(IUnknown), (void**)&pThis );
					pDrawingConverter->SetMainDocument(pThis);
					RELEASEINTERFACE(pThis);
					BSTR bstrMediaPath = sMediaPath.AllocSysString();
					pDrawingConverter->SetMediaDstPath(bstrMediaPath);
					SysFreeString(bstrMediaPath);
					m_pCurFileWriter = new Writers::FileWriter(sDirectoryOut, m_sFontDir, nVersion, m_bSaveChartAsImg, pDrawingConverter, pArray, sThemePath);

					//папка с картинками
					TCHAR tFolder[256];
					TCHAR tDrive[256];
					_tsplitpath( bstrFileIn, tDrive, tFolder, NULL, NULL );
					CString sFolder = CString(tFolder);
					CString sDrive = CString(tDrive);
					CString sFileInDir = sDrive + sFolder;

					VARIANT var;
					var.vt = VT_BSTR;
					var.bstrVal = sFileInDir.AllocSysString();
					pDrawingConverter->SetAdditionalParam(L"SourceFileDir", var);
					RELEASESYSSTRING(var.bstrVal);

					BinDocxRW::BinaryFileReader oBinaryFileReader(sFileInDir, oBufferedStream, *m_pCurFileWriter);
					oBinaryFileReader.ReadFile();

					if(NULL != pDrawingConverter)
					{
						VARIANT vt;
						pDrawingConverter->GetAdditionalParam(_T("ContentTypes"), &vt);
						if(VT_BSTR == vt.vt)
							m_pCurFileWriter->m_oContentTypesWriter.AddOverrideRaw(CString(vt.bstrVal));
					}

					m_pCurFileWriter->m_oCommentsWriter.Write();
					m_pCurFileWriter->m_oChartWriter.Write();
					m_pCurFileWriter->m_oStylesWriter.Write();
					m_pCurFileWriter->m_oNumberingWriter.Write();
					m_pCurFileWriter->m_oFontTableWriter.Write();
					m_pCurFileWriter->m_oHeaderFooterWriter.Write();
					//Setting пишем после HeaderFooter, чтобы заполнить evenAndOddHeaders
					m_pCurFileWriter->m_oSettingWriter.Write();
					//Document пишем после HeaderFooter, чтобы заполнить sectPr
					m_pCurFileWriter->m_oDocumentWriter.Write();
					//Rels и ContentTypes пишем в конце
					//m_pCurFileWriter->m_oDocumentRelsWriter.Write(_T("document.xml.rels"));
					m_pCurFileWriter->m_oContentTypesWriter.Write();

					//CSerializer oSerializer = CSerializer();
					//if(false != oSerializer.Write(oBufferedStream, sDirectoryOut))
					//{
					bResultOk = true;
					//}
					RELEASEINTERFACE(pDrawingConverter);
				}
				RELEASEARRAY(pArray);
			}
			oMappingFile.Close();
		}
		return bResultOk ? S_OK : S_FALSE;
	}
	STDMETHOD(GetXmlContent)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, BSTR* bsXml)
	{
		Streams::CBuffer oBuffer;
		Streams::CBufferedStream oBufferedStream;
		oBufferedStream.SetBuffer(&oBuffer);
		oBufferedStream.Create((BYTE*)pBinaryObj->pvData, pBinaryObj->rgsabound[0].cElements);

		oBufferedStream.Seek(lStart);
		long nLength = oBufferedStream.ReadLong();

		Writers::ContentWriter oTempContentWriter;
		BinDocxRW::Binary_DocumentTableReader oBinary_DocumentTableReader(oBufferedStream, *m_pCurFileWriter, oTempContentWriter, NULL);
		int res = oBinary_DocumentTableReader.Read1(nLength, &BinDocxRW::Binary_DocumentTableReader::ReadDocumentContent, &oBinary_DocumentTableReader, NULL);

		(*bsXml) = oTempContentWriter.m_oContent.GetData().AllocSysString();

		return S_OK;
	}
private:
	void CreateDocument(CString strDirectory, CString& sThemePath, CString& sMediaPath)
	{
		HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();

		// rels
		CString strRels = strDirectory + _T("\\_rels");
		CreateDirectory(strRels, NULL);

		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_RELS), _T("DOCXWR"), strRels + _T("\\.rels"));



		// docProps
		CString strDocProps = strDirectory + _T("\\docProps");
		CreateDirectory(strDocProps, NULL);

		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_APP),  _T("DOCXWR"), strDocProps + _T("\\app.xml"));
		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_CORE), _T("DOCXWR"), strDocProps + _T("\\core.xml"));

		// word
		CString strWord = strDirectory + _T("\\word");
		CreateDirectory(strWord, NULL);

		sMediaPath = strWord + _T("\\media");

		//LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_SETTINGS),		_T("DOCXWR"), strWord + _T("\\settings.xml"));
		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_WEBSETTINGS),	_T("DOCXWR"), strWord + _T("\\webSettings.xml"));

		// theme
		CString strTheme = strWord + _T("\\theme");
		CreateDirectory(strTheme, NULL);
		sThemePath = strTheme + _T("\\theme1.xml");
		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_THEME),	_T("DOCXWR"), sThemePath);

		// documentRels
		CreateDirectory(strWord + _T("\\_rels"), NULL);
	}
	void LoadResourceFile(HINSTANCE hInst, LPCTSTR sResName, LPCTSTR sResType, const CString& strDstFile)
	{
		HRSRC hrRes = FindResource(hInst, sResName, sResType);
		if (!hrRes)
			return;

		HGLOBAL hGlobal = LoadResource(hInst, hrRes);
		DWORD sz = SizeofResource(hInst, hrRes);
		void* ptrRes = LockResource(hGlobal);

		CFile oFile;
		oFile.CreateFile(strDstFile);
		oFile.WriteFile(ptrRes, sz);

		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}	
};