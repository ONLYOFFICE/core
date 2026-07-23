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

#include "../../../DesktopEditor/common/ASCVariant.h"
#include "../../Base/Base.h"

namespace PPTX
{
	class Document;
}

class CPPTXFile
{
private:
	PPTX::Document*		m_pPptxDocument;

    std::wstring		m_strTempDir;
    std::wstring		m_strDirectory;

	std::wstring		m_strFontDirectory;
	std::wstring		m_strMediaDirectory;
    bool				m_bIsUseSystemFonts;
	std::wstring		m_strEmbeddedFontsDirectory;

	std::wstring		m_strFolderThemes;
	
	bool				m_bIsNoBase64;
	bool				m_bIsMacro;
public:
	CPPTXFile();
	~CPPTXFile();

	_UINT32 LoadFromFile(std::wstring sSrcFileName, std::wstring sDstPath);

	_UINT32 SaveToFile(std::wstring sDstFileName, std::wstring sSrcPath);

    void get_TempDirectory(std::wstring* pVal);
    HRESULT put_TempDirectory(std::wstring newVal);

    void SetEmbeddedFontsDirectory(std::wstring val);

	void SetMediaDir		(std::wstring bsMediaDir);
	void SetFontDir			(std::wstring bsFontDir);
	void SetThemesDir		(std::wstring bsDir);
	void SetUseSystemFonts	(bool useSystemFonts);
	
	void SetIsNoBase64		(bool val);
    
	void SetMacroEnabled	(bool val);
	bool GetMacroEnabled	();
	
	_UINT32 OpenFileToPPTY		(std::wstring bsInput, std::wstring bsOutput);
    _UINT32 OpenDirectoryToPPTY	(std::wstring bsInput, std::wstring bsOutput);
    _UINT32 ConvertPPTYToPPTX	(std::wstring bsInput, std::wstring bsOutput, std::wstring bsThemesFolder);
};
