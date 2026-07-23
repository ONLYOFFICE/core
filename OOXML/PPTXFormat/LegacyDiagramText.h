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

#include "../DocxFormat/File.h"
#include "../Base/Base.h"

#include "Logic/TextParagraphPr.h"
#include "Logic/Colors/SrgbClr.h"
#include "FileTypes.h"
#include <string>

namespace PPTX
{
	class LegacyDiagramText : public OOX::File
	//[MS-PPT] Section 2.9.1; DocumentTextInfoContainer
	{
	public:
		LegacyDiagramText(OOX::Document *pMain);
		LegacyDiagramText(OOX::Document *pMain, const OOX::CPath& filename);
		virtual ~LegacyDiagramText();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content) const;
		LegacyDiagramText& operator=(const LegacyDiagramText& oSrc);

	public:
		virtual const OOX::FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

		std::wstring										m_Text;

		std::vector<PPTX::Logic::RunProperties>		m_arrRPr;
		std::vector<PPTX::Logic::TextParagraphPr>	m_arrPPr;
	private:
		BYTE*	m_Data;
		int		m_DataSize;

		struct SRecordHeader
		{
			_UINT16			RecVer_Inst;
			_UINT16			RecType;
			_UINT32	        RecLen;
		};

		void Parse();
		void ParseTextProps(BYTE* Data, int size, int count_text);
		int ParseParaProps(PPTX::Logic::TextParagraphPr& props, BYTE* Data, int size, int  & count);
		int ParseRunProps(PPTX::Logic::RunProperties& props, BYTE* Data, int size, int  & count);
	};
} // namespace PPTX
