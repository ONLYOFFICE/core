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

		CTxtRenderer txt_renderer(fonts);

		time_t start = time(0);
		txt_renderer.Convert(pdf_reader.get(), txt_file);
		time_t end = time(0);

		std::cout << U_TO_UTF8(filename_with_ext) << " " << end - start << " sec" << std::endl;
	}


	fonts->Release();
	return 0;
}
