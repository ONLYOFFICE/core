/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "PdfFile.h"
#include "PdfWriter.h"
#include "PdfReader.h"
#include "PdfEditor.h"

#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/graphics/commands/DocInfo.h"
#include "Resources/BaseFonts.h"

#include "OnlineOfficeBinToPdf.h"
#include "SrcWriter/Document.h"

class CPdfFile_Private
{
public:
	std::wstring wsSrcFile;
	std::wstring wsPassword;
	std::wstring wsTempFolder;
	NSFonts::IApplicationFonts* pAppFonts;

	CPdfReader* pReader;
	CPdfWriter* pWriter;
	CPdfEditor* pEditor;
};

// ------------------------------------------------------------------------

CPdfFile::CPdfFile(NSFonts::IApplicationFonts* pAppFonts)
{
	m_pInternal = new CPdfFile_Private();

	m_pInternal->pAppFonts = pAppFonts;
	m_pInternal->pWriter = NULL;
	m_pInternal->pReader = NULL;
	m_pInternal->pEditor = NULL;
}
CPdfFile::~CPdfFile()
{
	RELEASEOBJECT(m_pInternal->pWriter);
	RELEASEOBJECT(m_pInternal->pReader);
	RELEASEOBJECT(m_pInternal->pEditor);
	RELEASEOBJECT(m_pInternal);
}
NSFonts::IFontManager* CPdfFile::GetFontManager()
{
	if (!m_pInternal->pReader)
		return NULL;
	return m_pInternal->pReader->GetFontManager();
}

void CPdfFile::Close()
{
	if (m_pInternal->pEditor)
		m_pInternal->pEditor->Close();
	else if (m_pInternal->pReader)
		m_pInternal->pReader->Close();
}
void CPdfFile::Sign(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsPicturePath, ICertificate* pCertificate)
{
	if (!m_pInternal->pWriter)
		return;
	m_pInternal->pWriter->Sign(dX, dY, dW, dH, wsPicturePath, pCertificate);
}
void CPdfFile::SetDocumentInfo(const std::wstring& wsTitle, const std::wstring& wsCreator, const std::wstring& wsSubject, const std::wstring& wsKeywords)
{
	if (!m_pInternal->pWriter)
		return;
	m_pInternal->pWriter->SetDocumentInfo(wsTitle, wsCreator, wsSubject, wsKeywords);
}
void CPdfFile::RotatePage(int nRotate)
{
	if (!m_pInternal->pWriter)
		return;
	m_pInternal->pWriter->PageRotate(nRotate);
}
bool CPdfFile::EditPdf(const std::wstring& wsDstFile)
{
	if (wsDstFile.empty() || !m_pInternal->pReader)
		return false;

	m_pInternal->pReader->CleanUp();

	RELEASEOBJECT(m_pInternal->pWriter);
	m_pInternal->pWriter = new CPdfWriter(m_pInternal->pAppFonts, false, this, true, m_pInternal->wsTempFolder);

	RELEASEOBJECT(m_pInternal->pEditor);
	m_pInternal->pEditor = new CPdfEditor(m_pInternal->wsSrcFile, m_pInternal->wsPassword, wsDstFile, m_pInternal->pReader, m_pInternal->pWriter);
	return m_pInternal->pEditor->GetError() == 0;
}
void CPdfFile::SetEditType(int nType)
{
	if (!m_pInternal->pEditor)
		return;
	if (nType == 1)
		m_pInternal->pEditor->SetMode(CPdfEditor::Mode::WriteNew);
}
bool CPdfFile::EditPage(int nPageIndex)
{
	if (!m_pInternal->pEditor)
		return false;
	return m_pInternal->pEditor->EditPage(nPageIndex);
}
bool CPdfFile::DeletePage(int nPageIndex)
{
	if (!m_pInternal->pEditor)
		return false;
	return m_pInternal->pEditor->DeletePage(nPageIndex);
}
bool CPdfFile::AddPage(int nPageIndex)
{
	if (!m_pInternal->pEditor)
		return false;
	return m_pInternal->pEditor->AddPage(nPageIndex);
}
bool CPdfFile::MergePages(const std::wstring& wsPath, int nMaxID, const std::wstring& wsPrefixForm)
{
	if (!m_pInternal->pEditor)
		return false;
	if (m_pInternal->pReader->MergePages(wsPath, L"", nMaxID, U_TO_UTF8(wsPrefixForm)))
		return m_pInternal->pEditor->MergePages(wsPath);
	return false;
}
bool CPdfFile::MovePage(int nPageIndex, int nPos)
{
	if (!m_pInternal->pEditor)
		return false;
	return m_pInternal->pEditor->MovePage(nPageIndex, nPos);
}
HRESULT CPdfFile::ChangePassword(const std::wstring& wsPath, const std::wstring& wsPassword)
{
	RELEASEOBJECT(m_pInternal->pWriter);
	m_pInternal->pWriter = new CPdfWriter(m_pInternal->pAppFonts, false, this, false, m_pInternal->wsTempFolder);
	return _ChangePassword(wsPath, wsPassword, m_pInternal->pReader, m_pInternal->pWriter);
}

// ------------------------------------------------------------------------

int CPdfFile::GetError()
{
	if (!m_pInternal->pReader)
		return 1;
	return m_pInternal->pReader->GetError();
}
bool CPdfFile::IsNeedCMap()
{
	if (!m_pInternal->pReader)
		return false;
	return m_pInternal->pReader->IsNeedCMap();
}
void CPdfFile::SetCMapMemory(BYTE* pData, DWORD nSizeData)
{
	if (m_pInternal->pReader)
		m_pInternal->pReader->SetCMapMemory(pData, nSizeData);
}
void CPdfFile::SetCMapFolder(const std::wstring& sFolder)
{
	if (m_pInternal->pReader)
		m_pInternal->pReader->SetCMapFolder(sFolder);
}
void CPdfFile::SetCMapFile(const std::wstring& sFile)
{
	if (m_pInternal->pReader)
		m_pInternal->pReader->SetCMapFile(sFile);
}
void CPdfFile::ToXml(const std::wstring& sFile, bool bSaveStreams)
{
	if (m_pInternal->pReader)
		m_pInternal->pReader->ToXml(sFile, bSaveStreams);
}

bool CPdfFile::GetMetaData(const std::wstring& sFile, const std::wstring& sMetaName, BYTE** pMetaData, DWORD& nMetaLength)
{
	NSFile::CFileBinary oFile;
	if (!oFile.OpenFile(sFile))
		return false;

	int nBufferSize = 4096;
	BYTE* pBuffer = new BYTE[nBufferSize];
	if (!pBuffer)
	{
		oFile.CloseFile();
		return false;
	}

	DWORD nReadBytes = 0;
	if (!oFile.ReadFile(pBuffer, nBufferSize, nReadBytes))
	{
		RELEASEARRAYOBJECTS(pBuffer);
		oFile.CloseFile();
		return false;
	}
	oFile.CloseFile();
	pBuffer[nReadBytes - 1] = '\0';

	char* pFirst = strstr((char*)pBuffer, "%\315\312\322\251\015");

	if (!pFirst || pFirst - (char*)pBuffer + 6 >= nReadBytes)
	{
		RELEASEARRAYOBJECTS(pBuffer);
		return false;
	}
	pFirst += 6;

	if (strncmp(pFirst, "1 0 obj\012<<\012", 11) != 0 || pFirst - (char*)pBuffer + 11 >= nReadBytes)
	{
		RELEASEARRAYOBJECTS(pBuffer);
		return false;
	}
	pFirst += 11;

	std::string sMeta = U_TO_UTF8(sMetaName);
	char* pStream = strstr(pFirst, "stream\015\012");
	char* pMeta = strstr(pFirst, sMeta.c_str());
	if (!pStream || !pMeta || pStream < pMeta)
	{
		RELEASEARRAYOBJECTS(pBuffer);
		return false;
	}
	pStream += 8;
	int nStreamBegin = (int)(pStream - (char*)pBuffer);
	pMeta += sMeta.length() + 3;

	char* pMetaLast = strstr(pMeta, " ");
	if (!pMetaLast)
	{
		RELEASEARRAYOBJECTS(pBuffer);
		return false;
	}
	std::string sMetaOffset = std::string(pMeta, pMetaLast - pMeta);
	int nMetaOffset = std::stoi(sMetaOffset);

	pMeta = pMetaLast + 1;
	pMetaLast = strstr(pMeta, " ");
	if (!pMetaLast)
	{
		RELEASEARRAYOBJECTS(pBuffer);
		return false;
	}
	std::string sMetaSize = std::string(pMeta, pMetaLast - pMeta);
	nMetaLength = std::stoi(sMetaSize);

	RELEASEARRAYOBJECTS(pBuffer);
	*pMetaData = new BYTE[nMetaLength];
	pBuffer = *pMetaData;
	nReadBytes = 0;
	if (!oFile.OpenFile(sFile) || !oFile.SeekFile(nStreamBegin + nMetaOffset) || !oFile.ReadFile(pBuffer, nMetaLength, nReadBytes))
	{
		RELEASEARRAYOBJECTS(pBuffer);
		oFile.CloseFile();
		return false;
	}
	oFile.CloseFile();
	nMetaLength = nReadBytes;

	return true;
}
bool CPdfFile::LoadFromFile(const std::wstring& file, const std::wstring& options, const std::wstring& owner_password, const std::wstring& user_password)
{
	m_pInternal->pReader = new CPdfReader(m_pInternal->pAppFonts);
	if (!m_pInternal->pReader)
		return false;
	m_pInternal->wsSrcFile  = file;
	m_pInternal->wsPassword = owner_password;
	if (!m_pInternal->wsTempFolder.empty())
		m_pInternal->pReader->SetTempDirectory(m_pInternal->wsTempFolder);
	return m_pInternal->pReader->LoadFromFile(m_pInternal->pAppFonts, file, owner_password, user_password) && (m_pInternal->pReader->GetError() == 0);
}
bool CPdfFile::LoadFromMemory(BYTE* data, DWORD length, const std::wstring& options, const std::wstring& owner_password, const std::wstring& user_password)
{
	m_pInternal->pReader = new CPdfReader(m_pInternal->pAppFonts);
	if (!m_pInternal->pReader)
		return false;
	m_pInternal->wsSrcFile.clear();
	m_pInternal->wsPassword = owner_password;
	return m_pInternal->pReader->LoadFromMemory(m_pInternal->pAppFonts, data, length, owner_password, user_password) && (m_pInternal->pReader->GetError() == 0);
}
NSFonts::IApplicationFonts* CPdfFile::GetFonts()
{
	return m_pInternal->pAppFonts;
}
OfficeDrawingFileType CPdfFile::GetType()
{
	return odftPDF;
}
std::wstring CPdfFile::GetTempDirectory()
{
	return m_pInternal->wsTempFolder;
}
void CPdfFile::SetTempDirectory(const std::wstring& wsPath)
{
	m_pInternal->wsTempFolder = wsPath;
	if (m_pInternal->pReader)
		m_pInternal->pReader->SetTempDirectory(wsPath);
	if (m_pInternal->pWriter)
		m_pInternal->pWriter->SetTempDirectory(wsPath);
}
int CPdfFile::GetPagesCount()
{
	if (!m_pInternal->pReader)
		return 0;
	if (m_pInternal->pEditor)
	{
		int nWPages = m_pInternal->pEditor->GetPagesCount();
		if (nWPages > 0)
			return nWPages;
	}
	return m_pInternal->pReader->GetNumPages();
}
void CPdfFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	if (!m_pInternal->pReader)
		return;
	if (m_pInternal->pEditor)
		m_pInternal->pEditor->GetPageInfo(nPageIndex, pdWidth, pdHeight, pdDpiX, pdDpiY);
	else
		m_pInternal->pReader->GetPageInfo(nPageIndex, pdWidth, pdHeight, pdDpiX, pdDpiY);
}
bool CPdfFile::MergePages(BYTE* data, DWORD length, int nMaxID, const std::string& sPrefixForm)
{
	if (!m_pInternal->pReader)
	{
		free(data);
		return false;
	}
	return m_pInternal->pReader->MergePages(data, length, L"", nMaxID, sPrefixForm) && (m_pInternal->pReader->GetError() == 0);
}
bool CPdfFile::UnmergePages()
{
	if (!m_pInternal->pReader)
		return false;
	return m_pInternal->pReader->UnmergePages();
}
bool CPdfFile::RedactPage(int nPageIndex, double* arrRedactBox, int nLengthX8, BYTE* pChanges, int nLength)
{
	if (!m_pInternal->pReader)
	{
		free(pChanges);
		return false;
	}
	return m_pInternal->pReader->RedactPage(nPageIndex, arrRedactBox, nLengthX8, pChanges, nLength);
}
bool CPdfFile::UndoRedact()
{
	if (!m_pInternal->pReader)
		return true;
	return m_pInternal->pReader->UndoRedact();
}
bool CPdfFile::CheckOwnerPassword(const std::wstring& sPassword)
{
	if (!m_pInternal->pReader)
		return false;
	bool bRes = m_pInternal->pReader->CheckOwnerPassword(sPassword);
	if (bRes)
		m_pInternal->wsPassword = sPassword;
	else if (!m_pInternal->wsPassword.empty())
		bRes = m_pInternal->pReader->CheckOwnerPassword(m_pInternal->wsPassword);

	return bRes;
}
bool CPdfFile::CheckPerm(int nPerm)
{
	if (!m_pInternal->pReader)
		return false;
	return m_pInternal->pReader->CheckPerm(nPerm);
}
int CPdfFile::GetRotate(int nPageIndex)
{
	if (!m_pInternal->pReader)
		return 0;
	if (m_pInternal->pEditor)
		return m_pInternal->pEditor->GetRotate(nPageIndex);
	return m_pInternal->pReader->GetRotate(nPageIndex);
}
int CPdfFile::GetMaxRefID()
{
	if (!m_pInternal->pReader)
		return 0;
	return m_pInternal->pReader->GetMaxRefID();
}
bool CPdfFile::ValidMetaData()
{
	if (!m_pInternal->pReader)
		return false;
	return m_pInternal->pReader->ValidMetaData();
}
void CPdfFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak, COfficeDrawingPageParams* pParams)
{
	if (!m_pInternal->pReader)
		return;
	pRenderer->CommandLong(c_nPenWidth0As1px, 1);
	m_pInternal->pReader->SetParams(pParams);
	m_pInternal->pReader->DrawPageOnRenderer(pRenderer, nPageIndex, pBreak);
}
std::wstring CPdfFile::GetInfo()
{
	if (!m_pInternal->pReader)
		return std::wstring();
	return m_pInternal->pReader->GetInfo();
}
BYTE* CPdfFile::GetStructure()
{
	if (!m_pInternal->pReader)
		return NULL;
	return m_pInternal->pReader->GetStructure();
}
BYTE* CPdfFile::GetLinks(int nPageIndex)
{
	if (!m_pInternal->pReader)
		return NULL;
	return m_pInternal->pReader->GetLinks(nPageIndex);
}
BYTE* CPdfFile::GetWidgets()
{
	if (!m_pInternal->pReader)
		return NULL;
	return m_pInternal->pReader->GetWidgets();
}
BYTE* CPdfFile::GetAnnotEmbeddedFonts()
{
	if (!m_pInternal->pReader)
		return NULL;
	return m_pInternal->pReader->GetFonts(false);
}
BYTE* CPdfFile::GetAnnotStandardFonts()
{
	if (!m_pInternal->pReader)
		return NULL;
	return m_pInternal->pReader->GetFonts(true);
}
std::wstring CPdfFile::GetFontPath(const std::wstring& wsFontName)
{
	if (!m_pInternal->pReader)
		return L"";
	return m_pInternal->pReader->GetFontPath(wsFontName, false);
}
std::wstring CPdfFile::GetEmbeddedFontPath(const std::wstring& wsFontName)
{
	if (!m_pInternal->pReader)
		return L"";
	return m_pInternal->pReader->GetFontPath(wsFontName);
}
BYTE* CPdfFile::GetAnnots(int nPageIndex)
{
	if (!m_pInternal->pReader)
		return NULL;
	return m_pInternal->pReader->GetAnnots(nPageIndex);
}
BYTE* CPdfFile::SplitPages(const int* arrPageIndex, unsigned int unLength, BYTE* pChanges, DWORD nLength)
{
	if (!m_pInternal->pReader)
		return NULL;
	RELEASEOBJECT(m_pInternal->pWriter);
	m_pInternal->pWriter = new CPdfWriter(m_pInternal->pAppFonts, false, this);

	RELEASEOBJECT(m_pInternal->pEditor);
	m_pInternal->pEditor = new CPdfEditor(m_pInternal->wsSrcFile, m_pInternal->wsPassword, L"", m_pInternal->pReader, m_pInternal->pWriter, CPdfEditor::Mode::Split);

	BYTE* pRes = NULL;
	int nLen = 0;
	if (m_pInternal->pEditor->SplitPages(arrPageIndex, unLength))
	{
		if (pChanges && nLength > 3)
		{
			m_pInternal->pWriter->SetSplit(true);
			CConvertFromBinParams* pParams = new CConvertFromBinParams();
			AddToPdfFromBinary(pChanges + 4, nLength - 4, pParams);
			m_pInternal->pWriter->SetSplit(false);
		}
		m_pInternal->pEditor->AfterSplitPages();

		if (m_pInternal->pWriter->SaveToMemory(&pRes, &nLen) != 0)
		{
			RELEASEMEM(pRes);
		}
		else
			pRes = m_pInternal->pReader->StreamToCData(pRes, nLen);
	}

	RELEASEOBJECT(m_pInternal->pWriter);
	RELEASEOBJECT(m_pInternal->pEditor);
	return pRes;
}
BYTE* CPdfFile::VerifySign(const std::wstring& sFile, ICertificate* pCertificate, int nWidget)
{
	if (!m_pInternal->pReader)
		return NULL;
	return m_pInternal->pReader->VerifySign(sFile, pCertificate, nWidget);
}
BYTE* CPdfFile::GetAPWidget(int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nWidget, const char* sView, const char* sButtonView)
{
	if (!m_pInternal->pReader)
		return NULL;
	return m_pInternal->pReader->GetAPWidget(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nWidget, sView, sButtonView);
}
BYTE* CPdfFile::GetButtonIcon(int nBackgroundColor, int nPageIndex, bool bBase64, int nButtonWidget, const char* sIconView)
{
	if (!m_pInternal->pReader)
		return NULL;
	return m_pInternal->pReader->GetButtonIcon(nBackgroundColor, nPageIndex, bBase64, nButtonWidget, sIconView);
}
BYTE* CPdfFile::GetAPAnnots(int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nAnnot, const char* sView)
{
	if (!m_pInternal->pReader)
		return NULL;
	return m_pInternal->pReader->GetAPAnnots(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nAnnot, sView);
}

// ------------------------------------------------------------------------

void CPdfFile::CreatePdf(bool isPDFA)
{
	RELEASEOBJECT(m_pInternal->pWriter);
	m_pInternal->pWriter = new CPdfWriter(m_pInternal->pAppFonts, isPDFA, this, true, m_pInternal->wsTempFolder);
}
int CPdfFile::SaveToFile(const std::wstring& wsPath)
{
	if (!m_pInternal->pWriter)
		return 1;
	return m_pInternal->pWriter->SaveToFile(wsPath);
}
void CPdfFile::SetPassword(const std::wstring& wsPassword)
{
	if (m_pInternal->pWriter)
		m_pInternal->pWriter->SetPassword(wsPassword);
}
void CPdfFile::SetDocumentID(const std::wstring& wsDocumentID)
{
	if (m_pInternal->pWriter)
		m_pInternal->pWriter->SetDocumentID(wsDocumentID);
}
void CPdfFile::AddMetaData(const std::wstring& sMetaName, BYTE* pMetaData, DWORD nMetaLength)
{
	if (m_pInternal->pWriter)
		m_pInternal->pWriter->AddMetaData(sMetaName, pMetaData, nMetaLength);
}
HRESULT CPdfFile::OnlineWordToPdf(const std::wstring& wsSrcFile, const std::wstring& wsDstFile, CConvertFromBinParams* pParams)
{
	if (!m_pInternal->pWriter || !NSOnlineOfficeBinToPdf::ConvertBinToPdf(this, wsSrcFile, wsDstFile, false, pParams))
		return S_FALSE;
	return S_OK;
}
HRESULT CPdfFile::OnlineWordToPdfFromBinary(const std::wstring& wsSrcFile, const std::wstring& wsDstFile, CConvertFromBinParams* pParams)
{
	if (!m_pInternal->pWriter || !NSOnlineOfficeBinToPdf::ConvertBinToPdf(this, wsSrcFile, wsDstFile, true, pParams))
		return S_FALSE;
	return S_OK;
}
HRESULT CPdfFile::AddToPdfFromBinary(BYTE* pBuffer, unsigned int nLen, CConvertFromBinParams* pParams)
{
	if (!m_pInternal->pEditor || !NSOnlineOfficeBinToPdf::AddBinToPdf(this, pBuffer, nLen, pParams))
		return S_FALSE;
	return S_OK;
}
HRESULT CPdfFile::DrawImageWith1bppMask(IGrObject* pImage, NSImages::CPixJbig2* pMaskBuffer, const unsigned int& unMaskWidth, const unsigned int& unMaskHeight, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->DrawImageWith1bppMask(pImage, pMaskBuffer, unMaskWidth, unMaskHeight, dX, dY, dW, dH);
}
HRESULT CPdfFile::DrawImage1bpp(NSImages::CPixJbig2* pImageBuffer, const unsigned int& unWidth, const unsigned int& unHeight, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->DrawImage1bpp(pImageBuffer, unWidth, unHeight, dX, dY, dW, dH);
}
HRESULT CPdfFile::SetLinearGradient(const double& dX1, const double& dY1, const double& dX2, const double& dY2)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->SetLinearGradient(dX1, dY1, dX2, dY2);
}
HRESULT CPdfFile::SetRadialGradient(const double& dX1, const double& dY1, const double& dR1, const double& dX2, const double& dY2, const double& dR2)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->SetRadialGradient(dX1, dY1, dR1, dX2, dY2, dR2);
}

HRESULT CPdfFile::get_Type(LONG* lType)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	*lType = c_nPDFWriter;
	return S_OK;
}
HRESULT CPdfFile::NewPage()
{
	if (!m_pInternal->pWriter || m_pInternal->pEditor)
		return S_FALSE;
	return m_pInternal->pWriter->NewPage();
}
HRESULT CPdfFile::get_Height(double* dHeight)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_Height(dHeight);
}
HRESULT CPdfFile::put_Height(const double& dHeight)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	if (m_pInternal->pEditor)
	{
		if (m_pInternal->pEditor->IsEditPage())
			return S_OK;
		return m_pInternal->pWriter->put_Height(dHeight, false);
	}
	return m_pInternal->pWriter->put_Height(dHeight);
}
HRESULT CPdfFile::get_Width(double* dWidth)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_Width(dWidth);
}
HRESULT CPdfFile::put_Width(const double& dWidth)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	if (m_pInternal->pEditor)
	{
		if (m_pInternal->pEditor->IsEditPage())
			return S_OK;
		return m_pInternal->pWriter->put_Width(dWidth, false);
	}
	return m_pInternal->pWriter->put_Width(dWidth);
}
HRESULT CPdfFile::get_DpiX(double* dDpiX)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	*dDpiX = 72;
	return S_OK;
}
HRESULT CPdfFile::get_DpiY(double* dDpiY)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	*dDpiY = 72;
	return S_OK;
}
HRESULT CPdfFile::get_PenColor(LONG* lColor)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_PenColor(lColor);
}
HRESULT CPdfFile::put_PenColor(const LONG& lColor)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_PenColor(lColor);
}
HRESULT CPdfFile::get_PenAlpha(LONG* lAlpha)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_PenAlpha(lAlpha);
}
HRESULT CPdfFile::put_PenAlpha(const LONG& lAlpha)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_PenAlpha(lAlpha);
}
HRESULT CPdfFile::get_PenSize(double* dSize)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_PenSize(dSize);
}
HRESULT CPdfFile::put_PenSize(const double& dSize)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_PenSize(dSize);
}
HRESULT CPdfFile::get_PenDashStyle(BYTE* nDashStyle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_PenDashStyle(nDashStyle);
}
HRESULT CPdfFile::put_PenDashStyle(const BYTE& nDashStyle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_PenDashStyle(nDashStyle);
}
HRESULT CPdfFile::get_PenLineStartCap(BYTE* nCapStyle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_PenLineStartCap(nCapStyle);
}
HRESULT CPdfFile::put_PenLineStartCap(const BYTE& nCapStyle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_PenLineStartCap(nCapStyle);
}
HRESULT CPdfFile::get_PenLineEndCap(BYTE* nCapStyle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_PenLineEndCap(nCapStyle);
}
HRESULT CPdfFile::put_PenLineEndCap(const BYTE& nCapStyle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_PenLineEndCap(nCapStyle);
}
HRESULT CPdfFile::get_PenLineJoin(BYTE* nJoinStyle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_PenLineJoin(nJoinStyle);
}
HRESULT CPdfFile::put_PenLineJoin(const BYTE& nJoinStyle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_PenLineJoin(nJoinStyle);
}
HRESULT CPdfFile::get_PenDashOffset(double* dOffset)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_PenDashOffset(dOffset);
}
HRESULT CPdfFile::put_PenDashOffset(const double& dOffset)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_PenDashOffset(dOffset);
}
HRESULT CPdfFile::get_PenAlign(LONG* lAlign)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_PenAlign(lAlign);
}
HRESULT CPdfFile::put_PenAlign(const LONG& lAlign)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_PenAlign(lAlign);
}
HRESULT CPdfFile::get_PenMiterLimit(double* dMiter)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_PenMiterLimit(dMiter);
}
HRESULT CPdfFile::put_PenMiterLimit(const double& dMiter)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_PenMiterLimit(dMiter);
}
HRESULT CPdfFile::PenDashPattern(double* pPattern, LONG lCount)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PenDashPattern(pPattern, lCount);
}
HRESULT CPdfFile::get_BrushType(LONG* lType)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_BrushType(lType);
}
HRESULT CPdfFile::put_BrushType(const LONG& lType)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushType(lType);
}
HRESULT CPdfFile::get_BrushColor1(LONG* lColor)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_BrushColor1(lColor);
}
HRESULT CPdfFile::put_BrushColor1(const LONG& lColor)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushColor1(lColor);
}
HRESULT CPdfFile::get_BrushAlpha1(LONG* lAlpha)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_BrushAlpha1(lAlpha);
}
HRESULT CPdfFile::put_BrushAlpha1(const LONG& lAlpha)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushAlpha1(lAlpha);
}
HRESULT CPdfFile::get_BrushColor2(LONG* lColor)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_BrushColor2(lColor);
}
HRESULT CPdfFile::put_BrushColor2(const LONG& lColor)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushColor2(lColor);
}
HRESULT CPdfFile::get_BrushAlpha2(LONG* lAlpha)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_BrushAlpha2(lAlpha);
}
HRESULT CPdfFile::put_BrushAlpha2(const LONG& lAlpha)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushAlpha2(lAlpha);
}
HRESULT CPdfFile::get_BrushTexturePath(std::wstring* wsPath)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_BrushTexturePath(wsPath);
}
HRESULT CPdfFile::put_BrushTexturePath(const std::wstring& wsPath)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushTexturePath(wsPath);
}
HRESULT CPdfFile::get_BrushTextureMode(LONG* lMode)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_BrushTextureMode(lMode);
}
HRESULT CPdfFile::put_BrushTextureMode(const LONG& lMode)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushTextureMode(lMode);
}
HRESULT CPdfFile::get_BrushTextureAlpha(LONG* lAlpha)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_BrushTextureAlpha(lAlpha);
}
HRESULT CPdfFile::put_BrushTextureAlpha(const LONG& lAlpha)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushTextureAlpha(lAlpha);
}
HRESULT CPdfFile::get_BrushLinearAngle(double* dAngle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_BrushLinearAngle(dAngle);
}
HRESULT CPdfFile::put_BrushLinearAngle(const double& dAngle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushLinearAngle(dAngle);
}
HRESULT CPdfFile::BrushRect(const INT& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->BrushRect(nVal, dLeft, dTop, dWidth, dHeight);
}
HRESULT CPdfFile::BrushBounds(const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
{
	return m_pInternal ? S_OK : S_FALSE;
}
HRESULT CPdfFile::put_BrushGradientColors(LONG* pColors, double* pPositions, LONG lCount)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushGradientColors(pColors, pPositions, lCount);
}
HRESULT CPdfFile::get_BrushTextureImage(Aggplus::CImage** pImage)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_BrushTextureImage(pImage);
}
HRESULT CPdfFile::put_BrushTextureImage(Aggplus::CImage* pImage)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushTextureImage(pImage);
}
HRESULT CPdfFile::get_BrushTransform(Aggplus::CMatrix& oMatrix)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_BrushTransform(oMatrix);
}
HRESULT CPdfFile::put_BrushTransform(const Aggplus::CMatrix& oMatrix)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_BrushTransform(oMatrix);
}

HRESULT CPdfFile::get_FontName(std::wstring* wsName)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_FontName(wsName);
}
HRESULT CPdfFile::put_FontName(const std::wstring& wsName)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	std::wstring wsFont = wsName;
	if (m_pInternal->pEditor && wsName.find(L"Embedded: ") == 0)
	{
		std::wstring sSub = wsName.substr(10);
		bool bBold = false, bItalic = false;
		std::wstring wsFontPath;
		if (m_pInternal->pEditor->IsBase14(sSub, bBold, bItalic, wsFontPath))
		{
			if (bBold || bItalic)
			{
				LONG lStyle = 0;
				if (bBold)
					lStyle |= 1;
				if (bItalic)
					lStyle |= 2;
				put_FontStyle(lStyle);
			}

			NSFonts::IFontsMemoryStorage* pMemoryStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
			if (wsFontPath == sSub && (!pMemoryStorage || !pMemoryStorage->Get(wsFontPath)))
			{
				const BYTE* pData14 = NULL;
				unsigned int nSize14 = 0;
				std::wstring wsTempFileName = m_pInternal->wsTempFolder + L"/" + wsFontPath + L".base";
				if (NSFile::CFileBinary::Exists(wsTempFileName))
					wsFontPath = wsTempFileName;
				else if (PdfReader::GetBaseFont(sSub, pData14, nSize14))
				{
					NSFile::CFileBinary oFile;
					if (oFile.CreateFileW(wsTempFileName))
					{
						oFile.WriteFile((BYTE*)pData14, nSize14);
						wsFontPath = wsTempFileName;
					}
					else if (!wsTempFileName.empty())
						NSFile::CFileBinary::Remove(wsTempFileName);
					oFile.CloseFile();
				}
			}
		}
		else
			wsFont = sSub;
		m_pInternal->pWriter->AddFont(wsFont, bBold, bItalic, wsFontPath, 0);
	}
	return m_pInternal->pWriter->put_FontName(wsFont);
}
HRESULT CPdfFile::get_FontPath(std::wstring* wsPath)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_FontPath(wsPath);
}
HRESULT CPdfFile::put_FontPath(const std::wstring& wsPath)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_FontPath(wsPath);
}
HRESULT CPdfFile::get_FontSize(double* dSize)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_FontSize(dSize);
}
HRESULT CPdfFile::put_FontSize(const double& dSize)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_FontSize(dSize);
}
HRESULT CPdfFile::get_FontStyle(LONG* lStyle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_FontStyle(lStyle);
}
HRESULT CPdfFile::put_FontStyle(const LONG& lStyle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_FontStyle(lStyle);
}
HRESULT CPdfFile::get_FontStringGID(INT* bGid)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_FontStringGID(bGid);
}
HRESULT CPdfFile::put_FontStringGID(const INT& bGid)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_FontStringGID(bGid);
}
HRESULT CPdfFile::get_FontCharSpace(double* dSpace)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_FontCharSpace(dSpace);
}
HRESULT CPdfFile::put_FontCharSpace(const double& dSpace)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_FontCharSpace(dSpace);
}
HRESULT CPdfFile::get_FontFaceIndex(int* lFaceIndex)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_FontFaceIndex(lFaceIndex);
}
HRESULT CPdfFile::put_FontFaceIndex(const int& lFaceIndex)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_FontFaceIndex(lFaceIndex);
}
HRESULT CPdfFile::CommandDrawTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->CommandDrawTextCHAR(lUnicode, dX, dY, dW, dH);
}
HRESULT CPdfFile::CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->CommandDrawTextExCHAR(lUnicode, lGid, dX, dY, dW, dH);
}
HRESULT CPdfFile::CommandDrawText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->CommandDrawText(wsUnicodeText, dX, dY, dW, dH);
}
HRESULT CPdfFile::CommandDrawTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->CommandDrawTextEx(wsUnicodeText, pGids, nGidsCount, dX, dY, dW, dH);
}
HRESULT CPdfFile::CommandDrawTextCHAR2(unsigned int* unUnicode, const unsigned int& unUnicodeCount, const unsigned int& unGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->CommandDrawTextCHAR2(unUnicode, unUnicodeCount, unGid, dX, dY, dW, dH);
}
HRESULT CPdfFile::BeginCommand(const DWORD& lType)
{
	return m_pInternal->pWriter ? S_OK : S_FALSE;
}
HRESULT CPdfFile::EndCommand(const DWORD& lType)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->EndCommand(lType);
}
HRESULT CPdfFile::PathCommandMoveTo(const double& dX, const double& dY)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandMoveTo(dX, dY);
}
HRESULT CPdfFile::PathCommandLineTo(const double& dX, const double& dY)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandLineTo(dX, dY);
}
HRESULT CPdfFile::PathCommandLinesTo(double* pPoints, const int& nCount)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandLinesTo(pPoints, nCount);
}
HRESULT CPdfFile::PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandCurveTo(dX1, dY1, dX2, dY2, dXe, dYe);
}
HRESULT CPdfFile::PathCommandCurvesTo(double* pPoints, const int& nCount)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandCurvesTo(pPoints, nCount);
}
HRESULT CPdfFile::PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandArcTo(dX, dY, dW, dH, dStartAngle, dSweepAngle);
}
HRESULT CPdfFile::PathCommandClose()
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandClose();
}
HRESULT CPdfFile::PathCommandEnd()
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandEnd();
}
HRESULT CPdfFile::DrawPath(const LONG& lType)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->DrawPath(m_pInternal->pAppFonts, m_pInternal->wsTempFolder, lType);
}
HRESULT CPdfFile::PathCommandStart()
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandStart();
}
HRESULT CPdfFile::PathCommandGetCurrentPoint(double* dX, double* dY)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandGetCurrentPoint(dX, dY);
}
HRESULT CPdfFile::PathCommandTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandTextCHAR(lUnicode, dX, dY, dW, dH);
}
HRESULT CPdfFile::PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandTextExCHAR(lUnicode, lGid, dX, dY, dW, dH);
}
HRESULT CPdfFile::PathCommandText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandText(wsUnicodeText, dX, dY, dW, dH);
}
HRESULT CPdfFile::PathCommandTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->PathCommandTextEx(wsUnicodeText, pGids, nGidsCount, dX, dY, dW, dH);
}
HRESULT CPdfFile::DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->DrawImage(pImage, dX, dY, dW, dH);
}
HRESULT CPdfFile::DrawImageFromFile(const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->DrawImageFromFile(m_pInternal->pAppFonts, m_pInternal->wsTempFolder, wsImagePath, dX, dY, dW, dH, nAlpha);
}
HRESULT CPdfFile::SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->SetTransform(dM11, dM12, dM21, dM22, dX, dY);
}
HRESULT CPdfFile::GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->GetTransform(dM11, dM12, dM21, dM22, dX, dY);
}
HRESULT CPdfFile::ResetTransform()
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->ResetTransform();
}
HRESULT CPdfFile::get_ClipMode(LONG* lMode)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->get_ClipMode(lMode);
}
HRESULT CPdfFile::put_ClipMode(const LONG& lMode)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;
	return m_pInternal->pWriter->put_ClipMode(lMode);
}
HRESULT CPdfFile::CommandLong(const LONG& lType, const LONG& lCommand)
{
	return m_pInternal->pWriter ? S_OK : S_FALSE;
}
HRESULT CPdfFile::CommandDouble(const LONG& lType, const double& dCommand)
{
	return m_pInternal->pWriter ? S_OK : S_FALSE;
}
HRESULT CPdfFile::CommandString(const LONG& lType, const std::wstring& sCommand)
{
	return m_pInternal->pWriter ? S_OK : S_FALSE;
}

HRESULT CPdfFile::IsSupportAdvancedCommand(const IAdvancedCommand::AdvancedCommandType& type)
{
	switch (type)
	{
	case IAdvancedCommand::AdvancedCommandType::Hyperlink:
	case IAdvancedCommand::AdvancedCommandType::Link:
	case IAdvancedCommand::AdvancedCommandType::DocInfo:
	case IAdvancedCommand::AdvancedCommandType::FormField:
	case IAdvancedCommand::AdvancedCommandType::Annotaion:
	case IAdvancedCommand::AdvancedCommandType::DeleteAnnot:
	case IAdvancedCommand::AdvancedCommandType::WidgetsInfo:
	case IAdvancedCommand::AdvancedCommandType::ShapeStart:
	case IAdvancedCommand::AdvancedCommandType::ShapeEnd:
	case IAdvancedCommand::AdvancedCommandType::PageClear:
	case IAdvancedCommand::AdvancedCommandType::PageRotate:
	case IAdvancedCommand::AdvancedCommandType::Headings:
	case IAdvancedCommand::AdvancedCommandType::Redact:
		return S_OK;
	default:
		break;
	}
	return S_FALSE;
}
HRESULT CPdfFile::AdvancedCommand(IAdvancedCommand* command)
{
	if (!m_pInternal->pWriter)
		return S_FALSE;

	switch (command->GetCommandType())
	{
	case IAdvancedCommand::AdvancedCommandType::Hyperlink:
	{
		CHyperlinkCommand* pCommand = (CHyperlinkCommand*)command;
		return m_pInternal->pWriter->AddHyperlink(pCommand->GetX(), pCommand->GetY(),
												  pCommand->GetW(), pCommand->GetH(),
												  pCommand->GetUrl(), pCommand->GetToolTip());
	}
	case IAdvancedCommand::AdvancedCommandType::Link:
	{
		CLinkCommand* pCommand = (CLinkCommand*)command;
		if (m_pInternal->pEditor && m_pInternal->pEditor->IsEditPage())
			m_pInternal->pEditor->EditPage(pCommand->GetPage(), false);
		return m_pInternal->pWriter->AddLink(pCommand->GetX(), pCommand->GetY(), pCommand->GetW(), pCommand->GetH(),
											 pCommand->GetDestX(), pCommand->GetDestY(), pCommand->GetPage());
	}
	case IAdvancedCommand::AdvancedCommandType::DocInfo:
	{
		CDocInfoCommand* pCommand = (CDocInfoCommand*)command;
		m_pInternal->pWriter->SetDocumentInfo(pCommand->GetTitle(), pCommand->GetCreator(),
											  pCommand->GetSubject(), pCommand->GetKeywords());
		return S_OK;
	}
	case IAdvancedCommand::AdvancedCommandType::FormField:
	{
		return m_pInternal->pWriter->AddFormField(m_pInternal->pAppFonts, (CFormFieldInfo*)command, m_pInternal->wsTempFolder);
	}
	case IAdvancedCommand::AdvancedCommandType::Annotaion:
	{
		CAnnotFieldInfo* pCommand = (CAnnotFieldInfo*)command;
		if (m_pInternal->pEditor && m_pInternal->pEditor->IsEditPage())
		{
			m_pInternal->pEditor->EditAnnot(pCommand->GetPage(), pCommand->GetID());
			if (pCommand->IsStamp())
			{
				int nFlags = pCommand->GetMarkupAnnotPr()->GetFlag();
				if (nFlags & (1 << 15))
					m_pInternal->pEditor->EditAnnot(pCommand->GetPage(), pCommand->GetCopyAP());
			}
		}
		return m_pInternal->pWriter->AddAnnotField(m_pInternal->pAppFonts, pCommand);
	}
	case IAdvancedCommand::AdvancedCommandType::DeleteAnnot:
	{
		CAnnotFieldDelete* pCommand = (CAnnotFieldDelete*)command;
		if (m_pInternal->pEditor && m_pInternal->pEditor->IsEditPage())
			m_pInternal->pEditor->DeleteAnnot(pCommand->GetID());
		return S_OK;
	}
	case IAdvancedCommand::AdvancedCommandType::WidgetsInfo:
	{
		CWidgetsInfo* pCommand = (CWidgetsInfo*)command;
		if (m_pInternal->pEditor && m_pInternal->pEditor->EditWidgets(pCommand))
			return m_pInternal->pWriter->EditWidgetParents(m_pInternal->pAppFonts, pCommand, m_pInternal->wsTempFolder);
		return S_OK;
	}
	case IAdvancedCommand::AdvancedCommandType::ShapeStart:
	{
		CShapeStart* pCommand = (CShapeStart*)command;
		std::vector<double> arrRedact;
		if (m_pInternal->pEditor)
		{
			m_pInternal->pEditor->AddShapeXML(pCommand->GetShapeXML());
			arrRedact = m_pInternal->pEditor->WriteRedact(pCommand->GetRedactID());
		}
		return m_pInternal->pWriter->AddRedact(arrRedact);
	}
	case IAdvancedCommand::AdvancedCommandType::ShapeEnd:
	{
		if (m_pInternal->pEditor)
			m_pInternal->pEditor->EndMarkedContent();
		return S_OK;
	}
	case IAdvancedCommand::AdvancedCommandType::PageClear:
	{
		if (m_pInternal->pEditor && m_pInternal->pEditor->IsEditPage())
			m_pInternal->pEditor->ClearPage();
		return S_OK;
	}
	case IAdvancedCommand::AdvancedCommandType::PageRotate:
	{
		CPageRotate* pCommand = (CPageRotate*)command;
		if (m_pInternal->pEditor)
			m_pInternal->pWriter->PageRotate(pCommand->GetPageRotate());
		return S_OK;
	}
	case IAdvancedCommand::AdvancedCommandType::Headings:
	{
		m_pInternal->pWriter->SetHeadings((CHeadings*)command);
		return S_OK;
	}
	case IAdvancedCommand::AdvancedCommandType::Redact:
	{
		if (m_pInternal->pEditor && m_pInternal->pEditor->IsEditPage())
			m_pInternal->pEditor->Redact((CRedact*)command);
		return S_OK;
	}
	default:
		break;
	}
	return S_FALSE;
}
