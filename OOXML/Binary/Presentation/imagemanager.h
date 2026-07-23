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

#include "../../SystemUtility/File.h"
#include "../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../DesktopEditor/graphics/pro/Fonts.h"
#include "CalculatorCRC32.h"
#include "WMFToImageConverter.h"

#include <map>

namespace NSShapeImageGen
{
    static bool _CopyFile(std::wstring strExists, std::wstring strNew, LPVOID lpFunc, LPVOID lpData)
	{
		return NSFile::CFileBinary::Copy(strExists, strNew);
	}

	enum MediaType
	{
		itJPG	= 0,
		itPNG	= 1,
		itGIF	= 2,
		itVIF	= 3,
		itWMF	= 4,
		itEMF	= 5,
		itSVG   = 6,
		itMedia = 7,
		itUnknown = 8
	};

	class CMediaInfo
	{
	public:
		NSShapeImageGen::MediaType	m_eType;
		LONG						m_lID;
		bool						m_bValid;
		std::wstring				m_sName;
		std::wstring				m_sExt;

		CMediaInfo()
		{
			m_eType		= itJPG;
			m_lID		= -1;
			m_bValid	= true;
			m_sName		= L"image";
		}
		CMediaInfo(const CMediaInfo& oSrc)
		{
			*this = oSrc;
		}
		CMediaInfo& operator=(const CMediaInfo& oSrc)
		{
			m_eType		= oSrc.m_eType;
			m_lID		= oSrc.m_lID;
			m_bValid	= oSrc.m_bValid;
			m_sName		= oSrc.m_sName;
			m_sExt		= oSrc.m_sExt;

			return *this;
		}
		void SetNameModificator(NSShapeImageGen::MediaType eType, int typeAdditionalFile = 0 )
		{
			if(itMedia == eType)
			{
				m_sName = L"media";
				return;
			}

			int nRes = 0;
			
			if(itWMF == eType)			nRes += 1;
			if(itEMF == eType)			nRes += 2;
			
			if(typeAdditionalFile == 1)	nRes += 4;
			if(typeAdditionalFile == 2)	nRes += 8;
			
			
			if(0 != nRes)
				m_sName = L"display" + std::to_wstring(nRes) + L"image";
		}

		std::wstring GetPath2()
		{
			std::wstring _strExt;
			switch (m_eType)
			{
			case itPNG:
				_strExt = L".png";
				break;
			case itGIF:
				_strExt = L".gif";
			break;
			case itJPG:
				_strExt = L".jpg";
				break;
			case itWMF:
				_strExt = L".wmf";
				break;
			case itEMF:
				_strExt = L".emf";
				break;
			case itSVG:
				_strExt = L".svg";
				break;
			default:
				_strExt = m_sExt;
				break;
			}
			return m_sName + std::to_wstring(m_lID) + _strExt;
		}
		std::wstring GetPathWithoutExtension()
		{
			return m_sName + std::to_wstring(m_lID);
		}
	};

	class CMediaManager
	{
	public:
		std::map<std::wstring, CMediaInfo>	m_mapMediaFiles; //map for files by link
		std::map<DWORD, CMediaInfo>			m_mapMediaData; //map for files by data

		std::wstring						m_strDstMedia;
		std::wstring						m_strTempMedia;

		LONG								m_lMaxSizeImage;
		LONG								m_lNextIDImage;

		CCalculatorCRC32					m_oCRC;
		LONG								m_lDstFormat;
		NSWMFToImageConverter::CImageExt	m_oImageExt;
        NSFonts::IFontManager*				m_pFontManager;

		CMediaManager();
		~CMediaManager();

		void Clear()
		{
			m_strDstMedia	= L"";
			m_lMaxSizeImage = 1200;
			m_lNextIDImage	= 0;

			m_mapMediaData.clear();
			m_mapMediaFiles.clear();
		}

		CMediaInfo WriteImage(CBgraFrame& punkImage, double& width, double& height);
		CMediaInfo WriteImage(const std::string& strFile, double& width, double& height, const std::wstring& strAdditionalFile, int typeAdditionalFile);
		CMediaInfo WriteImage(const std::wstring& strFile, double& width, double& height, const std::wstring& strAdditionalFile, int typeAdditionalFile);
		CMediaInfo WriteMedia(const std::wstring& strFile);
        void SetFontManager(NSFonts::IFontManager* pFontManager);
	protected:
		inline void CopyFile(std::wstring& strFileSrc, std::wstring& strFileDst)
		{
			_CopyFile(strFileSrc, strFileDst, NULL, NULL);
		}

		void SaveImage(const std::wstring& strFileSrc, CMediaInfo& oInfo, LONG __width, LONG __height);
		void SaveImage(CBgraFrame& oBgraFrame, CMediaInfo& oInfo, LONG __width, LONG __height);
		CMediaInfo GenerateImageID(CBgraFrame& punkData, double dWidth, double dHeight);
		CMediaInfo GenerateImageID(std::wstring strFileName, const std::wstring & strUrl, double dWidth, double dHeight, const std::wstring& strAdditionalFile, int typeAdditionalFile);
		CMediaInfo GenerateMediaID(const std::wstring& strFileName, const std::wstring & strUrl);
		CMediaInfo GenerateImageID(std::string strFileName, const std::wstring & strUrl, double dWidth, double dHeight, const std::wstring& strAdditionalFile, int typeAdditionalFile);
		MediaType GetImageType(CBgraFrame& pFrame);
	};	
}
