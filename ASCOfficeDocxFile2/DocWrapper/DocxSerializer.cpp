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
#include "DocxSerializer.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Path.h"
#include "../BinWriter/BinWriters.h"
#include "../BinReader/Readers.h"
#include "../../ASCOfficePPTXFile/Editor/FontPicker.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

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
    std::wstring strDirSrc      = NSSystemPath::Combine(string2std_string(sTmpDir), _T("from"));
    std::wstring strDirDst      = NSSystemPath::Combine(string2std_string(sTmpDir), _T("to"));
    std::wstring strEditorBin   = NSSystemPath::Combine(strDirDst, _T("Editor.bin"));

    NSDirectory::CreateDirectory(strDirSrc);
	NSDirectory::CreateDirectory(strDirDst);

    CString sDirSrc = std_string2string(strDirSrc);
	CString sEditorBin = std_string2string(strEditorBin);

    COfficeUtils oCOfficeUtils(NULL);

    if(S_OK == oCOfficeUtils.ExtractToDirectory(string2std_string(sSrcFileName), strDirSrc, NULL, 0))
		if(saveToFile(sEditorBin, sDirSrc, sXMLOptions))
            if(S_OK == oCOfficeUtils.CompressFileOrDirectory(strDirDst, string2std_string(sDstFileName), -1))
				return true;
	return false;
}
bool BinDocxRW::CDocxSerializer::ConvertDoctToDocx(const CString& sSrcFileName, const CString& sDstFileName, const CString& sTmpDir, const CString& sXMLOptions)
{
	std::wstring strDirSrc		= NSSystemPath::Combine(string2std_string(sTmpDir), _T("from"));
	std::wstring strEditorBin	= NSSystemPath::Combine(strDirSrc, _T("Editor.bin"));
	std::wstring strDirDst		= NSSystemPath::Combine(string2std_string(sTmpDir), _T("to"));
	
	NSDirectory::CreateDirectory(strDirSrc);
	NSDirectory::CreateDirectory(strDirDst);
	
	CString sEditorBin = std_string2string(strEditorBin);

    COfficeUtils oCOfficeUtils(NULL);

    if(S_OK == oCOfficeUtils.ExtractToDirectory(string2std_string(sSrcFileName), strDirSrc, NULL, 0))
    {
        CString sMediaPath;
        CString sThemePath;
		CString sEmbedPath;

        CreateDocxFolders(std_string2string(strDirDst), sThemePath, sMediaPath, sEmbedPath);

        if(loadFromFile(sEditorBin, std_string2string(strDirDst), sXMLOptions, sThemePath, sMediaPath, sEmbedPath))
        {
            if(S_OK == oCOfficeUtils.CompressFileOrDirectory(strDirDst, string2std_string(sDstFileName), -1))
				return true;
        }
    }
    return false;
}
bool BinDocxRW::CDocxSerializer::saveToFile(const CString& sSrcFileName, const CString& sDstPath, const CString& sXMLOptions)
{
	OOX::CPath pathMain(sSrcFileName);
    
	OOX::CPath pathMedia = pathMain.GetDirectory() + FILE_SEPARATOR_STR + _T("media");
	NSDirectory::CreateDirectory(string2std_string(pathMedia.GetPath()));

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
	oDrawingConverter.SetMediaDstPath(pathMedia.GetPath());
	
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
        BYTE* pbBase64Buffer = new BYTE[nBase64BufferLen + 64];
//        if(TRUE == Base64::Base64Encode(pbBinBuffer, nBinBufferLen, (LPSTR)pbBase64Buffer, &nBase64BufferLen, Base64::B64_BASE64_FLAG_NOCRLF))
        if(true == Base64_1::Base64Encode(pbBinBuffer, nBinBufferLen, pbBase64Buffer, &nBase64BufferLen))
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

bool BinDocxRW::CDocxSerializer::CreateDocxFolders(CString strDirectory, CString& sThemePath, CString& sMediaPath, CString& sEmbedPath)
{
	bool res = true;
	// rels
    OOX::CPath pathRels = strDirectory + FILE_SEPARATOR_STR + _T("_rels");
	if (!NSDirectory::CreateDirectory(pathRels.GetPath().GetBuffer()))	res = false;

	// word
    OOX::CPath pathWord = strDirectory + FILE_SEPARATOR_STR + _T("word");
    if (!NSDirectory::CreateDirectory(pathWord.GetPath().GetBuffer()))	res = false;

	// documentRels
    OOX::CPath pathWordRels = pathWord + FILE_SEPARATOR_STR + _T("_rels");
    if (!NSDirectory::CreateDirectory(pathWordRels.GetPath().GetBuffer()))res = false;

	//media
    OOX::CPath pathMedia = pathWord + FILE_SEPARATOR_STR + _T("media");
	if (!NSDirectory::CreateDirectory(pathMedia.GetPath().GetBuffer()))		res = false;
	sMediaPath = pathMedia.GetPath();

	//embeddings
    OOX::CPath pathEmbeddings = pathWord + FILE_SEPARATOR_STR + _T("embeddings");
	if (!NSDirectory::CreateDirectory(pathEmbeddings.GetPath().GetBuffer()))res = false;
	sEmbedPath = pathEmbeddings.GetPath();

	// theme
    OOX::CPath pathTheme = pathWord + FILE_SEPARATOR_STR + _T("theme");
	if (!NSDirectory::CreateDirectory(pathTheme.GetPath().GetBuffer()))		res = false;

    OOX::CPath pathThemeRels = pathTheme + FILE_SEPARATOR_STR + _T("_rels");
	if (!NSDirectory::CreateDirectory(pathThemeRels.GetPath().GetBuffer())) res = false;
	
    pathTheme = pathTheme + FILE_SEPARATOR_STR + _T("theme1.xml");
	sThemePath = pathTheme.GetPath();

	return res;
}
bool BinDocxRW::CDocxSerializer::loadFromFile(const CString& sSrcFileName, const CString& sDstPath, const CString& sXMLOptions, const CString& sThemePath, const CString& sMediaPath, const CString& sEmbedPath)
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
			
            if(false != Base64::Base64Decode((LPCSTR)(pBase64Data + nIndex), nBase64DataSize - nIndex, pData, &nDataSize))
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
				oDrawingConverter.SetEmbedDstPath(sEmbedPath);
				m_pCurFileWriter = new Writers::FileWriter(sDstPath, m_sFontDir, nVersion, m_bSaveChartAsImg, &oDrawingConverter, sThemePath);

				//папка с картинками
				std::wstring strFileInDir = NSSystemPath::GetDirectoryName(string2std_string(sSrcFileName));
				CString sFileInDir = strFileInDir.c_str();

				VARIANT var;
				var.vt = VT_BSTR;
#if defined(_WIN32) || defined (_WIN64)
				var.bstrVal = sFileInDir.AllocSysString();
				oDrawingConverter.SetAdditionalParam(CString(L"SourceFileDir"), var);
				RELEASESYSSTRING(var.bstrVal);
#else
				var.bstrVal = sFileInDir.GetString();
				oDrawingConverter.SetAdditionalParam(CString(L"SourceFileDir"), var);
#endif
	//default theme
				m_pCurFileWriter->m_oDefaultTheme.Write(sThemePath);
				
				BinaryFileReader oBinaryFileReader(sFileInDir, oBufferedStream, *m_pCurFileWriter);
				oBinaryFileReader.ReadFile();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				OOX::CContentTypes oContentTypes;
	//docProps
                OOX::CPath pathDocProps = sDstPath + FILE_SEPARATOR_STR + _T("docProps");
                FileSystem::Directory::CreateDirectory(pathDocProps.GetPath());
				
                OOX::CPath DocProps = CString(_T("docProps"));

				OOX::CApp* pApp = new OOX::CApp();
				if (pApp)
				{
					pApp->SetApplication(_T("OnlyOffice"));
					pApp->SetAppVersion(_T("3.0000"));
					pApp->SetDocSecurity(0);
					pApp->SetScaleCrop(false);
					pApp->SetLinksUpToDate(false);
					pApp->SetSharedDoc(false);
					pApp->SetHyperlinksChanged(false);
					
					pApp->write(pathDocProps + FILE_SEPARATOR_STR + _T("app.xml"), DocProps, oContentTypes);
					delete pApp;
				}				
				OOX::CCore* pCore = new OOX::CCore();
				if (pCore)
				{
					pCore->SetCreator(_T(""));
					pCore->SetLastModifiedBy(_T(""));
					pCore->write(pathDocProps + FILE_SEPARATOR_STR + _T("core.xml"), DocProps, oContentTypes);
					delete pCore;
				} 
/////////////////////////////////////////////////////////////////////////////////////
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
				m_pCurFileWriter->m_oWebSettingsWriter.Write();
				//Document пишем после HeaderFooter, чтобы заполнить sectPr
				m_pCurFileWriter->m_oDocumentWriter.Write();
				//Rels и ContentTypes пишем в конце
				m_pCurFileWriter->m_oDocumentRelsWriter.Write();
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
	
//    CString bsTxContentTemp = _T("<root xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");
//
//    bsTxContentTemp += bsTxContent;
//    bsTxContentTemp + _T("</root>");
    
    CString sBegin(_T("<root xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">"));
    
    CString sEnd(_T("</root>"));
    CString bsTxContentTemp = sBegin + bsTxContent + sEnd;
	
	OOX::Logic::CSdtContent oSdtContent;
	if (oReader.FromString(bsTxContentTemp))
	{
        oReader.ReadNextNode();//root
        oReader.ReadNextNode();//v:textbox
		CString sRootName = XmlUtils::GetNameNoNS(oReader.GetName());
		if(_T("textbox") == sRootName)
			oReader.ReadNextNode();//w:txbxContent
		oSdtContent.fromXML(oReader);
	}
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
