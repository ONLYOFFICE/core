/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#ifndef _METAFILE_H
#define _METAFILE_H

#include "../../fontengine/ApplicationFonts.h"
#include "../../graphics/IRenderer.h"
#include "../../graphics/pro/Image.h"

#include "Wmf/WmfFile.h"
#include "Emf/EmfFile.h"
#include "StarView/SvmFile.h"
#include "svg/SVGTransformer.h"

namespace MetaFile
{
	class CMetaFile : public IMetaFile
	{
	public:
		CMetaFile(NSFonts::IApplicationFonts *pAppFonts);
		virtual ~CMetaFile();

		bool LoadFromFile(const wchar_t* wsFilePath);
		bool DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight);
		void Close();
		void GetBounds(double* pdX, double* pdY, double* pdW, double* pdH);
		int GetType();
		void ConvertToRaster(const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight = -1);
		NSFonts::IFontManager* get_FontManager();

	private:

		CApplicationFonts* m_pAppFonts;
		CFontManager*      m_pFontManager;
		CWmfFile           m_oWmfFile;
		CEmfFile           m_oEmfFile;
		CSvmFile           m_oSvmFile;
        CSVGTransformer    m_oSvgFile;

		int                m_lType;
	};
}

#endif //_METAFILE_H
