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

#pragma once
#include "../DesktopEditor/common/CalculatorCRC32.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../../resources/ImageInfo.h"
#include <map>
#include <memory>

namespace NSDocxRenderer
{
	class CImageManager
	{
	public:
		std::map<std::wstring, std::shared_ptr<CImageInfo>> m_mapImagesFile;
		std::map<DWORD, std::shared_ptr<CImageInfo>> m_mapImageData;
		std::shared_ptr<CImageInfo> m_pEmptyInfo;

		std::wstring m_strDstMedia{L""};

		int m_lMaxSizeImage{1200};
		int m_lNextIDImage{0};

		CCalculatorCRC32 m_oCRC;

	public:
		CImageManager(){};

		void NewDocument();

	public:
		std::shared_ptr<CImageInfo> WriteImage(Aggplus::CImage* pImage, double& x, double& y, double& width, double& height);

		std::shared_ptr<CImageInfo> WriteImage(const std::wstring& strFile, double& x, double& y, double& width, double& height);

	protected:
		void CopyFile(std::wstring& strFileSrc, std::wstring& strFileDst);

		void SaveImage(Aggplus::CImage* pImage, std::shared_ptr<CImageInfo> pInfo);

		void SaveEmptyImage(std::shared_ptr<CImageInfo> pInfo);

		std::shared_ptr<CImageInfo> GenerateImageID(Aggplus::CImage* pImage);

		std::shared_ptr<CImageInfo> GenerateImageID(const std::wstring& strFileName);

		CImageInfo::ImageType GetImageType(Aggplus::CImage* pFrame);

		void FlipY(Aggplus::CImage* pImage);

		void FlipX(CBgraFrame* pImage);
	};
} // namespace NSDocxRenderer
