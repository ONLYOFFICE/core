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

#define MAYBE_UNUSED(x) (void)(x)

#include "../TxtRenderer.h"

#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../../PdfFile/PdfFile.h"

int main(int argc, char* argv[])
{
	MAYBE_UNUSED(argc); MAYBE_UNUSED(argv);

	CApplicationFontsWorker worker;
	worker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
	worker.m_bIsNeedThumbnails = false;

	// worker.m_arAdditionalFolders.push_back(L"");

	if (!NSDirectory::Exists(worker.m_sDirectory))
		NSDirectory::CreateDirectory(worker.m_sDirectory);

	NSFonts::IApplicationFonts* fonts = worker.Check();

	std::wstring temp_dir = NSFile::GetProcessDirectory() + L"/temp";
	std::wstring output_dir = NSFile::GetProcessDirectory() + L"/output";

	if (!NSDirectory::Exists(temp_dir))
		NSDirectory::CreateDirectory(temp_dir);

	if (!NSDirectory::Exists(output_dir))
		NSDirectory::CreateDirectory(output_dir);

	std::vector<std::wstring> source_files = NSDirectory::GetFiles(L"");
	//source_files.push_back(L"");

	for (const auto& souce_file : source_files)
	{
		std::unique_ptr<CPdfFile> pdf_reader(new CPdfFile(fonts));
		if (!pdf_reader) continue;
		pdf_reader->SetTempDirectory(temp_dir);
		pdf_reader->LoadFromFile(souce_file);

		std::wstring ext = NSFile::GetFileExtention(souce_file);
		std::wstring filename_with_ext = NSFile::GetFileName(souce_file);
		std::wstring filename = filename_with_ext.substr(0, filename_with_ext.size() - 1 - ext.size());
		std::wstring txt_file = output_dir + L"/" + filename + L".txt";

		CTxtRenderer txt_renderer;

		time_t start = time(0);
		txt_renderer.Convert(pdf_reader.get(), txt_file);
		time_t end = time(0);

		std::cout << U_TO_UTF8(filename_with_ext) << " " << end - start << " sec" << std::endl;
	}


	fonts->Release();
	return 0;
}
