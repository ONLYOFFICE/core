/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "gtest/gtest.h"
#include "../PdfFile.h"
#include "../../DesktopEditor/common/File.h"

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

namespace
{
std::string CreatePdf(const std::string& catalog, const std::string& form,
	const std::string& trailer = "",
	const std::string& xfa = "<template xmlns=\"http://www.xfa.org/schema/xfa-template/3.3/\"/>")
{
	const std::vector<std::string> objects = {
		"<< /Type /Catalog /Pages 2 0 R " + catalog + " >>",
		"<< /Type /Pages /Count 1 /Kids [3 0 R] >>",
		"<< /Type /Page /Parent 2 0 R /MediaBox [0 0 100 100] /Resources << >> >>",
		"<< /Fields [] " + form + " >>",
		"<< /Length " + std::to_string(xfa.size()) + " >>\nstream\n" + xfa + "\nendstream"
	};

	std::ostringstream pdf;
	pdf << "%PDF-1.7\n";
	std::vector<std::streamoff> offsets;
	for (size_t i = 0; i < objects.size(); ++i)
	{
		offsets.push_back(pdf.tellp());
		pdf << i + 1 << " 0 obj\n" << objects[i] << "\nendobj\n";
	}

	const std::streamoff xref = pdf.tellp();
	pdf << "xref\n0 " << objects.size() + 1 << "\n0000000000 65535 f \n";
	for (std::streamoff offset : offsets)
		pdf << std::setfill('0') << std::setw(10) << offset << " 00000 n \n";
	pdf << "trailer\n<< /Size " << objects.size() + 1 << " /Root 1 0 R " << trailer << " >>\n";
	pdf << "startxref\n" << xref << "\n%%EOF\n";
	return pdf.str();
}

class CTemporaryPdf
{
public:
	std::wstring path;

	CTemporaryPdf()
		: path(NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"pdf_xfa_\u03b1_"))
	{
	}

	~CTemporaryPdf()
	{
		NSFile::CFileBinary::Remove(path);
	}

	bool Write(const std::string& data)
	{
		NSFile::CFileBinary file;
		return file.CreateFileW(path) && file.WriteFile(data.data(), static_cast<DWORD>(data.size()));
	}
};
}

class CPdfXfaTest : public testing::Test
{
protected:
	NSFonts::IApplicationFonts* pApplicationFonts;
	CPdfFile* pPdfFile;

	void SetUp() override
	{
		pApplicationFonts = NSFonts::NSApplication::Create();
		pPdfFile = new CPdfFile(pApplicationFonts);
	}

	void TearDown() override
	{
		delete pPdfFile;
		pApplicationFonts->Release();
	}

	bool Load(const std::string& catalog, const std::string& form)
	{
		std::string pdf = CreatePdf(catalog, form);
		return pPdfFile->LoadFromMemory(reinterpret_cast<BYTE*>(&pdf[0]), static_cast<DWORD>(pdf.size()));
	}
};

TEST_F(CPdfXfaTest, Stream)
{
	ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering true", "/XFA 5 0 R"));
	EXPECT_TRUE(pPdfFile->IsDynamicXfa());
}

TEST_F(CPdfXfaTest, PacketArray)
{
	ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering true", "/XFA [(template) 5 0 R]"));
	EXPECT_TRUE(pPdfFile->IsDynamicXfa());
}

TEST_F(CPdfXfaTest, DirectAcroForm)
{
	ASSERT_TRUE(Load("/AcroForm << /Fields [] /XFA 5 0 R >> /NeedsRendering true", ""));
	EXPECT_TRUE(pPdfFile->IsDynamicXfa());
}

TEST_F(CPdfXfaTest, StaticXfa)
{
	ASSERT_TRUE(Load("/AcroForm 4 0 R", "/XFA 5 0 R"));
	EXPECT_FALSE(pPdfFile->IsDynamicXfa());
	ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering false", "/XFA [(template) 5 0 R]"));
	EXPECT_FALSE(pPdfFile->IsDynamicXfa());
}

TEST_F(CPdfXfaTest, InvalidNeedsRendering)
{
	for (const std::string& value : {"1", "(true)", "/true", "null"})
	{
		SCOPED_TRACE(value);
		ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering " + value, "/XFA 5 0 R"));
		EXPECT_FALSE(pPdfFile->IsDynamicXfa());
	}
}

TEST_F(CPdfXfaTest, MissingXfa)
{
	ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering true", ""));
	EXPECT_FALSE(pPdfFile->IsDynamicXfa());
}

TEST_F(CPdfXfaTest, InvalidXfa)
{
	for (const std::string& value : {"[]", "null", "true", "1", "(template)", "<< >>"})
	{
		SCOPED_TRACE(value);
		ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering true", "/XFA " + value));
		EXPECT_FALSE(pPdfFile->IsDynamicXfa());
	}
}

TEST_F(CPdfXfaTest, MissingAcroForm)
{
	ASSERT_TRUE(Load("/NeedsRendering true", "/XFA 5 0 R"));
	EXPECT_FALSE(pPdfFile->IsDynamicXfa());
}

TEST_F(CPdfXfaTest, InvalidAcroForm)
{
	for (const std::string& value : {"null", "true", "1", "(form)", "[]"})
	{
		SCOPED_TRACE(value);
		ASSERT_TRUE(Load("/NeedsRendering true /AcroForm " + value, "/XFA 5 0 R"));
		EXPECT_FALSE(pPdfFile->IsDynamicXfa());
	}
}

TEST_F(CPdfXfaTest, OrdinaryPdf)
{
	ASSERT_TRUE(Load("", ""));
	EXPECT_FALSE(pPdfFile->IsDynamicXfa());
}

TEST_F(CPdfXfaTest, DocumentInfo)
{
	ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering true", "/XFA 5 0 R"));
	EXPECT_NE(std::wstring::npos, pPdfFile->GetInfo().find(L"\"DynamicXfa\":true"));
	ASSERT_TRUE(Load("/AcroForm 4 0 R", "/XFA 5 0 R"));
	EXPECT_NE(std::wstring::npos, pPdfFile->GetInfo().find(L"\"DynamicXfa\":false"));
	ASSERT_TRUE(Load("", ""));
	EXPECT_NE(std::wstring::npos, pPdfFile->GetInfo().find(L"\"DynamicXfa\":false"));
}

TEST_F(CPdfXfaTest, UnloadedAndClosed)
{
	EXPECT_FALSE(pPdfFile->IsDynamicXfa());
	ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering true", "/XFA 5 0 R"));
	ASSERT_TRUE(pPdfFile->IsDynamicXfa());
	pPdfFile->Close();
	EXPECT_FALSE(pPdfFile->IsDynamicXfa());
}

TEST_F(CPdfXfaTest, FailedLoadClearsState)
{
	ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering true", "/XFA 5 0 R"));
	ASSERT_TRUE(pPdfFile->IsDynamicXfa());
	BYTE invalid[] = "not a PDF";
	ASSERT_FALSE(pPdfFile->LoadFromMemory(invalid, sizeof(invalid) - 1));
	EXPECT_FALSE(pPdfFile->IsDynamicXfa());
}

TEST_F(CPdfXfaTest, ReloadClearsState)
{
	ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering true", "/XFA 5 0 R"));
	ASSERT_TRUE(pPdfFile->IsDynamicXfa());
	ASSERT_TRUE(Load("", ""));
	EXPECT_FALSE(pPdfFile->IsDynamicXfa());
}

TEST_F(CPdfXfaTest, MergedDocuments)
{
	ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering true", "/XFA 5 0 R"));
	ASSERT_TRUE(pPdfFile->IsDynamicXfa());

	const std::string pdf = CreatePdf("", "");
	BYTE* data = static_cast<BYTE*>(malloc(pdf.size()));
	ASSERT_NE(nullptr, data);
	memcpy(data, pdf.data(), pdf.size());
	ASSERT_TRUE(pPdfFile->MergePages(data, static_cast<DWORD>(pdf.size())));
	EXPECT_FALSE(pPdfFile->IsDynamicXfa());
	ASSERT_TRUE(pPdfFile->UnmergePages());
	EXPECT_TRUE(pPdfFile->IsDynamicXfa());
}

TEST(CPdfXfaFileTest, DynamicFormsWithoutApplicationFonts)
{
	CTemporaryPdf file;
	ASSERT_TRUE(file.Write(CreatePdf("/AcroForm 4 0 R /NeedsRendering true", "/XFA 5 0 R")));
	EXPECT_TRUE(CPdfFile::IsDynamicXfa(file.path));
	ASSERT_TRUE(file.Write(CreatePdf("/AcroForm 4 0 R /NeedsRendering true", "/XFA [(template) 5 0 R]")));
	EXPECT_TRUE(CPdfFile::IsDynamicXfa(file.path));
}

TEST(CPdfXfaFileTest, StaticAndOrdinaryForms)
{
	CTemporaryPdf file;
	ASSERT_TRUE(file.Write(CreatePdf("/AcroForm 4 0 R", "/XFA 5 0 R")));
	EXPECT_FALSE(CPdfFile::IsDynamicXfa(file.path));
	ASSERT_TRUE(file.Write(CreatePdf("", "")));
	EXPECT_FALSE(CPdfFile::IsDynamicXfa(file.path));
}

TEST(CPdfXfaFileTest, InvalidAndMissingFile)
{
	CTemporaryPdf file;
	ASSERT_TRUE(file.Write("not a PDF"));
	EXPECT_FALSE(CPdfFile::IsDynamicXfa(file.path));
	ASSERT_TRUE(NSFile::CFileBinary::Remove(file.path));
	EXPECT_FALSE(CPdfFile::IsDynamicXfa(file.path));
}

TEST_F(CPdfXfaTest, FileProbePreservesLoadedReader)
{
	ASSERT_TRUE(Load("", ""));
	const bool needsCMap = pPdfFile->IsNeedCMap();
	const std::wstring info = pPdfFile->GetInfo();
	CTemporaryPdf file;
	ASSERT_TRUE(file.Write(CreatePdf("/AcroForm 4 0 R /NeedsRendering true", "/XFA 5 0 R")));
	EXPECT_TRUE(CPdfFile::IsDynamicXfa(file.path));
	EXPECT_EQ(needsCMap, pPdfFile->IsNeedCMap());
	EXPECT_EQ(info, pPdfFile->GetInfo());
	EXPECT_EQ(1, pPdfFile->GetPagesCount());
}

TEST_F(CPdfXfaTest, FileProbeRequiresPassword)
{
	ASSERT_TRUE(Load("/AcroForm 4 0 R /NeedsRendering true", "/XFA 5 0 R"));
	CTemporaryPdf file;
	ASSERT_EQ(S_OK, pPdfFile->ChangePassword(file.path, L"xfa-test"));
	ASSERT_TRUE(pPdfFile->LoadFromFile(file.path, L"", L"xfa-test", L"xfa-test"));
	ASSERT_TRUE(pPdfFile->IsDynamicXfa());
	EXPECT_FALSE(CPdfFile::IsDynamicXfa(file.path));
	EXPECT_FALSE(pPdfFile->IsNeedCMap());
}

TEST_F(CPdfXfaTest, FileProbeAcceptsEmptyUserPassword)
{
	// RC4-40 with owner password "xfa-owner" and an empty user password.
	const std::string trailer =
		"/Encrypt << /Filter /Standard /V 1 /R 2 /Length 40 /P -3904 "
		"/O <f4845af1ace2f2720beadc4926bb3c058ee470ff32e1f718d8d3ea7f5a36210e> "
		"/U <9472eda889640e8cbe93f38d1eded24566cd29abe0934f8d325bee93d54a6dd5> >> "
		"/ID [<00112233445566778899aabbccddeeff> <00112233445566778899aabbccddeeff>]";
	const std::string encryptedXfa =
		"\x1f\xfe\x62\x81\x63\xa8\x82\xfd\x8d\xa3\x57\xdf\x08\xe7\xa1\xf1"
		"\x73\x57\x3c\xdf\xde\xbf\xa8\x43\x7e\xfa\xa0\x68\x9f\xa3\x5e\x0e"
		"\x2a\x9c\x8d\xfa\xcf\x6d\x03\xde\x28\x1d\x3d\x38\x9d\xcd\x85\xba"
		"\x83\xba\x22\x47\x4f\xd3\xdc\x0a\x1a\xd7\xca\x68\x5e\x28\x6d";
	CTemporaryPdf file;
	ASSERT_TRUE(file.Write(CreatePdf("/AcroForm 4 0 R /NeedsRendering true", "/XFA 5 0 R", trailer, encryptedXfa)));
	ASSERT_TRUE(pPdfFile->LoadFromFile(file.path));
	ASSERT_TRUE(pPdfFile->IsDynamicXfa());
	EXPECT_FALSE(pPdfFile->CheckPerm(3));
	EXPECT_FALSE(pPdfFile->CheckPerm(5));
	EXPECT_TRUE(CPdfFile::IsDynamicXfa(file.path));
	EXPECT_FALSE(pPdfFile->CheckPerm(3));
	EXPECT_FALSE(pPdfFile->CheckPerm(5));
	EXPECT_FALSE(pPdfFile->IsNeedCMap());
}
