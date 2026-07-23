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
#ifndef _METAFILE_H
#define _METAFILE_H

#include "../../graphics/pro/Fonts.h"
#include "../../graphics/IRenderer.h"
#include "../../graphics/pro/Image.h"

#ifdef METAFILE_SUPPORT_WMF_EMF
#include "Wmf/WmfFile.h"
#include "Emf/EmfFile.h"
#endif

#ifdef METAFILE_SUPPORT_SVM
#include "StarView/SvmFile.h"
#endif

#ifdef METAFILE_SUPPORT_SVG
#ifdef SVG_OLD_ENGINE
#include "svg/SVGTransformer.h"
typedef CSVGTransformer CSvgFile;
#else
#include "svg/CSvgFile.h"
#endif
#endif

namespace MetaFile
{
	class CMetaFile : public IMetaFile
	{
	public:
		CMetaFile(NSFonts::IApplicationFonts *pAppFonts);
		virtual ~CMetaFile();

		bool LoadFromFile(const wchar_t* wsFilePath);
		bool LoadFromBuffer(BYTE* pBuffer, unsigned int unSize);
		bool LoadFromString(const std::wstring& data);
		bool DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight);
		void Close();
		void GetBounds(double* pdX, double* pdY, double* pdW, double* pdH);
		int GetType();
		void ConvertToRaster(const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight = -1);
		void ConvertToRaster(BYTE*& pBuffer, int& nSize, unsigned int unFileType, int nWidth, int nHeight = -1);

		NSFonts::IFontManager* get_FontManager();

		// conversion to Svg
		std::wstring ConvertToSvg(unsigned int unWidth = 0, unsigned int unHeight = 0);

		void SetTempDirectory(const std::wstring& dir);

		// For tests
	#ifdef METAFILE_SUPPORT_WMF_EMF
		void ConvertToXml(const wchar_t *wsFilePath);
		void ConvertToXmlAndRaster(const wchar_t *wsXmlFilePath, const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight = -1);
		bool LoadFromXmlFile(const wchar_t* wsFilePath);
		bool DrawOnRenderer(const wchar_t *wsXmlFilePath, IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight);
		void ConvertToEmf(const wchar_t* wsFilePath);
	#endif


	private:
		void ConvertTo(const wchar_t* wsOutFilePath, BYTE*& pBuffer, int& nSize, unsigned int unFileType, int nWidth, int nHeight = -1);

		NSFonts::IApplicationFonts* m_pAppFonts;
		NSFonts::IFontManager*      m_pFontManager;

	#ifdef METAFILE_SUPPORT_WMF_EMF
		CWmfFile           m_oWmfFile;
		CEmfFile           m_oEmfFile;
	#endif

	#ifdef METAFILE_SUPPORT_SVM
		CSvmFile           m_oSvmFile;
	#endif

	#ifdef METAFILE_SUPPORT_SVG
		CSvgFile           m_oSvgFile;
	#endif

		int                m_lType;
	};
}

#endif //_METAFILE_H
