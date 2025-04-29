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

#include "gtest/gtest.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../DesktopEditor/xmlsec/src/include/CertificateCommon.h"
#include "../../DesktopEditor/graphics/MetafileToGraphicsRenderer.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DjVuFile/DjVu.h"
#include "../PdfFile.h"

class CPdfFileTest : public testing::Test
{
protected:
	static CApplicationFontsWorker* oWorker;
	static NSFonts::IApplicationFonts* pApplicationFonts;
	static std::wstring wsTempDir;
	static std::wstring wsSrcFile;
	static std::wstring wsDstFile;

	static std::wstring strDirIn;
	static std::wstring strDirOut;
	static std::wstring strDiffs;

public:
	CPdfFile* pdfFile;

public:
	static void SetUpTestSuite()
	{
		oWorker = new CApplicationFontsWorker();

		oWorker->m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
		oWorker->m_bIsNeedThumbnails = false;

		if (!NSDirectory::Exists(oWorker->m_sDirectory))
			NSDirectory::CreateDirectory(oWorker->m_sDirectory);

		pApplicationFonts = oWorker->Check();

		wsTempDir = NSFile::GetProcessDirectory() + L"/pdftemp";
		wsSrcFile = NSFile::GetProcessDirectory() + L"/test.pdf";
		wsDstFile = NSFile::GetProcessDirectory() + L"/test2.pdf";

		strDirIn  = NSFile::GetProcessDirectory() + L"/resI";
		strDirOut = NSFile::GetProcessDirectory() + L"/resO";
		strDiffs  = NSFile::GetProcessDirectory() + L"/resD";

		if (NSDirectory::Exists(wsTempDir))
			NSDirectory::DeleteDirectory(wsTempDir);
		NSDirectory::CreateDirectory(wsTempDir);

		if (!NSDirectory::Exists(strDirOut))
			NSDirectory::CreateDirectory(strDirOut);
	}
	static void TearDownTestSuite()
	{
		RELEASEINTERFACE(pApplicationFonts);
		RELEASEOBJECT(oWorker);
	}

	void LoadFromFile(const std::wstring& _wsSrcFile = L"")
	{
		bool bResult = pdfFile->LoadFromFile(_wsSrcFile.empty() ? wsSrcFile : _wsSrcFile);
		if (!bResult)
		{
			std::wstring wsPassword = L"123456";
			bResult = pdfFile->LoadFromFile(_wsSrcFile.empty() ? wsSrcFile : _wsSrcFile, L"", wsPassword, wsPassword);
		}

		ASSERT_TRUE(bResult);
	}
	void DrawSmth()
	{
		pdfFile->PathCommandStart();
		pdfFile->PathCommandMoveTo(10, 10);
		pdfFile->PathCommandLineTo(20, 20);
		pdfFile->PathCommandCurveTo(70, 30, 30, 20, 50, 50);
		pdfFile->PathCommandClose();
		pdfFile->put_BrushColor1(0xFF0000);
		pdfFile->put_PenColor(0x0000FF);
		pdfFile->put_PenSize(1);
		pdfFile->DrawPath(c_nStroke | c_nWindingFillMode);
		pdfFile->PathCommandEnd();
	}
	ICertificate* GetCertificate()
	{
		std::wstring wsCertificateFile = NSFile::GetProcessDirectory() + L"/cert.pfx";
		std::wstring wsPrivateKeyFile  = L"";
		std::string sCertificateFilePassword = "123456";
		std::string sPrivateFilePassword = "";

		return NSCertificate::FromFiles(wsPrivateKeyFile, sPrivateFilePassword, wsCertificateFile, sCertificateFilePassword);
	}
	ICertificate* GenerateCertificateECDSA512()
	{
		std::map<std::wstring, std::wstring> properties;
		properties.insert(std::make_pair(L"DNS", L"8.8.8.8"));
		properties.insert(std::make_pair(L"email", L"sign@onlyoffice.com"));

		return NSCertificate::GenerateByAlg("ecdsa512", properties);
	}
	ICertificate* GenerateCertificateRSA2048()
	{
		std::map<std::wstring, std::wstring> properties;
		properties.insert(std::make_pair(L"DNS", L"8.8.8.8"));
		properties.insert(std::make_pair(L"email", L"sign@onlyoffice.com"));

		return NSCertificate::GenerateByAlg("rsa2048", properties);
	}

	virtual void SetUp() override
	{
		pdfFile = new CPdfFile(pApplicationFonts);
		pdfFile->SetTempDirectory(wsTempDir);
	}
	virtual void TearDown() override
	{
		RELEASEOBJECT(pdfFile);
	}
};

CApplicationFontsWorker* CPdfFileTest::oWorker = NULL;
NSFonts::IApplicationFonts* CPdfFileTest::pApplicationFonts = NULL;
std::wstring CPdfFileTest::wsTempDir;
std::wstring CPdfFileTest::wsSrcFile;
std::wstring CPdfFileTest::wsDstFile;
std::wstring CPdfFileTest::strDirIn;
std::wstring CPdfFileTest::strDirOut;
std::wstring CPdfFileTest::strDiffs;

TEST_F(CPdfFileTest, DjVuToPdf)
{
	GTEST_SKIP();

	CDjVuFile* pDjVu = new CDjVuFile(pApplicationFonts);
	pDjVu->LoadFromFile(NSFile::GetProcessDirectory() + L"/test.djvu");
	pDjVu->ConvertToPdf(wsDstFile);

	RELEASEOBJECT(pDjVu);
}

TEST_F(CPdfFileTest, GetMetaData)
{
	GTEST_SKIP();

	BYTE* pMetaData = NULL;
	DWORD nMetaLength = 0;

	if (pdfFile->GetMetaData(wsSrcFile, L"ONLYOFFICEFORM", &pMetaData, nMetaLength))
	{
		NSFile::CFileBinary oFile;
		if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/ONLYOFFICEFORM.docxf"))
			oFile.WriteFile(pMetaData, nMetaLength);
		oFile.CloseFile();

		EXPECT_TRUE(pMetaData);
	}
	RELEASEARRAYOBJECTS(pMetaData);
}

TEST_F(CPdfFileTest, PdfType)
{
	GTEST_SKIP();

	NSFile::CFileBinary oFile;
	ASSERT_TRUE(oFile.OpenFile(wsSrcFile));

	int nSize = 4096;
	BYTE* pBuffer = new BYTE[nSize];
	if (!pBuffer)
	{
		oFile.CloseFile();
		return;
	}

	DWORD dwReadBytes = 0;
	oFile.ReadFile(pBuffer, nSize, dwReadBytes);
	oFile.CloseFile();

	char* pData = (char*)pBuffer;
	bool bPDF = false;
	for (int i = 0; i < nSize - 5; ++i)
	{
		int nPDF = strncmp(&pData[i], "%PDF-", 5);
		if (!nPDF)
		{
			bPDF = true;
			break;
		}
	}

	ASSERT_TRUE(bPDF);

	RELEASEARRAYOBJECTS(pBuffer);
}

TEST_F(CPdfFileTest, GetEmbeddedFont)
{
	GTEST_SKIP();

	LoadFromFile();

	std::wcout << pdfFile->GetEmbeddedFontPath(L"Symbol") << std::endl;
}

TEST_F(CPdfFileTest, ValidMetaData)
{
	GTEST_SKIP();

	LoadFromFile();

	std::cout << "ValidMetaData " << (pdfFile->ValidMetaData() ? "true" : "false") << std::endl;
}

TEST_F(CPdfFileTest, PdfBinToPng)
{
	GTEST_SKIP();

	NSFile::CFileBinary oFile;
	ASSERT_TRUE(oFile.OpenFile(NSFile::GetProcessDirectory() + L"/pdf.bin"));

	DWORD dwFileSize = oFile.GetFileSize();
	BYTE* pFileContent = new BYTE[dwFileSize];
	if (!pFileContent)
	{
		oFile.CloseFile();
		FAIL();
	}

	DWORD dwReaded = 0;
	EXPECT_TRUE(oFile.ReadFile(pFileContent, dwFileSize, dwReaded));
	oFile.CloseFile();

	NSOnlineOfficeBinToPdf::CMetafileToRenderterRaster imageWriter(NULL);
	imageWriter.SetApplication(pApplicationFonts);
	imageWriter.SetRasterFormat(4);
	imageWriter.SetFileName(NSFile::GetProcessDirectory() + L"/resPdfBinToPng.png");

	imageWriter.ConvertBuffer(pFileContent, dwFileSize);
}

TEST_F(CPdfFileTest, PdfFromBin)
{
	GTEST_SKIP();

	pdfFile->CreatePdf();
	EXPECT_HRESULT_SUCCEEDED(pdfFile->OnlineWordToPdfFromBinary(NSFile::GetProcessDirectory() + L"/pdf.bin", wsDstFile));
}

TEST_F(CPdfFileTest, PdfFromBase64)
{
	GTEST_SKIP();

	pdfFile->CreatePdf();
	EXPECT_HRESULT_SUCCEEDED(pdfFile->OnlineWordToPdf(NSFile::GetProcessDirectory() + L"/base64.txt", wsDstFile));
}

TEST_F(CPdfFileTest, PdfToPdf)
{
	GTEST_SKIP();

	LoadFromFile();
	pdfFile->CreatePdf();

	for (int i = 0; i < pdfFile->GetPagesCount(); i++)
	{
		pdfFile->NewPage();
		pdfFile->DrawPageOnRenderer(pdfFile, i, NULL);
	}

	pdfFile->SaveToFile(wsDstFile);
}

TEST_F(CPdfFileTest, SetMetaData)
{
	GTEST_SKIP();

	pdfFile->CreatePdf();

	BYTE* pFileData = NULL;
	DWORD nFileSize;
	std::wstring sFile = NSFile::GetProcessDirectory() + L"/ONLYOFFICEFORM.docxf";
	EXPECT_TRUE(NSFile::CFileBinary::ReadAllBytes(sFile, &pFileData, nFileSize));
	pdfFile->AddMetaData(L"ONLYOFFICEFORM", pFileData, nFileSize);
	RELEASEARRAYOBJECTS(pFileData);

	EXPECT_HRESULT_SUCCEEDED(pdfFile->OnlineWordToPdfFromBinary(NSFile::GetProcessDirectory() + L"/pdf.bin", wsDstFile));
}

TEST_F(CPdfFileTest, ConvertToRaster)
{
	GTEST_SKIP();

	LoadFromFile();

	double dPageDpiX, dPageDpiY, dWidth, dHeight;
	int i = 0;
	//for (i = 0; i < pdfFile->GetPagesCount(); i++)
	{
		pdfFile->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
		pdfFile->ConvertToRaster(i, NSFile::GetProcessDirectory() + L"/resO/res" + std::to_wstring(i) + L".png", 4, dWidth, dHeight, true, pdfFile->GetFontManager());
	}
}

TEST_F(CPdfFileTest, VerifySign)
{
	GTEST_SKIP();

	LoadFromFile();

	ICertificate* pCertificate = GetCertificate();
	ASSERT_TRUE(pCertificate);

	BYTE* pWidgets = pdfFile->VerifySign(wsSrcFile, pCertificate);
	EXPECT_TRUE(pWidgets);

	RELEASEARRAYOBJECTS(pWidgets);
	RELEASEOBJECT(pCertificate);
}

TEST_F(CPdfFileTest, SplitPdf)
{
	//GTEST_SKIP();

	LoadFromFile();

	int nBufferLen = NULL;
	BYTE* pBuffer = NULL;
	NSFile::CFileBinary oFile;
	if (oFile.OpenFile(NSFile::GetProcessDirectory() + L"/base64.txt"))
	{
		DWORD dwFileSize = oFile.GetFileSize();
		BYTE* pFileContent = new BYTE[dwFileSize];
		if (!pFileContent)
		{
			oFile.CloseFile();
			FAIL();
		}

		DWORD dwReaded;
		EXPECT_TRUE(oFile.ReadFile(pFileContent, dwFileSize, dwReaded));

		nBufferLen = NSBase64::Base64DecodeGetRequiredLength(dwFileSize);
		pBuffer = new BYTE[nBufferLen];
		if (!pBuffer)
		{
			RELEASEARRAYOBJECTS(pFileContent);
			FAIL();
		}

		EXPECT_TRUE(NSBase64::Base64Decode((const char*)pFileContent, dwFileSize, pBuffer, &nBufferLen));
	}
	oFile.CloseFile();

	std::vector<int> arrPages = { 1 };
	BYTE* pFile = pdfFile->SplitPages(arrPages.data(), arrPages.size(), pBuffer, nBufferLen);
	ASSERT_TRUE(pFile != NULL);

	std::wstring wsSplitFile = NSFile::GetProcessDirectory() + L"/test_split.pdf";
	if (oFile.CreateFileW(wsSplitFile))
	{
		int nLength = pFile[0] | pFile[1] << 8 | pFile[2] << 16 | pFile[3] << 24;
		oFile.WriteFile(pFile + 4, nLength - 4);
	}
	oFile.CloseFile();

	RELEASEARRAYOBJECTS(pFile);
	RELEASEARRAYOBJECTS(pBuffer);
}

TEST_F(CPdfFileTest, MergePdf)
{
	GTEST_SKIP();

	LoadFromFile();

	ASSERT_TRUE(pdfFile->EditPdf(wsDstFile));

	std::wstring wsSplitFile = NSFile::GetProcessDirectory() + L"/test_split.pdf";
	pdfFile->MergePages(wsSplitFile, 0, L"");

	pdfFile->Close();
}

TEST_F(CPdfFileTest, EditPdf)
{
	GTEST_SKIP();

	LoadFromFile();
	ASSERT_TRUE(pdfFile->EditPdf(wsDstFile));

	EXPECT_TRUE(pdfFile->EditPage(0));
	{
		DrawSmth();
		pdfFile->RotatePage(90);
	}

	pdfFile->DeletePage(1);

	pdfFile->AddPage(2);

	pdfFile->Close();
}

TEST_F(CPdfFileTest, EditPdfFromBase64)
{
	GTEST_SKIP();

	NSFonts::NSApplicationFontStream::SetGlobalMemoryStorage(NSFonts::NSApplicationFontStream::CreateDefaultGlobalMemoryStorage());

	LoadFromFile();
	ASSERT_TRUE(pdfFile->EditPdf(wsDstFile));

	// чтение и конвертации бинарника
	NSFile::CFileBinary oFile;
	ASSERT_TRUE(oFile.OpenFile(NSFile::GetProcessDirectory() + L"/base64.txt"));

	DWORD dwFileSize = oFile.GetFileSize();
	BYTE* pFileContent = new BYTE[dwFileSize];
	if (!pFileContent)
	{
		oFile.CloseFile();
		FAIL();
	}

	DWORD dwReaded;
	EXPECT_TRUE(oFile.ReadFile(pFileContent, dwFileSize, dwReaded));
	oFile.CloseFile();

	int nBufferLen = NSBase64::Base64DecodeGetRequiredLength(dwFileSize);
	BYTE* pBuffer = new BYTE[nBufferLen];
	if (!pBuffer)
	{
		RELEASEARRAYOBJECTS(pFileContent);
		FAIL();
	}

	EXPECT_TRUE(NSBase64::Base64Decode((const char*)pFileContent, dwFileSize, pBuffer, &nBufferLen));
	CConvertFromBinParams* pParams = new CConvertFromBinParams();
	pParams->m_sMediaDirectory = NSFile::GetProcessDirectory();
	pdfFile->AddToPdfFromBinary(pBuffer + 4, nBufferLen - 4, pParams);

	RELEASEOBJECT(pParams);
	RELEASEARRAYOBJECTS(pBuffer);
	RELEASEARRAYOBJECTS(pFileContent);

	pdfFile->Close();
}

TEST_F(CPdfFileTest, EditPdfFromBin)
{
	GTEST_SKIP();

	LoadFromFile();
	ASSERT_TRUE(pdfFile->EditPdf(wsDstFile));

	// чтение бинарника
	NSFile::CFileBinary oFile;
	ASSERT_TRUE(oFile.OpenFile(NSFile::GetProcessDirectory() + L"/changes.bin"));

	DWORD dwFileSize = oFile.GetFileSize();
	BYTE* pFileContent = new BYTE[dwFileSize];
	if (!pFileContent)
	{
		oFile.CloseFile();
		FAIL();
	}

	DWORD dwReaded;
	EXPECT_TRUE(oFile.ReadFile(pFileContent, dwFileSize, dwReaded));
	oFile.CloseFile();

	CConvertFromBinParams* pParams = new CConvertFromBinParams();
	pParams->m_sMediaDirectory = NSFile::GetProcessDirectory();
	pdfFile->AddToPdfFromBinary(pFileContent + 4, dwReaded - 4, pParams);

	RELEASEOBJECT(pParams);
	RELEASEARRAYOBJECTS(pFileContent);

	pdfFile->Close();
}

TEST_F(CPdfFileTest, EditPdfSign)
{
	GTEST_SKIP();

	LoadFromFile();
	ASSERT_TRUE(pdfFile->EditPdf(wsDstFile));

	ICertificate* pCertificate = GetCertificate();
	ASSERT_TRUE(pCertificate);

	EXPECT_TRUE(pdfFile->EditPage(0));
	{
		pdfFile->Sign(10, 10, 100, 100, NSFile::GetProcessDirectory() + L"/test.jpeg", pCertificate);
	}

	pdfFile->Close();

	RELEASEOBJECT(pCertificate);
}

TEST_F(CPdfFileTest, ChangePasswordToEmpty)
{
	GTEST_SKIP();

	LoadFromFile();
	EXPECT_HRESULT_SUCCEEDED(pdfFile->ChangePassword(wsDstFile));
}

TEST_F(CPdfFileTest, ChangePasswordToPassword)
{
	GTEST_SKIP();

	LoadFromFile();
	EXPECT_HRESULT_SUCCEEDED(pdfFile->ChangePassword(wsDstFile, L"123456"));
}

TEST_F(CPdfFileTest, ImgDiff)
{
	GTEST_SKIP();

	if (NSDirectory::Exists(strDirIn))
		NSDirectory::DeleteDirectory(strDirIn);
	NSDirectory::CreateDirectory(strDirIn);
	if (NSDirectory::Exists(strDirOut))
		NSDirectory::DeleteDirectory(strDirOut);
	NSDirectory::CreateDirectory(strDirOut);
	if (NSDirectory::Exists(strDiffs))
		NSDirectory::DeleteDirectory(strDiffs);
	NSDirectory::CreateDirectory(strDiffs);

	LoadFromFile();

	int nCountInPages = pdfFile->GetPagesCount();
	for (int nPage = 0; nPage < nCountInPages; ++nPage)
	{
		std::wstring sPageI = strDirIn + L"/res" + std::to_wstring(nPage) + L".png";
		std::wstring sPageO = strDirOut + L"/res" + std::to_wstring(nPage) + L".png";
		std::wstring sPageDiff = strDiffs + L"/res" + std::to_wstring(nPage) + L".png";

		CBgraFrame frameI;
		frameI.OpenFile(sPageI);

		CBgraFrame frameO;
		frameO.OpenFile(sPageO);

		int nW_I = frameI.get_Width();
		int nH_I = frameI.get_Height();

		int nW_O = frameO.get_Width();
		int nH_O = frameO.get_Height();

		if (nW_I != nW_O || nH_I != nH_O)
		{
			if (!NSDirectory::Exists(strDiffs))
				NSDirectory::CreateDirectories(CorrectPathW(strDiffs));

			std::wstring sFilePagesDiff = sPageDiff;
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(sPageDiff);
			oFile.WriteStringUTF8(L"sizes!");
			oFile.CloseFile();
			continue;
		}

		BYTE* pDataI = frameI.get_Data();
		BYTE* pDataO = frameO.get_Data();
		size_t sizeMemory = 4 * nW_I * nH_I;

		if (0 == memcmp(pDataI, pDataO, sizeMemory))
			continue;

		int nEpsilonEps = 3;
		int nEpsilonNatural = 5;

		int nDivExist = 0;
		for (int indexPixH = 0; indexPixH < nH_I; indexPixH++)
		{
			for (int indexPixW = 0; indexPixW < nW_I; indexPixW++)
			{
				if (pDataI[0] != pDataO[0] || pDataI[1] != pDataO[1] || pDataI[2] != pDataO[2])
				{
					// test epsilon natural
					if ((abs(pDataI[0] - pDataO[0]) < nEpsilonNatural) &&
							(abs(pDataI[1] - pDataO[1]) < nEpsilonNatural) &&
							(abs(pDataI[2] - pDataO[2]) < nEpsilonNatural))
					{
						pDataI += 4;
						pDataO += 4;
						continue;
					}

					// test epsilon left, right, top, bottom
					int nEpsUp = nEpsilonEps;
					if (indexPixH > 0)
					{
						BYTE* pByteI = frameI.get_Data() + 4 * (indexPixH - 1) * nW_I + 4 * indexPixW;

						if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
								(abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
								(abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
						{
							nEpsUp = nEpsilonEps - 1;
						}
					}

					int nEpsDown = nEpsilonEps;
					if (indexPixH < (nH_I - 1))
					{
						BYTE* pByteI = frameI.get_Data() + 4 * (indexPixH + 1) * nW_I + 4 * indexPixW;

						if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
								(abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
								(abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
						{
							nEpsDown = nEpsilonEps - 1;
						}
					}

					int nEpsLeft = nEpsilonEps;
					if (indexPixW > 0)
					{
						BYTE* pByteI = pDataI - 4;

						if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
								(abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
								(abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
						{
							nEpsLeft = nEpsilonEps - 1;
						}
					}

					int nEpsRight = nEpsilonEps;
					if (indexPixW < (nW_I - 1))
					{
						BYTE* pByteI = pDataI + 4;

						if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
								(abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
								(abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
						{
							nEpsRight = nEpsilonEps - 1;
						}
					}

					if ((nEpsLeft < nEpsilonEps) ||
							(nEpsRight < nEpsilonEps) ||
							(nEpsUp < nEpsilonEps) ||
							(nEpsDown < nEpsilonEps))
					{
						pDataI += 4;
						pDataO += 4;
						continue;
					}

					++nDivExist;

					if (pDataO[0] == 0x00 && pDataO[1] == 0x00 && pDataO[2] == 0xFF)
					{
						pDataO[0] = 0xFF;
						pDataO[1] = 0x00;
						pDataO[2] = 0x00;
					}
					else
					{
						pDataO[0] = 0x00;
						pDataO[1] = 0x00;
						pDataO[2] = 0xFF;
					}
				}
				pDataI += 4;
				pDataO += 4;
			}
		}

		if (nDivExist > 7)
		{
			if (!NSDirectory::Exists(strDiffs))
				NSDirectory::CreateDirectories(CorrectPathW(strDiffs));

			CBgraFrame frameOSrc;
			frameOSrc.OpenFile(sPageO);

			BYTE* pData1 = frameI.get_Data();
			BYTE* pData2 = frameOSrc.get_Data();
			BYTE* pData3 = frameO.get_Data();

			int nRowW = 4 * nW_I;
			BYTE* pDataAll = new BYTE[3 * nRowW * nH_I];
			BYTE* pDataAllSrc = pDataAll;
			for (int j = 0; j < nH_I; j++)
			{
				memcpy(pDataAll, pData1, nRowW);
				pDataAll += nRowW;
				pData1 += nRowW;

				memcpy(pDataAll, pData2, nRowW);
				pDataAll += nRowW;
				pData2 += nRowW;

				memcpy(pDataAll, pData3, nRowW);
				pDataAll += nRowW;
				pData3 += nRowW;
			}

			CBgraFrame oFrameAll;
			oFrameAll.put_Data(pDataAllSrc);
			oFrameAll.put_Width(3 * nW_I);
			oFrameAll.put_Height(nH_I);
			oFrameAll.put_Stride(-3 * nRowW);
			oFrameAll.SaveFile(sPageDiff, 4);
		}
	}
}
