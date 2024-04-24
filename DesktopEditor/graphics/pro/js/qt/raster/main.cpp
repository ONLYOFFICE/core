/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

#include "raster.h"
#include "../../../../../raster/BgraFrame.h"
#include "../../../../../raster/ImageFileFormatChecker.h"
#include "../../../../../raster/Metafile/MetaFile.h"

int main()
{
	if (false)
	{
		std::wstring sFilePath = L"D:/1.jpg";
		CBgraFrame oFrame;
		if (oFrame.OpenFile(sFilePath))
		{
			oFrame.SaveFile(L"D:/1.png", 4);
			return 0;
		}
	}

	if (false)
	{
		std::wstring sFilePath = L"D:/1.emf";
		MetaFile::CMetaFile oFrame(NULL);
		if (oFrame.LoadFromFile(sFilePath.c_str()))
		{
			std::wstring sContentSvg = oFrame.ConvertToSvg();
			NSFile::CFileBinary::SaveToFile(L"D:/1.svg", sContentSvg, true);

			return 0;
		}
	}

	if (true)
	{
		BYTE* pFileBuffer = NULL;
		DWORD nFileSize = 0;
		NSFile::CFileBinary::ReadAllBytes(L"D:/image1.wmf", &pFileBuffer, nFileSize);

		void* pEncodedData = Raster_Encode(pFileBuffer, (int)nFileSize, 24);
		int nEncodedSize = Raster_GetEncodedSize(pEncodedData);
		BYTE* pEncodedBuffer = (BYTE*)Raster_GetEncodedBuffer(pEncodedData);

		NSFile::CFileBinary oFile;
		oFile.CreateFileW(L"D:/123.svg");
		oFile.WriteFile(pEncodedBuffer, nEncodedSize);
		oFile.CloseFile();

		Raster_DestroyEncodedData(pEncodedData);

		RELEASEARRAYOBJECTS(pFileBuffer);
	}

	return 1;
}
