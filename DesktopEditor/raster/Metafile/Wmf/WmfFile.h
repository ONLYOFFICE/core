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
#ifndef _METAFILE_WMF_WMFFILE_H
#define _METAFILE_WMF_WMFFILE_H

#include "../../../common/StringExt.h"
#include "../Common/IOutputDevice.h"
#include "../Common/MetaFile.h"

#include "WmfTypes.h"
#include "WmfPlayer.h"

#include "WmfParser/CWmfParser.h"

#include <cmath>

#undef DrawText

namespace MetaFile
{
	class CWmfFile
	{
	public:
		CWmfFile() : m_pParser(new CWmfParser)
		{}

		~CWmfFile()
		{
			if (NULL != m_pParser)
				delete m_pParser;
		}

		bool ReadFromBuffer(BYTE* pBuffer, unsigned int unSize)
		{
			if (NULL != m_pParser)
				return m_pParser->ReadFromBuffer(pBuffer, unSize);

			return false;
		}

		bool OpenFromWmfFile(const wchar_t* wsFilePath)
		{
			if (NULL != m_pParser)
			{
				NSFonts::IFontManager* pFont = m_pParser->GetFontManager();

				delete m_pParser;
				m_pParser = new CWmfParser();

				m_pParser->SetFontManager(pFont);
			}

			return m_pParser->OpenFromFile(wsFilePath);
		}

		CWmfParserBase* GetWmfParser()
		{
			return m_pParser;
		}

		void Scan()
		{
			m_pParser->Scan();
		}

		void PlayMetaFile()
		{
			m_pParser->PlayFile();
		}

		void Close()
		{
			m_pParser->Close();
		}

		void SetFontManager(NSFonts::IFontManager* pFontManager)
		{
			m_pParser->SetFontManager(pFontManager);
		}
		bool CheckError()
		{
			return m_pParser->CheckError();
		}

		void SetOutputDevice(IOutputDevice* pOutput)
		{
			m_pParser->SetInterpretator(pOutput);
		}

		void SetOutputDevice(const wchar_t *wsFilePath, InterpretatorType oInterpretatorType, unsigned int unWidth = 0, unsigned int unHeight = 0)
		{
			m_pParser->SetInterpretator(wsFilePath, oInterpretatorType, unWidth, unHeight);
		}

		void SetOutputDevice(InterpretatorType oInterpretatorType, unsigned int unWidth = 0, unsigned int unHeight = 0)
		{
			m_pParser->SetInterpretator(oInterpretatorType, unWidth, unHeight);
		}

		void SetOutputDevice(IOutputDevice* pOutput, const wchar_t *wsFilePath)
		{
			m_pParser->SetInterpretator(pOutput, wsFilePath);
		}

		const TRectL& GetBounds() const
		{
			return m_pParser->GetDCBounds();
		}
	private:

		CWmfParserBase		*m_pParser;
	};
}

#endif // _METAFILE_WMF_WMFFILE_H
