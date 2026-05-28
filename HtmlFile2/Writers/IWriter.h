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

#ifndef IWRITER_H
#define IWRITER_H

#include "../../DesktopEditor/graphics/pro/Fonts.h"

#include "../../Common/3dParty/html/css/src/CNode.h"
#include "../Common.h"

namespace HTML
{
class IWriter
{
protected:
	const std::wstring *m_pTempDir;  // Temp folder
	const std::wstring *m_pSrcPath;  // Source directory
	const std::wstring *m_pBasePath; // Full base path
	const std::wstring *m_pCorePath; // Path to the root file (used for working with Epub)

	NSFonts::IApplicationFonts* m_pFonts; // Necessary for optimizing font handling
public:
	IWriter()
		: m_pTempDir(nullptr), m_pSrcPath(nullptr),
		  m_pBasePath(nullptr), m_pCorePath(nullptr),
		  m_pFonts(nullptr)
	{};

	virtual ~IWriter()
	{
		if (nullptr != m_pFonts)
			delete m_pFonts;
	};

	virtual void Begin(const std::wstring& wsDst) = 0;
	virtual void End(const std::wstring& wsDst) = 0;

	virtual bool WriteText(std::wstring wsText, const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) = 0;

	virtual void WriteMetadata(const std::wstring& wsName, const std::wstring& wsValue) = 0;

	virtual void PageBreak() = 0;

	virtual void BeginBlock() = 0;
	virtual void EndBlock(bool bAddBlock) = 0;

	virtual void SetDataOutput(XmlString* pOutputData) = 0; // Set output location for interpreter
	virtual void RevertDataOutput() = 0; // Revert output location to original

	virtual XmlString* GetCurrentDocument() const = 0;

	void SetSrcDirectory (const std::wstring& wsPath)
	{
		m_pSrcPath = &wsPath;
	}
	void SetTempDirectory(const std::wstring& wsPath)
	{
		m_pTempDir = &wsPath;
	}
	void SetBaseDirectory(const std::wstring& wsPath)
	{
		m_pBasePath = &wsPath;
	}
	void SetCoreDirectory(const std::wstring& wsPath)
	{
		m_pCorePath = &wsPath;
	}

	std::wstring GetTempDir()  const
	{
		return (nullptr != m_pTempDir) ? *m_pTempDir : std::wstring();
	}
	std::wstring GetSrcPath()  const
	{
		return (nullptr != m_pSrcPath) ? *m_pSrcPath : std::wstring();
	}
	std::wstring GetBasePath() const
	{
		return (nullptr != m_pBasePath) ? *m_pBasePath : std::wstring();
	}
	std::wstring GetCorePath() const
	{
		return (nullptr != m_pCorePath) ? *m_pCorePath : std::wstring();
	}

	NSFonts::IApplicationFonts* GetFonts()
	{
		if (nullptr == m_pFonts)
		{
			m_pFonts = NSFonts::NSApplication::Create();

			if (NULL != m_pFonts)
				m_pFonts->Initialize();
		}

		return m_pFonts;
	}
};
}

#endif // IWRITER_H
