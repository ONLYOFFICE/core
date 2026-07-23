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
#ifndef DOCTRENDERER_H
#define DOCTRENDERER_H

#include <string>
#include <vector>
#include "./common.h"

namespace NSDoctRenderer
{
	namespace DoctRendererFormat
	{
		enum FormatFile
		{
			DOCT      = 0,
			XLST      = 1,
			PPTT      = 2,
			PDF       = 3,
			HTML      = 4,
			PPTX_THEME_THUMBNAIL = 5,
			IMAGE     = 6,
			VSDT      = 7,
			WATERMARK = 8,

			INVALID = 255
		};
	}

	enum class AdditionalParamType
	{
		DRAWINGFILE = 0,
		INVALID = 255
	};
}

namespace NSDoctRenderer
{
	class CDoctRenderer_Private;
	class BUILDER_DECL CDoctrenderer
	{
	public:
		CDoctrenderer(const std::wstring& sAllFontsPath = L"");
		void LoadConfig(const std::wstring& sConfigDir, const std::wstring& sAllFontsPath = L"");
		~CDoctrenderer();

	public:
		bool Execute(const std::wstring& strXml, std::wstring& strError);
		std::vector<std::wstring> GetImagesInChanges();
		void CreateCache(const std::wstring& sAllFontsPath, const std::wstring& sCacheDir);
		void CreateSnapshots();

		void SetAdditionalParam(const AdditionalParamType& type, void* data);

	private:
		CDoctRenderer_Private* m_pInternal;
	};
}

#endif // DOCTRENDERER_H
