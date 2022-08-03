﻿/*
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
#ifndef _METAFILE_EMF_EMFFILE_H
#define _METAFILE_EMF_EMFFILE_H

#include "../Common/MetaFile.h"
#include "EmfParser/CEmfParser.h"
#include "EmfParser/CEmfxParser.h"

#include "EmfTypes.h"
#include "EmfPlayer.h"
#include "EmfPath.h"

#if defined(DrawText)
#undef DrawText
#endif

namespace MetaFile
{
	class CEmfFile
	{
	public:
		CEmfFile() : m_pParser(new CEmfParser)
        {}

		~CEmfFile()
		{
			RELEASEOBJECT(m_pParser);
		}

		bool OpenFromEmfFile(const wchar_t* wsFilePath)
		{
			if (NULL != m_pParser && m_pParser->GetType() == EmfParserType::EmfxParser)
			{
                NSFonts::IFontManager* pFont = m_pParser->GetFontManager();
                delete m_pParser;
				m_pParser = new CEmfParser();
                m_pParser->SetFontManager(pFont);
			}

			return m_pParser->OpenFromFile(wsFilePath);
		}

		bool OpenFromXmlFile(const wchar_t* wsFilePath)
		{
			if (NULL != m_pParser && m_pParser->GetType() == EmfParserType::EmfParser)
			{
                NSFonts::IFontManager* pFont = m_pParser->GetFontManager();
				delete m_pParser;
				m_pParser = new CEmfxParser();
                m_pParser->SetFontManager(pFont);
			}

			return m_pParser->OpenFromFile(wsFilePath);
		}

		CEmfParserBase* GetEmfParser()
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

		void SetOutputDevice(IOutputDevice* pOutput, const wchar_t *wsFilePath)
		{
			m_pParser->SetInterpretator(pOutput, wsFilePath);
		}

		TEmfRectL* GetBounds()
		{
			return m_pParser->GetBounds();
		}
	private:

		CEmfParserBase		*m_pParser;
	};
}

#endif // _METAFILE_EMF_EMFFILE_H
