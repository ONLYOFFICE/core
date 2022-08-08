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

		TRectD GetBounds()
		{
			return m_pParser->GetBounds();
		}
	private:

		CWmfParserBase		*m_pParser;
	};
}

#endif // _METAFILE_WMF_WMFFILE_H
