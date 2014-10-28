#include "DocxSerializer.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Path.h"
#include "../BinWriter/BinWriters.h"
#include "../BinReader/Readers.h"
#include "../../ASCOfficePPTXFile/Editor/FontPicker.h"
#include "../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

int BinDocxRW::g_nCurFormatVersion = 0;

BinDocxRW::CDocxSerializer::CDocxSerializer()
{
	m_oBinaryFileWriter = NULL;
	m_pCurFileWriter = NULL;
	m_bIsNoBase64Save = false;
	m_bSaveChartAsImg = false;
}
bool BinDocxRW::CDocxSerializer::ConvertDocxToDoct(const CString& sSrcFileName, const CString& sDstFileName, const CString& sTmpDir, const CString& sXMLOptions)
{
	std::wstring strDirSrc = NSSystemPath::Combine(string2std_string(sTmpDir), _T("from"));
	std::wstring strEditorBin = NSSystemPath::Combine(strDirSrc, _T("Editor.bin"));
	std::wstring strDirDst = NSSystemPath::Combine(string2std_string(sTmpDir), _T("to"));
	CString sDirSrc = std_string2string(strDirSrc);
	CString sEditorBin = std_string2string(strEditorBin);
	COfficeUtils oCOfficeUtils(NULL);
	if(oCOfficeUtils.ExtractToDirectory(string2std_string(sSrcFileName), strDirSrc, NULL, 0))
		if(saveToFile(sDirSrc, sEditorBin, sXMLOptions))
			if(oCOfficeUtils.CompressFileOrDirectory(strDirDst, string2std_string(sDstFileName), -1))
				return true;
	return false;
}
bool BinDocxRW::CDocxSerializer::ConvertDoctToDocx(const CString& sSrcFileName, const CString& sDstFileName, const CString& sTmpDir, const CString& sXMLOptions, const CString& sThemePath, const CString& sMediaPath)
{
	std::wstring strDirSrc = NSSystemPath::Combine(string2std_string(sTmpDir), _T("from"));
	std::wstring strEditorBin = NSSystemPath::Combine(strDirSrc, _T("Editor.bin"));
	std::wstring strDirDst = NSSystemPath::Combine(string2std_string(sTmpDir), _T("to"));
	CString sDirSrc = std_string2string(strDirSrc);
	CString sEditorBin = std_string2string(strEditorBin);
	COfficeUtils oCOfficeUtils(NULL);
	if(oCOfficeUtils.ExtractToDirectory(string2std_string(sSrcFileName), strDirSrc, NULL, 0))
		if(loadFromFile(sDirSrc, sEditorBin, sXMLOptions, sThemePath, sMediaPath))
			if(oCOfficeUtils.CompressFileOrDirectory(strDirDst, string2std_string(sDstFileName), -1))
				return true;
	return false;
}
bool BinDocxRW::CDocxSerializer::saveToFile(const CString& sSrcFileName, const CString& sDstPath, const CString& sXMLOptions)
{
	//create mediadir
	OOX::CPath path(sSrcFileName);
	CString mediaDir = path.GetDirectory() + _T("media\\");
	NSDirectory::CreateDirectory(string2std_string(mediaDir));

	COfficeFontPicker* pFontPicker = new COfficeFontPicker();
	pFontPicker->Init(m_sFontDir);
	CFontManager* pFontManager = pFontPicker->get_FontManager();
	DocWrapper::FontProcessor fp;
	fp.setFontManager(pFontManager);
	NSBinPptxRW::CDrawingConverter oDrawingConverter;
	oDrawingConverter.SetFontManager(pFontManager);
	NSBinPptxRW::CBinaryFileWriter& oBufferedStream = *oDrawingConverter.m_pBinaryWriter;

	NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager = NULL;
	if(false == m_sEmbeddedFontsDir.IsEmpty())
	{
		NSDirectory::CreateDirectory(string2std_string(m_sEmbeddedFontsDir));

		pFontPicker->SetEmbeddedFontsDirectory(m_sEmbeddedFontsDir);

		pEmbeddedFontsManager = pFontPicker->GetNativeCutter();

		//добавляем весь латинский алфавит для списков.
		pEmbeddedFontsManager->CheckString(CString(_T("abcdefghijklmnopqrstuvwxyz")));

		//добавим мега шрифт
		pEmbeddedFontsManager->CheckFont(_T("Wingdings 3"), fp.getFontManager());
		pEmbeddedFontsManager->CheckFont(_T("Arial"), fp.getFontManager());
		//pEmbeddedFontsManager добавляются все цифры
	}

	oDrawingConverter.SetFontDir(m_sFontDir);
	oDrawingConverter.SetFontPicker(pFontPicker);
	oDrawingConverter.SetMainDocument(this);
	oDrawingConverter.SetMediaDstPath(mediaDir);
	ParamsWriter oParamsWriter(&oBufferedStream, &fp, &oDrawingConverter, pEmbeddedFontsManager);
	m_oBinaryFileWriter = new BinaryFileWriter(oParamsWriter);
	m_oBinaryFileWriter->intoBindoc(sDstPath);
	BYTE* pbBinBuffer = oBufferedStream.GetBuffer();
	int nBinBufferLen = oBufferedStream.GetPosition();

	if (m_bIsNoBase64Save)
	{
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(string2std_string(sSrcFileName));
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
			oFile.CreateFileW(string2std_string(sSrcFileName));
			oFile.WriteStringUTF8(string2std_string(m_oBinaryFileWriter->WriteFileHeader(nBinBufferLen)));
			oFile.WriteFile(pbBase64Buffer, nBase64BufferLen);
			oFile.CloseFile();
		}
		RELEASEARRAYOBJECTS(pbBase64Buffer);
	}
	RELEASEOBJECT(m_oBinaryFileWriter);
	RELEASEOBJECT(pFontPicker);
	return true;
}
bool BinDocxRW::CDocxSerializer::loadFromFile(const CString& sSrcFileName, const CString& sDstPath, const CString& sXMLOptions, const CString& sThemePath, const CString& sMediaPath)
{
	bool bResultOk = false;
	NSFile::CFileBinary oFile;
	if(oFile.OpenFile(string2std_string(sSrcFileName)))
	{
		DWORD nBase64DataSize = 0;
		BYTE* pBase64Data = new BYTE[oFile.GetFileSize()];
		oFile.ReadFile(pBase64Data, oFile.GetFileSize(), nBase64DataSize);
		oFile.CloseFile();

		//проверяем формат
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
			//Читаем из файла версию и длину base64
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
			BYTE* pData = new BYTE[nDataSize];
			if(FALSE != Base64::Base64Decode((LPCSTR)(pBase64Data + nIndex), nBase64DataSize - nIndex, pData, &nDataSize))
			{
				NSBinPptxRW::CDrawingConverter oDrawingConverter;
				NSBinPptxRW::CBinaryFileReader& oBufferedStream = *oDrawingConverter.m_pReader;
				oBufferedStream.Init(pData, 0, nDataSize);

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
				oDrawingConverter.SetMainDocument(this);
				oDrawingConverter.SetMediaDstPath(sMediaPath);
				m_pCurFileWriter = new Writers::FileWriter(sDstPath, m_sFontDir, nVersion, m_bSaveChartAsImg, &oDrawingConverter, sThemePath);

				//папка с картинками
				std::wstring strFileInDir = NSSystemPath::GetDirectoryName(string2std_string(sSrcFileName));
				CString sFileInDir = strFileInDir.c_str();

				VARIANT var;
				var.vt = VT_BSTR;
#ifdef _WIN32
				var.bstrVal = sFileInDir.AllocSysString();
				oDrawingConverter.SetAdditionalParam(CString(L"SourceFileDir"), var);
				RELEASESYSSTRING(var.bstrVal);
#else
				var.bstrVal = sFileInDir.GetString();
				oDrawingConverter.SetAdditionalParam(CString(L"SourceFileDir"), var);
#endif

				BinaryFileReader oBinaryFileReader(sFileInDir, oBufferedStream, *m_pCurFileWriter);
				oBinaryFileReader.ReadFile();

				VARIANT vt;
				oDrawingConverter.GetAdditionalParam(CString(_T("ContentTypes")), &vt);
				if(VT_BSTR == vt.vt)
					m_pCurFileWriter->m_oContentTypesWriter.AddOverrideRaw(CString(vt.bstrVal));

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
			}
		}
		RELEASEARRAYOBJECTS(pBase64Data);
	}
	return bResultOk;
}
bool BinDocxRW::CDocxSerializer::getXmlContent(NSBinPptxRW::CBinaryFileReader& oBufferedStream, long lLength, CString& sOutputXml)
{
	long nLength = oBufferedStream.GetLong();
	Writers::ContentWriter oTempContentWriter;
	BinDocxRW::Binary_DocumentTableReader oBinary_DocumentTableReader(oBufferedStream, *m_pCurFileWriter, oTempContentWriter, m_pCurFileWriter->m_pComments);
	int res = oBinary_DocumentTableReader.Read1(nLength, &BinDocxRW::Binary_DocumentTableReader::ReadDocumentContent, &oBinary_DocumentTableReader, NULL);

	sOutputXml = oTempContentWriter.m_oContent.GetData().GetString();
	return true;
}
bool BinDocxRW::CDocxSerializer::getBinaryContent(const CString& bsTxContent, NSBinPptxRW::CBinaryFileWriter& oBufferedStream, long& lDataSize)
{
	if(NULL == m_oBinaryFileWriter)
		return false;
	long nStartPos = oBufferedStream.GetPosition();

	XmlUtils::CXmlLiteReader oReader;
	oReader.FromString(bsTxContent);
	oReader.ReadNextNode();//v:textbox
	CString sRootName = oReader.GetName();
	if(_T("v:textbox") == sRootName)
		oReader.ReadNextNode();//w:txbxContent

	OOX::Logic::CSdtContent oSdtContent;
	oSdtContent.fromXML(oReader);
	BinDocxRW::ParamsWriter& oParamsWriter = m_oBinaryFileWriter->m_oParamsWriter;
	NSBinPptxRW::CBinaryFileWriter* pBufferedStreamOld = oParamsWriter.m_pCBufferedStream;
	oParamsWriter.m_pCBufferedStream = &oBufferedStream;

	BinDocxRW::BinaryCommonWriter oBinaryCommonWriter(oParamsWriter);
	int nCurPos = oBinaryCommonWriter.WriteItemWithLengthStart();
	BinDocxRW::ParamsDocumentWriter oParams(oParamsWriter.m_pCurRels, oParamsWriter.m_sCurDocumentPath);
	BinDocxRW::BinaryDocumentTableWriter oBinaryDocumentTableWriter(oParamsWriter, oParams, &oParamsWriter.m_mapIgnoreComments, NULL);
	oBinaryDocumentTableWriter.WriteDocumentContent(oSdtContent.m_arrItems);
	oBinaryCommonWriter.WriteItemWithLengthEnd(nCurPos);

	oParamsWriter.m_pCBufferedStream = pBufferedStreamOld;
	long nEndPos = oBufferedStream.GetPosition();
	lDataSize = nEndPos - nStartPos;
	return true;
}
void BinDocxRW::CDocxSerializer::setFontDir(const CString& sFontDir)
{
	m_sFontDir = sFontDir;
}
void BinDocxRW::CDocxSerializer::setEmbeddedFontsDir(const CString& sEmbeddedFontsDir)
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
