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
#include "../PdfFile.h"

class CPdfFileTest : public testing::Test
{
protected:
	static CApplicationFontsWorker* oWorker;
	static NSFonts::IApplicationFonts* pApplicationFonts;
	static std::wstring wsTempDir;

public:
	CPdfFile* pdfFile;
	std::wstring wsSrcFile;
	std::wstring wsDstFile;

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

		if (!NSDirectory::Exists(wsTempDir))
			NSDirectory::CreateDirectory(wsTempDir);
	}
	static void TearDownTestSuite()
	{
		RELEASEINTERFACE(pApplicationFonts);
		RELEASEOBJECT(oWorker);
	}

	void LoadFromFile()
	{
		bool bResult = pdfFile->LoadFromFile(wsSrcFile);
		if (!bResult)
		{
			std::wstring wsPassword = L"123456";
			bResult = pdfFile->LoadFromFile(wsSrcFile, L"", wsPassword, wsPassword);
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
		wsSrcFile = NSFile::GetProcessDirectory() + L"/test.pdf";
		wsDstFile = NSFile::GetProcessDirectory() + L"/test2.pdf";

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

TEST_F(CPdfFileTest, GetMetaData)
{
	GTEST_SKIP();

	BYTE* pMetaData = NULL;
	DWORD nMetaLength = 0;

	if (pdfFile->GetMetaData(wsSrcFile, L"Test0", &pMetaData, nMetaLength))
	{
		NSFile::CFileBinary oFile;
		if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/resGetMetaData0.png"))
			oFile.WriteFile(pMetaData, nMetaLength);
		oFile.CloseFile();

		EXPECT_TRUE(pMetaData);
	}
	RELEASEARRAYOBJECTS(pMetaData);

	if (pdfFile->GetMetaData(wsSrcFile, L"Test1", &pMetaData, nMetaLength))
	{
		NSFile::CFileBinary oFile;
		if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/resGetMetaData1.png"))
			oFile.WriteFile(pMetaData, nMetaLength);
		oFile.CloseFile();

		EXPECT_TRUE(pMetaData);
	}
	RELEASEARRAYOBJECTS(pMetaData);
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

TEST_F(CPdfFileTest, SetMetaData)
{
	GTEST_SKIP();

	pdfFile->CreatePdf();

	BYTE* pFileData = NULL;
	DWORD nFileSize;
	std::wstring sFile = NSFile::GetProcessDirectory() + L"/res0.png";
	EXPECT_TRUE(NSFile::CFileBinary::ReadAllBytes(sFile, &pFileData, nFileSize));
	pdfFile->AddMetaData(L"Test0", pFileData, nFileSize);
	RELEASEARRAYOBJECTS(pFileData);

	sFile = NSFile::GetProcessDirectory() + L"/res1.png";
	EXPECT_TRUE(NSFile::CFileBinary::ReadAllBytes(sFile, &pFileData, nFileSize));
	pdfFile->AddMetaData(L"Test1", pFileData, nFileSize);
	RELEASEARRAYOBJECTS(pFileData);

	EXPECT_HRESULT_SUCCEEDED(pdfFile->OnlineWordToPdfFromBinary(NSFile::GetProcessDirectory() + L"/pdf.bin", wsDstFile));
}

TEST_F(CPdfFileTest, ConvertToRaster)
{
	GTEST_SKIP();

	LoadFromFile();

	double dPageDpiX, dPageDpiY, dWidth, dHeight;
	int i = 0;
	for (i = 0; i < pdfFile->GetPagesCount(); i++)
	{
		pdfFile->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
		pdfFile->ConvertToRaster(i, NSFile::GetProcessDirectory() + L"/res" + std::to_wstring(i) + L".png", 4, dWidth, dHeight, true, pdfFile->GetFontManager());
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
	//GTEST_SKIP();

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
	pdfFile->AddToPdfFromBinary(pBuffer, nBufferLen, NULL);

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
	ASSERT_TRUE(oFile.OpenFile(NSFile::GetProcessDirectory() + L"/changes0.json"));

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

	pdfFile->AddToPdfFromBinary(pFileContent, dwReaded, NULL);

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
