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

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

namespace
{
std::string CreatePdf(const std::string& catalog, const std::string& form)
{
	const std::string xfa = "<template xmlns=\"http://www.xfa.org/schema/xfa-template/3.3/\"/>";
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
	pdf << "trailer\n<< /Size " << objects.size() + 1 << " /Root 1 0 R >>\n";
	pdf << "startxref\n" << xref << "\n%%EOF\n";
	return pdf.str();
}
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
