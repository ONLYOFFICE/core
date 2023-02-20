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
#include "DocxSerializer.h"
 
#include "../BinWriter/BinWriters.h"
#include "../BinReader/Readers.h"

#include "../../../PPTXFormat/DrawingConverter/ASCOfficeDrawingConverter.h"
#include "../../Presentation/FontPicker.h"
#include "../../../PPTXFormat/Logic/HeadingVariant.h"

#include "FontProcessor.h"
#include "../../../../OfficeUtils/src/OfficeUtils.h"

#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../../../DesktopEditor/common/Path.h"
#include "../../../../DesktopEditor/common/SystemUtils.h"

#include "../../../DocxFormat/App.h"
#include "../../../DocxFormat/Core.h"
#include "../../../DocxFormat/Endnote.h"
#include "../../../DocxFormat/Footnote.h"

int BinDocxRW::g_nCurFormatVersion = 0;

namespace BinDocxRW
{
	class CPackageFile
	{
	public:
		CPackageFile() {}
		virtual ~CPackageFile() {}

		bool unpackage(const std::wstring& sSrcFileName, const std::wstring& sDstPath)
		{
			m_sDestPath = sDstPath;

			XmlUtils::CXmlLiteReader oReader;

			if (!oReader.FromFile(sSrcFileName))
				return false;

			if (!oReader.ReadNextNode())
				return false;

			std::wstring sName = oReader.GetName();
			if (L"pkg:package" != sName)
				return false;

			int nDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nDepth))
			{
				sName = oReader.GetName();

				if (L"pkg:part" == sName)
				{
					read_package_part(oReader);
				}
			}

			m_oContentTypes.Write(m_sDestPath);
			return true;
		}
	private:
		std::wstring m_sDestPath;
		OOX::CContentTypes m_oContentTypes;
		
		void read_package_part(XmlUtils::CXmlLiteReader &oReader)
		{
			nullable_int padding;
			nullable_string compression;
			nullable_string contentType;
			nullable_string name;

			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"pkg:name", name)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pkg:contentType", contentType)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pkg:padding", padding)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pkg:compression", compression)
			WritingElement_ReadAttributes_End(oReader)

			if (name.IsInit() == false) return;

			if (contentType.IsInit())
			{
				m_oContentTypes.Registration(*contentType, L"", *name);
			}
				
			int nDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nDepth))
			{
				std::wstring  sName = oReader.GetName();

				if (L"pkg:xmlData" == sName)
				{
					std::wstring data = oReader.GetInnerXml();
					XmlUtils::replace_all(data, L"&#xA;", L"");
					XmlUtils::replace_all(data, L"&#x9;", L"");
					//todooo убрать "красивую" разметку xml
					WriteXmlFile(*name, data);
				}
				if (L"pkg:binaryData" == sName)
				{
					std::string data = oReader.GetText2A();
					WriteBinaryFile(*name, data, compression.IsInit() ? compression.get() : L"");
				}
			}
		}
		void WriteXmlFile(const std::wstring & name, std::wstring & data)
		{
			if (name.empty()) return;

			OOX::CPath path(m_sDestPath + name);
			
			NSDirectory::CreateDirectories(path.GetDirectory());

			NSFile::CFileBinary file;
			if (file.CreateFileW(path.GetPath()))
			{
				file.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n");
				file.WriteStringUTF8(data);
				file.CloseFile();
			}
		}
		void WriteBinaryFile(const std::wstring & name, std::string & data, const std::wstring & compression)
		{
			if (name.empty()) return;
			
			OOX::CPath path(m_sDestPath + name);
			
			NSDirectory::CreateDirectories(path.GetDirectory());
			
			NSFile::CFileBinary file;
			if (file.CreateFileW(path.GetPath()))
			{
				BYTE* pDstBuffer = NULL;
				int dstLen = Base64::Base64DecodeGetRequiredLength((int)data.size());

				pDstBuffer = new BYTE[dstLen];
				if (pDstBuffer)
				{
					Base64::Base64Decode(data.c_str(), (int)data.size(), pDstBuffer, &dstLen);

					file.WriteFile(pDstBuffer, (DWORD)dstLen);
					file.CloseFile();

					delete []pDstBuffer;
				}
			}
		}
	};
}

BinDocxRW::CDocxSerializer::CDocxSerializer() : m_bIsMacro(false), m_bIsNoBase64Save(false), m_bIsNoBase64(false), m_bIsOForm(false)
{
	m_pParamsWriter		= NULL;
	m_pCurFileWriter	= NULL;
}
BinDocxRW::CDocxSerializer::~CDocxSerializer()
{
	RELEASEOBJECT(m_pParamsWriter);
	RELEASEOBJECT(m_pCurFileWriter);
}
bool BinDocxRW::CDocxSerializer::saveToFile(const std::wstring& sDstFileName, const std::wstring& sSrcPath, const std::wstring& sXMLOptions, const std::wstring& sTempPath)
{
	OOX::CPath pathMain(sDstFileName);
    
	OOX::CPath pathMedia = pathMain.GetDirectory() + FILE_SEPARATOR_STR + _T("media");
	NSDirectory::CreateDirectory(pathMedia.GetPath());

	COfficeFontPicker* pFontPicker = new COfficeFontPicker();
	pFontPicker->Init(m_sFontDir);
    NSFonts::IFontManager* pFontManager = pFontPicker->get_FontManager();
	DocWrapper::FontProcessor fp;
	fp.setFontManager(pFontManager);
	
	NSBinPptxRW::CDrawingConverter oDrawingConverter;
	oDrawingConverter.SetFontManager(pFontManager);
	NSBinPptxRW::CBinaryFileWriter& oBufferedStream = *oDrawingConverter.m_pBinaryWriter;

	NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager = NULL;
    if(false == m_sEmbeddedFontsDir.empty())
	{
		NSDirectory::CreateDirectory(m_sEmbeddedFontsDir);

		pFontPicker->SetEmbeddedFontsDirectory(m_sEmbeddedFontsDir);

		pEmbeddedFontsManager = pFontPicker->GetNativeCutter();

		//добавляем весь латинский алфавит для списков.
        pEmbeddedFontsManager->CheckString(std::wstring(_T("abcdefghijklmnopqrstuvwxyz")));

		//добавим мега шрифт
		pEmbeddedFontsManager->CheckFont(_T("Wingdings 3"), fp.getFontManager());
		pEmbeddedFontsManager->CheckFont(_T("Arial"), fp.getFontManager());
		//pEmbeddedFontsManager добавляются все цифры
	}

	oDrawingConverter.SetFontDir(m_sFontDir);
	oDrawingConverter.SetFontPicker(pFontPicker);
	oDrawingConverter.SetMainDocument(this);

    oDrawingConverter.SetDstPath(pathMain.GetDirectory() + FILE_SEPARATOR_STR + L"word");
	oDrawingConverter.SetMediaDstPath(pathMedia.GetPath());
	oDrawingConverter.SetTempPath(sTempPath);
	
	m_pParamsWriter = new ParamsWriter(&oBufferedStream, &fp, &oDrawingConverter, pEmbeddedFontsManager);

	BinaryFileWriter oBinaryFileWriter(*m_pParamsWriter);
	
	if (m_bIsNoBase64)
	{
		oBufferedStream.WriteStringUtf8(oBinaryFileWriter.WriteFileHeader(0, g_nFormatVersionNoBase64));
	}
	oBinaryFileWriter.intoBindoc(sSrcPath);
	
	BYTE* pbBinBuffer = oBufferedStream.GetBuffer();
	int nBinBufferLen = oBufferedStream.GetPosition();


	if (m_bIsNoBase64 || m_bIsNoBase64Save)
	{
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(sDstFileName);
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
			oFile.CreateFileW(sDstFileName);
			oFile.WriteStringUTF8(oBinaryFileWriter.WriteFileHeader(nBinBufferLen, g_nFormatVersion));
			oFile.WriteFile(pbBase64Buffer, nBase64BufferLen);
			oFile.CloseFile();
		}
		RELEASEARRAYOBJECTS(pbBase64Buffer);
	}
	RELEASEOBJECT(m_pParamsWriter);
	RELEASEOBJECT(pFontPicker);
	return true;
}
bool BinDocxRW::CDocxSerializer::CreateDocxFolders(std::wstring strDirectory, std::wstring& sThemePath, std::wstring& sMediaPath, std::wstring& sEmbedPath)
{
	bool res = true;
	// rels
    OOX::CPath pathRels = strDirectory + FILE_SEPARATOR_STR + L"_rels";
	if (!NSDirectory::CreateDirectory(pathRels.GetPath()))	res = false;

	// word
    OOX::CPath pathWord = strDirectory + FILE_SEPARATOR_STR + L"word";
    if (!NSDirectory::CreateDirectory(pathWord.GetPath()))	res = false;

	// documentRels
    OOX::CPath pathWordRels = pathWord + FILE_SEPARATOR_STR + L"_rels";
    if (!NSDirectory::CreateDirectory(pathWordRels.GetPath()))res = false;

	//media
    OOX::CPath pathMedia = pathWord + FILE_SEPARATOR_STR + L"media";
	if (!NSDirectory::CreateDirectory(pathMedia.GetPath()))		res = false;
	sMediaPath = pathMedia.GetPath();

	//embeddings
    OOX::CPath pathEmbeddings = pathWord + FILE_SEPARATOR_STR + L"embeddings";
	if (!NSDirectory::CreateDirectory(pathEmbeddings.GetPath()))res = false;
	sEmbedPath = pathEmbeddings.GetPath();

	// theme
    OOX::CPath pathTheme = pathWord + FILE_SEPARATOR_STR + L"theme";
	if (!NSDirectory::CreateDirectory(pathTheme.GetPath()))		res = false;

    OOX::CPath pathThemeRels = pathTheme + FILE_SEPARATOR_STR + L"_rels";
	if (!NSDirectory::CreateDirectory(pathThemeRels.GetPath())) res = false;
	
    pathTheme = pathTheme + FILE_SEPARATOR_STR + L"theme1.xml";
	sThemePath = pathTheme.GetPath();

	return res;
}
bool BinDocxRW::CDocxSerializer::loadFromFile(const std::wstring& sSrcFileName, const std::wstring& sDstPath, const std::wstring& sXMLOptions, const std::wstring& sThemePath, const std::wstring& sMediaPath, const std::wstring& sEmbedPath)
{
	bool bResultOk = false;
	RELEASEOBJECT(m_pCurFileWriter);
	
	NSFile::CFileBinary oFile;
	if(oFile.OpenFile(sSrcFileName))
	{
		DWORD nBase64DataSize = 0;
		BYTE* pBase64Data = new BYTE[oFile.GetFileSize()];
		oFile.ReadFile(pBase64Data, oFile.GetFileSize(), nBase64DataSize);
		oFile.CloseFile();

		//проверяем формат
		bool bValidFormat = false;
        std::wstring sSignature(g_sFormatSignature);
        int nSigLength = (int)sSignature.length();
		
		if ((int)nBase64DataSize > nSigLength)
		{
            std::string sCurSig((char*)pBase64Data, nSigLength);
            if(sSignature == std::wstring(sCurSig.begin(), sCurSig.end()))
			{
                bValidFormat = true;
            }
		}
		if(bValidFormat)
		{
			//Читаем из файла версию и длину base64
			int nIndex = nSigLength;
			int nType = 0;
            std::string version = "";
            std::string dst_len = "";
			
			while (nIndex < nBase64DataSize)
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
                    version += _c;
				else
                    dst_len += _c;
			}
			int nVersion = g_nFormatVersion;
			if(!version.empty())
			{
				version = version.substr(1);
				g_nCurFormatVersion = nVersion = std::stoi(version.c_str());
			}
			bool bIsNoBase64 = nVersion == g_nFormatVersionNoBase64;

			NSBinPptxRW::CDrawingConverter oDrawingConverter;
			NSBinPptxRW::CBinaryFileReader& oBufferedStream = *oDrawingConverter.m_pReader;
			int nDataSize = 0;
			BYTE* pData = NULL;
			if (!bIsNoBase64)
			{
				nDataSize = atoi(dst_len.c_str());
				pData = new BYTE[nDataSize];
				if(Base64::Base64Decode((const char*)(pBase64Data + nIndex), nBase64DataSize - nIndex, pData, &nDataSize))
				{
					oBufferedStream.Init(pData, 0, nDataSize);
				}
				else
				{
					RELEASEARRAYOBJECTS(pData);
				}
			}
			else
			{
				nDataSize = nBase64DataSize;
				pData = pBase64Data;
				oBufferedStream.Init(pData, 0, nDataSize);
				oBufferedStream.Seek(nIndex);
			}

			
			if (NULL != pData)
			{
				oDrawingConverter.SetMainDocument(this);
                oDrawingConverter.SetDstPath(sDstPath + FILE_SEPARATOR_STR + L"word");

				oDrawingConverter.SetMediaDstPath(sMediaPath);
				oDrawingConverter.SetEmbedDstPath(sEmbedPath);
				
				m_pCurFileWriter = new Writers::FileWriter(sDstPath, m_sFontDir, false, nVersion, &oDrawingConverter, sThemePath);

	//папка с картинками
				std::wstring strFileInDir = NSSystemPath::GetDirectoryName(sSrcFileName);
                std::wstring sFileInDir = strFileInDir.c_str();

                oDrawingConverter.SetSrcPath(sFileInDir);
				
				BinaryFileReader oBinaryFileReader(sFileInDir, oBufferedStream, *m_pCurFileWriter, m_bIsMacro, m_bIsOForm);
				oBinaryFileReader.ReadFile();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//themes
				m_pCurFileWriter->m_oTheme.Write(sThemePath);

				OOX::CContentTypes *pContentTypes = oDrawingConverter.GetContentTypes();
	//docProps
                OOX::CPath pathDocProps = sDstPath + FILE_SEPARATOR_STR + _T("docProps");
                NSDirectory::CreateDirectory(pathDocProps.GetPath());
				
                OOX::CPath DocProps = std::wstring(_T("docProps"));

				if (NULL != m_pCurFileWriter->m_pApp)
				{
					m_pCurFileWriter->m_pApp->write(pathDocProps + FILE_SEPARATOR_STR + _T("app.xml"), DocProps, *pContentTypes);
				}
				else
				{
					OOX::CApp pApp(NULL);
					pApp.SetDefaults();
					pApp.write(pathDocProps + FILE_SEPARATOR_STR + _T("app.xml"), DocProps, *pContentTypes);
				}

				if (NULL != m_pCurFileWriter->m_pCore)
				{
					m_pCurFileWriter->m_pCore->write(pathDocProps + FILE_SEPARATOR_STR + _T("core.xml"), DocProps, *pContentTypes);
				}
				else
				{
					OOX::CCore pCore(NULL);
					pCore.SetDefaults();
					pCore.write(pathDocProps + FILE_SEPARATOR_STR + _T("core.xml"), DocProps, *pContentTypes);
				}

				if (NULL != m_pCurFileWriter->m_pCustomProperties)
				{
					m_pCurFileWriter->m_pCustomProperties->write(pathDocProps + FILE_SEPARATOR_STR + _T("custom.xml"), DocProps, *pContentTypes);
				}

/////////////////////////////////////////////////////////////////////////////////////
				m_pCurFileWriter->Write();
				pContentTypes->Write(sDstPath);

				bResultOk = true;

			}
			if (!bIsNoBase64)
			{
				RELEASEARRAYOBJECTS(pData);
			}

		}
		RELEASEARRAYOBJECTS(pBase64Data);
	}
	return bResultOk;
}
bool BinDocxRW::CDocxSerializer::getXmlContent(NSBinPptxRW::CBinaryFileReader& oBufferedStream, long lLength, std::wstring& sOutputXml)
{
	long nLength = oBufferedStream.GetLong();
	Writers::ContentWriter oTempContentWriter;
	BinDocxRW::Binary_DocumentTableReader oBinary_DocumentTableReader(oBufferedStream, *m_pCurFileWriter, oTempContentWriter, m_bIsOForm);
	oBinary_DocumentTableReader.ReadDocumentContentOut(nLength);

    sOutputXml = oTempContentWriter.m_oContent.GetData();
	return true;
}
bool BinDocxRW::CDocxSerializer::getBinaryContent(const std::wstring& bsTxContent, NSBinPptxRW::CBinaryFileWriter& oBufferedStream, long& lDataSize)
{
	if(NULL == m_pParamsWriter)
		return false;
	long nStartPos = oBufferedStream.GetPosition();

	XmlUtils::CXmlLiteReader oReader;
	
    std::wstring sBegin(_T("<root xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">"));
    
    std::wstring sEnd(_T("</root>"));
    std::wstring bsTxContentTemp = sBegin + bsTxContent + sEnd;
	
	OOX::Logic::CSdtContent oSdtContent;
	if (oReader.FromString(bsTxContentTemp))
	{
        oReader.ReadNextNode();//root
        oReader.ReadNextNode();//v:textbox
		std::wstring sRootName = XmlUtils::GetNameNoNS(oReader.GetName());
		if(_T("textbox") == sRootName)
			oReader.ReadNextNode();//w:txbxContent
		oSdtContent.fromXML(oReader);
	}
	BinDocxRW::ParamsWriter& oParamsWriter = *m_pParamsWriter;
	NSBinPptxRW::CBinaryFileWriter* pBufferedStreamOld = oParamsWriter.m_pCBufferedStream;
	oParamsWriter.m_pCBufferedStream = &oBufferedStream;

	BinDocxRW::BinaryCommonWriter oBinaryCommonWriter(oParamsWriter);
	int nCurPos = oBinaryCommonWriter.WriteItemWithLengthStart();
	BinDocxRW::ParamsDocumentWriter oParams(oParamsWriter.m_pCurRels);
	
	BinDocxRW::BinaryDocumentTableWriter oBinaryDocumentTableWriter(oParamsWriter, oParams, &oParamsWriter.m_mapIgnoreComments, NULL);
	oBinaryDocumentTableWriter.WriteDocumentContent(oSdtContent.m_arrItems);
	oBinaryCommonWriter.WriteItemWithLengthEnd(nCurPos);

	oParamsWriter.m_pCBufferedStream = pBufferedStreamOld;
	long nEndPos = oBufferedStream.GetPosition();
	lDataSize = nEndPos - nStartPos;
	return true;
}
bool BinDocxRW::CDocxSerializer::getBinaryContentElem(OOX::EElementType eElemType, void* pElem, NSBinPptxRW::CBinaryFileWriter& oBufferedStream, long& lDataSize)
{
	if(NULL == m_pParamsWriter)
		return false;
	long nStartPos = oBufferedStream.GetPosition();

	BinDocxRW::ParamsWriter& oParamsWriter = *m_pParamsWriter;
	NSBinPptxRW::CBinaryFileWriter* pBufferedStreamOld = oParamsWriter.m_pCBufferedStream;
	oParamsWriter.m_pCBufferedStream = &oBufferedStream;

	BinDocxRW::BinaryCommonWriter oBinaryCommonWriter(oParamsWriter);
	int nCurPos = oBinaryCommonWriter.WriteItemWithLengthStart();
	
	BinDocxRW::ParamsDocumentWriter oParams(oParamsWriter.m_pCurRels);
	BinDocxRW::BinaryDocumentTableWriter oBinaryDocumentTableWriter(oParamsWriter, oParams, &oParamsWriter.m_mapIgnoreComments, NULL);
	if(OOX::et_m_oMathPara == eElemType)
	{
		OOX::Logic::COMathPara* pMathPara = static_cast<OOX::Logic::COMathPara*>(pElem);
		oBinaryDocumentTableWriter.WriteMathOMathPara(pMathPara->m_arrItems);
	}
	else if(OOX::et_m_oMath == eElemType)
	{
		OOX::Logic::COMath* pMath = static_cast<OOX::Logic::COMath*>(pElem);
		oBinaryDocumentTableWriter.WriteMathArgNodes(pMath->m_arrItems);
	}
	else if(OOX::et_w_sdtContent == eElemType)
	{
		OOX::Logic::CSdtContent* pContent = static_cast<OOX::Logic::CSdtContent*>(pElem);
		oBinaryDocumentTableWriter.WriteDocumentContent(pContent->m_arrItems);
	}
	oBinaryCommonWriter.WriteItemWithLengthEnd(nCurPos);

	oParamsWriter.m_pCBufferedStream = pBufferedStreamOld;
	long nEndPos = oBufferedStream.GetPosition();
	lDataSize = nEndPos - nStartPos;
	return true;
}

bool BinDocxRW::CDocxSerializer::getXmlContentElem(OOX::EElementType eType, NSBinPptxRW::CBinaryFileReader& oBufferedStream, std::wstring& sOutputXml)
{
	long nLength = oBufferedStream.GetLong();
	Writers::ContentWriter oTempContentWriter;
	BinDocxRW::Binary_DocumentTableReader oBinary_DocumentTableReader(oBufferedStream, *m_pCurFileWriter, oTempContentWriter, m_bIsOForm);

	if(OOX::et_m_oMathPara == eType)
	{
        oTempContentWriter.m_oContent.WriteString(std::wstring(_T("<m:oMathPara>")));
		oBinary_DocumentTableReader.ReadMathOMathParaOut(nLength);
        oTempContentWriter.m_oContent.WriteString(std::wstring(_T("</m:oMathPara>")));
	}
	else if(OOX::et_m_oMath == eType)
	{
        oTempContentWriter.m_oContent.WriteString(std::wstring(_T("<m:oMath>")));
		oBinary_DocumentTableReader.ReadMathArgOut(nLength);
        oTempContentWriter.m_oContent.WriteString(std::wstring(_T("</m:oMath>")));
	}


	sOutputXml = oTempContentWriter.m_oContent.GetData();
	return true;
}

void BinDocxRW::CDocxSerializer::setFontDir(const std::wstring& sFontDir)
{
	m_sFontDir = sFontDir;
}
void BinDocxRW::CDocxSerializer::setEmbeddedFontsDir(const std::wstring& sEmbeddedFontsDir)
{
	m_sEmbeddedFontsDir = sEmbeddedFontsDir;
}
void BinDocxRW::CDocxSerializer::setIsNoBase64Save(bool val)
{
	m_bIsNoBase64Save = val;
}
void BinDocxRW::CDocxSerializer::setIsNoBase64(bool val)
{
	m_bIsNoBase64 = val;
}
void BinDocxRW::CDocxSerializer::setMacroEnabled(bool val)
{
	m_bIsMacro = val;
}
void BinDocxRW::CDocxSerializer::setOFormEnabled(bool val)
{
	m_bIsOForm = val;
}
bool BinDocxRW::CDocxSerializer::unpackageFile(const std::wstring& sSrcFileName, const std::wstring& sDstPath)
{
	BinDocxRW::CPackageFile file;
	
	return file.unpackage(sSrcFileName, sDstPath);
}
bool BinDocxRW::CDocxSerializer::convertFlat(const std::wstring& sSrcFileName, const std::wstring& sDstPath)
{
	OOX::CDocxFlat docxflat(sSrcFileName);

	if (false == docxflat.m_pDocument.IsInit())
		return false;

	OOX::CDocx docx;

	if (docxflat.m_pDocument.IsInit())
	{
		NSCommon::smart_ptr<OOX::File> file = docxflat.m_pDocument.GetPointer(); file.AddRef();
		docx.Add(file);
		docx.m_oMain.document = docxflat.m_pDocument.GetPointer();
	}
	if (docxflat.m_pApp.IsInit())
	{
		NSCommon::smart_ptr<OOX::File> file(docxflat.m_pApp.GetPointer()); file.AddRef();
		docx.Add(file);
	}
	if (docxflat.m_pCore.IsInit())
	{
		NSCommon::smart_ptr<OOX::File> file(docxflat.m_pCore.GetPointer()); file.AddRef();
		docx.Add(file);
	}	
	//docxflat.m_oBgPict.GetPointer();

	return docx.Write(sDstPath);
}
