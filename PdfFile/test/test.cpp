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
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../DesktopEditor/xmlsec/src/include/CertificateCommon.h"
#include "../PdfFile.h"

void TEST(IRenderer* pRenderer)
{
    // ТЕСТОВЫЕ КОММАНДЫ
    pRenderer->PathCommandStart();
    pRenderer->PathCommandMoveTo(10, 10);
    pRenderer->PathCommandLineTo(20, 20);
    pRenderer->PathCommandCurveTo(70, 30, 30, 20, 50, 50);
    pRenderer->PathCommandClose();
    pRenderer->put_BrushColor1(0xFF0000);
    pRenderer->put_PenColor(0x0000FF);
    pRenderer->put_PenSize(1);
    pRenderer->DrawPath(c_nStroke | c_nWindingFillMode);
    pRenderer->PathCommandEnd();
}

void TEST2(IRenderer* pRenderer)
{
    ((CPdfFile*)pRenderer)->OnlineWordToPdf(NSFile::GetProcessDirectory() + L"/../example/pdf.bin", L"");
}

void TEST3(IRenderer* pRenderer)
{
    ((CPdfFile*)pRenderer)->OnlineWordToPdfFromBinary(NSFile::GetProcessDirectory() + L"/../example1/1/pdf.bin", L"");
}

int main()
{
    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;

    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    NSFonts::IApplicationFonts* pApplicationFonts = oWorker.Check();

    std::wstring wsSrcFile = NSFile::GetProcessDirectory() + L"/test.pdf";
    std::wstring wsDstFile = NSFile::GetProcessDirectory() + L"/test2.pdf";
    std::wstring wsTempDir = NSFile::GetProcessDirectory() + L"/pdftemp";

    if (!NSDirectory::Exists(wsTempDir))
        NSDirectory::CreateDirectory(wsTempDir);

    CPdfFile pdfFile(pApplicationFonts);
    pdfFile.SetTempDirectory(wsTempDir);

    std::wstring wsPassword;
    bool bResult = pdfFile.LoadFromFile(wsSrcFile);
    if (!bResult)
    {
        wsPassword = L"123456";
        bResult = pdfFile.LoadFromFile(wsSrcFile, wsPassword);
    }

    ICertificate* pCertificate = NULL;
	if (true)
    {
		std::wstring wsCertificateFile = NSFile::GetProcessDirectory() + L"/cert.pfx";
		std::wstring wsPrivateKeyFile = L"";
        std::string sCertificateFilePassword = "123456";
		std::string sPrivateFilePassword = "";

		//pCertificate = NSCertificate::FromFiles(wsPrivateKeyFile, sPrivateFilePassword, wsCertificateFile, sCertificateFilePassword);

		std::map<std::wstring, std::wstring> properties;
		properties.insert(std::make_pair(L"DNS", L"8.8.8.8"));
		//properties.insert(std::make_pair(L"IP Address", L"127.0.0.1"));
		properties.insert(std::make_pair(L"email", L"sign@onlyoffice.com"));
		//properties.insert(std::make_pair(L"phone", L"+00000000000"));
		//std::wstring sNameTest = L"NameTest";
		//std::wstring sValueTest = L"ValueTest";
		//properties.insert(std::make_pair(sNameTest, sValueTest));

		pCertificate = NSCertificate::GenerateByAlg("ecdsa512", properties);
		//pCertificate = NSCertificate::GenerateByAlg("rsa2048", properties);
    }

	if (false)
    {
        pdfFile.CreatePdf();
        pdfFile.OnlineWordToPdfFromBinary(NSFile::GetProcessDirectory() + L"/pdf.bin", wsDstFile);

        RELEASEINTERFACE(pApplicationFonts);
        RELEASEOBJECT(pCertificate);
        return 0;
    }

	if (false)
    {
        double dPageDpiX, dPageDpiY, dWidth, dHeight;
        int i = 0;
        for (i = 0; i < pdfFile.GetPagesCount(); i++)
        {
            pdfFile.GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
            pdfFile.ConvertToRaster(i, NSFile::GetProcessDirectory() + L"/res" + std::to_wstring(i) + L".png", 4, dWidth * dPageDpiX / 25.4, dHeight * dPageDpiY / 25.4, true, pdfFile.GetFontManager());
        }

        if (pCertificate)
        {
            BYTE* pWidgets = pdfFile.VerifySign(wsSrcFile, pCertificate);
            RELEASEARRAYOBJECTS(pWidgets);
        }

        RELEASEINTERFACE(pApplicationFonts);
        RELEASEOBJECT(pCertificate);
        return 0;
    }

	if (false)
    {
        pdfFile.CreatePdf(true);
        double dPageDpiX, dPageDpiY, dWidth, dHeight;
        pdfFile.GetPageInfo(0, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

        dWidth  *= 25.4 / dPageDpiX;
        dHeight *= 25.4 / dPageDpiY;

        pdfFile.NewPage();
        pdfFile.BeginCommand(c_nPageType);
        pdfFile.put_Width(dWidth);
        pdfFile.put_Height(dHeight);
        std::string sTitle = "1<2<3<4";
        pdfFile.DocInfo(UTF8_TO_U(sTitle), L"5\"6\";7\'8\'", L"1>2>3>4", L"1&2&3&4&5");
        //pdfFile.DrawImageFromFile(NSFile::GetProcessDirectory() + L"/test.jpg", 10, 10, 455, 200);
        pdfFile.EndCommand(c_nPageType);

        if (pCertificate)
			pdfFile.Sign(10, 70, 50, 50, NSFile::GetProcessDirectory() + L"/test.png", pCertificate);
        int nRes = pdfFile.SaveToFile(wsDstFile);

        RELEASEINTERFACE(pApplicationFonts);
        RELEASEOBJECT(pCertificate);
        return 0;
    }

    if (bResult && pdfFile.EditPdf(wsDstFile))
    {
        if (pCertificate)
		{
			if (pdfFile.EditPage(0))
			{
				TEST(&pdfFile);
				pdfFile.Sign(10, 120, 50, 50, NSFile::GetProcessDirectory() + L"/test.png", pCertificate);
			}
        }
        else
        {
            if (pdfFile.EditPage(0))
            {
                TEST(&pdfFile);
                pdfFile.RotatePage(90);
            }

            pdfFile.DeletePage(1);

            if (pdfFile.EditPage(1))
            {
                TEST2(&pdfFile);
            }

            if (pdfFile.AddPage(3))
            {
                TEST3(&pdfFile);
            }
        }

        pdfFile.Close();
    }

    RELEASEINTERFACE(pApplicationFonts);
    RELEASEOBJECT(pCertificate);
    return 0;
}
