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
