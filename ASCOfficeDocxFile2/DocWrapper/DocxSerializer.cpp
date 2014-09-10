#include "DocxSerializer.h"

#ifdef _WIN32
#include <atlbase.h>
#include <atlstr.h>
#else
#include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/File.h"
#include "../BinWriter/BinWriters.h"
#include "../BinReader/Readers.h"

#ifndef _WIN32
#include "../../DesktopEditor/common/Types.h"
#endif

int BinDocxRW::g_nCurFormatVersion = 0;

#define BUFFER_GROW_SIZE 1 * 1024 * 1024 //1mb

BinDocxRW::CDocxSerializer::CDocxSerializer()
{
	m_oBinaryFileWriter = NULL;
	m_pCurFileWriter = NULL;
	m_bIsNoBase64Save = false;
	m_bSaveChartAsImg = false;
	m_pInterface = NULL;
}
bool BinDocxRW::CDocxSerializer::saveToFile(std::wstring& sSrcFileName, std::wstring& sDstPath, std::wstring& sXMLOptions)
{
	//create mediadir
	OOX::CPath path(std_string2string(sSrcFileName));
	CString mediaDir = path.GetDirectory() + _T("media\\");
	NSDirectory::CreateDirectory(string2std_string(mediaDir));

	Streams::CBuffer oBuffer;
	oBuffer.Create(BUFFER_GROW_SIZE, BUFFER_GROW_SIZE);
	Streams::CBufferedStream oBufferedStream;
	oBufferedStream.SetBuffer(&oBuffer);

#ifdef _WIN32
	DocWrapper::FontProcessor fp;
	fp.setFontDir(std_string2string(m_sFontDir));
	PPTXFile::IOfficeFontPicker* pFontPicker = NULL;
	CoCreateInstance(__uuidof(PPTXFile::COfficeFontPicker), NULL, CLSCTX_ALL, __uuidof(PPTXFile::IOfficeFontPicker), (void**)(&pFontPicker));
	BSTR bstrFontDir1 = std_string2string(m_sFontDir).AllocSysString();
	pFontPicker->Init(bstrFontDir1);
	SysFreeString(bstrFontDir1);
	NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager = NULL;
	if(false == std_string2string(m_sEmbeddedFontsDir).IsEmpty())
	{
		NSDirectory::CreateDirectory(m_sEmbeddedFontsDir);

		BSTR bstrEmbeddedFontsDirectory = std_string2string(m_sEmbeddedFontsDir).AllocSysString();
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

	BSTR bstrFontDir = std_string2string(m_sFontDir).AllocSysString();
	pOfficeDrawingConverter->SetFontDir(bstrFontDir);
	SysFreeString(bstrFontDir);
	VARIANT vt;
	vt.vt = VT_UNKNOWN;
	vt.punkVal = pFontPicker;
	pOfficeDrawingConverter->SetAdditionalParam(_T("FontPicker"), vt);
	pOfficeDrawingConverter->SetMainDocument(m_pInterface);
	BSTR bstrMediaDir = mediaDir.AllocSysString();
	pOfficeDrawingConverter->SetMediaDstPath(bstrMediaDir);
	SysFreeString(bstrMediaDir);
	ParamsWriter oParamsWriter(oBufferedStream, fp, pOfficeDrawingConverter, pEmbeddedFontsManager);
	m_oBinaryFileWriter = new BinaryFileWriter(oParamsWriter);
	m_oBinaryFileWriter->intoBindoc(std_string2string(sDstPath));
#endif
	BYTE* pbBinBuffer = oBufferedStream.GetBuffer();
	int nBinBufferLen = oBufferedStream.GetPosition();

	if (m_bIsNoBase64Save)
	{
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(sSrcFileName);
		oFile.WriteFile(pbBinBuffer, nBinBufferLen);
		oFile.CloseFile();
	}
	else
	{
		int nBase64BufferLen = Base64::Base64EncodeGetRequiredLength(nBinBufferLen, Base64::B64_BASE64_FLAG_NOCRLF);
		BYTE* pbBase64Buffer = new BYTE[nBase64BufferLen];
		if(TRUE == Base64::Base64Encode(pbBinBuffer, nBinBufferLen, (LPSTR)pbBase64Buffer, &nBase64BufferLen, Base64::B64_BASE64_FLAG_NOCRLF))
		{
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(sSrcFileName);
			oFile.WriteStringUTF8(string2std_string(m_oBinaryFileWriter->WriteFileHeader(nBinBufferLen)));
			oFile.WriteFile(pbBase64Buffer, nBase64BufferLen);
			oFile.CloseFile();
		}
	}
#ifdef _WIN32
	RELEASEOBJECT(m_oBinaryFileWriter);
	RELEASEINTERFACE(pFontPicker);
	RELEASEINTERFACE(pOfficeDrawingConverter);
#endif
	return true;
}
bool BinDocxRW::CDocxSerializer::loadFromFile(std::wstring& sSrcFileName, std::wstring& sDstPath, std::wstring& sXMLOptions, std::wstring& sThemePath, std::wstring& sMediaPath)
{
	bool bResultOk = false;
	NSFile::CFileBinary oFile;
	if(oFile.OpenFile(sSrcFileName))
	{
		DWORD nBase64DataSize = 0;
		BYTE* pBase64Data = new BYTE[oFile.GetFileSize()];
		oFile.ReadFile(pBase64Data, oFile.GetFileSize(), nBase64DataSize);
		oFile.CloseFile();

		//провер€ем формат
		bool bValidFormat = false;
		CString sSignature(g_sFormatSignature);
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

				int nVersion = g_nFormatVersion;
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
				pDrawingConverter->SetMainDocument(m_pInterface);
				BSTR bstrMediaPath = std_string2string(sMediaPath).AllocSysString();
				pDrawingConverter->SetMediaDstPath(bstrMediaPath);
				SysFreeString(bstrMediaPath);
				m_pCurFileWriter = new Writers::FileWriter(std_string2string(sDstPath), std_string2string(m_sFontDir), nVersion, m_bSaveChartAsImg, pDrawingConverter, pArray, std_string2string(sThemePath));

				//папка с картинками
				TCHAR tFolder[256];
				TCHAR tDrive[256];
				_tsplitpath( sSrcFileName.c_str(), tDrive, tFolder, NULL, NULL );
				CString sFolder = CString(tFolder);
				CString sDrive = CString(tDrive);
				CString sFileInDir = sDrive + sFolder;

				VARIANT var;
				var.vt = VT_BSTR;
				var.bstrVal = sFileInDir.AllocSysString();
				pDrawingConverter->SetAdditionalParam(L"SourceFileDir", var);
				RELEASESYSSTRING(var.bstrVal);

				BinaryFileReader oBinaryFileReader(sFileInDir, oBufferedStream, *m_pCurFileWriter);
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
		RELEASEARRAYOBJECTS(pBase64Data);
	}
	return bResultOk;
}
bool BinDocxRW::CDocxSerializer::GetXmlContent(unsigned char* pBinaryObj, long lSize, long lStart, long lLength, std::wstring& sOutputXml)
{
	Streams::CBuffer oBuffer;
	Streams::CBufferedStream oBufferedStream;
	oBufferedStream.SetBuffer(&oBuffer);
	oBufferedStream.Create(pBinaryObj, lSize);

	oBufferedStream.Seek(lStart);
	long nLength = oBufferedStream.ReadLong();

	Writers::ContentWriter oTempContentWriter;
	BinDocxRW::Binary_DocumentTableReader oBinary_DocumentTableReader(oBufferedStream, *m_pCurFileWriter, oTempContentWriter, NULL);
	int res = oBinary_DocumentTableReader.Read1(nLength, &BinDocxRW::Binary_DocumentTableReader::ReadDocumentContent, &oBinary_DocumentTableReader, NULL);

	sOutputXml = oTempContentWriter.m_oContent.GetData().GetString();
	return true;
}
bool BinDocxRW::CDocxSerializer::GetBinaryContent(std::wstring& bsTxContent, unsigned char** ppBinary, long &lDataSize)
{
	if(NULL == m_oBinaryFileWriter)
		return false;
	Streams::CBuffer oBuffer;
	oBuffer.Create(BUFFER_GROW_SIZE, BUFFER_GROW_SIZE);
	Streams::CBufferedStream oBufferedStream;
	oBufferedStream.SetBuffer(&oBuffer);

	XmlUtils::CXmlLiteReader oReader;
	oReader.FromString(std_string2string(bsTxContent));
	oReader.ReadNextNode();//v:textbox
	CString sRootName = oReader.GetName();
	if(_T("v:textbox") == sRootName)
		oReader.ReadNextNode();//w:txbxContent

	OOX::Logic::CSdtContent oSdtContent;
	oSdtContent.fromXML(oReader);
	BinDocxRW::ParamsWriter oCurParamsWriter(m_oBinaryFileWriter->m_oParamsWriter);
	BinDocxRW::ParamsWriter oParamsWriter(oBufferedStream, oCurParamsWriter.m_oFontProcessor, oCurParamsWriter.m_pOfficeDrawingConverter, oCurParamsWriter.m_pEmbeddedFontsManager);
	oParamsWriter.m_poTheme = oCurParamsWriter.m_poTheme;
	oParamsWriter.m_oSettings = oCurParamsWriter.m_oSettings;
	oParamsWriter.m_pCurRels = oCurParamsWriter.m_pCurRels;
	oParamsWriter.m_sCurDocumentPath = oCurParamsWriter.m_sCurDocumentPath;

	BinDocxRW::BinaryCommonWriter oBinaryCommonWriter(oParamsWriter);
	int nCurPos = oBinaryCommonWriter.WriteItemWithLengthStart();
	BinDocxRW::BinaryDocumentTableWriter oBinaryDocumentTableWriter(oParamsWriter, BinDocxRW::ParamsDocumentWriter(oParamsWriter.m_pCurRels, oParamsWriter.m_sCurDocumentPath), NULL, NULL);
	oBinaryDocumentTableWriter.WriteDocumentContent(oSdtContent.m_arrItems);
	oBinaryCommonWriter.WriteItemWithLengthEnd(nCurPos);

	if (NULL != ppBinary)
	{
		lDataSize = oBufferedStream.GetPosition();
		*ppBinary = new unsigned char[lDataSize];
		BYTE* pDataS = oBufferedStream.GetBuffer();
		memcpy(*ppBinary, pDataS, lDataSize);
	}
	return true;
}
void BinDocxRW::CDocxSerializer::setFontDir(std::wstring& sFontDir)
{
	m_sFontDir = sFontDir;
}
void BinDocxRW::CDocxSerializer::setEmbeddedFontsDir(std::wstring& sEmbeddedFontsDir)
{
	m_sEmbeddedFontsDir = sEmbeddedFontsDir;
}
void BinDocxRW::CDocxSerializer::setIsNoBase64Save(bool bIsNoBase64Save)
{
	m_bIsNoBase64Save = bIsNoBase64Save;
}
void BinDocxRW::CDocxSerializer::setSaveChartAsImg(bool bSaveChartAsImg)
{
	m_bSaveChartAsImg = bSaveChartAsImg;
}
#ifdef _WIN32
void BinDocxRW::CDocxSerializer::setComInterface(IUnknown* pInterface)
{
	m_pInterface = pInterface;
}
#endif