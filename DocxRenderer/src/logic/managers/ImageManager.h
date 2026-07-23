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

#pragma once

#include <map>

#include "../../../../DesktopEditor/common/CalculatorCRC32.h"

#include "ExternalImageStorage.h"

namespace NSDocxRenderer
{
	class CImageManager
	{
	public:
		std::map<DWORD, std::shared_ptr<CImageInfo>> m_mapImageData;
		std::wstring  m_strDstMedia {L""};
		IImageStorage* m_pExternalStorage = nullptr;

		CImageManager() = default;
		~CImageManager() = default;
		void Clear();

		std::shared_ptr<CImageInfo> WriteImage(Aggplus::CImage* pImage, double& x, double& y, double& width, double& height);
		std::shared_ptr<CImageInfo> WriteImage(const std::wstring& strFile);
		std::shared_ptr<CImageInfo> GenerateImageID(Aggplus::CImage* pImage);

		static CImageInfo::ImageType GetImageType(Aggplus::CImage* pFrame);
		static void FlipY(Aggplus::CImage* pImage);

	private:
		std::shared_ptr<CImageInfo> GenerateImageID(const std::wstring& strFileName);
		int m_lMaxSizeImage {1200};
		int m_lNextIDImage {0};
		CCalculatorCRC32 m_oCRC;
	};
}
