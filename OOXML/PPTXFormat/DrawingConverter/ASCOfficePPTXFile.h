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

	_UINT32 LoadFromFile(std::wstring sSrcFileName, std::wstring sDstPath, std::wstring sXMLOptions);

	_UINT32 SaveToFile(std::wstring sDstFileName, std::wstring sSrcPath, std::wstring sXMLOptions);

    void get_TempDirectory(std::wstring* pVal);
    HRESULT put_TempDirectory(std::wstring newVal);

    void SetEmbeddedFontsDirectory(std::wstring val);

	void SetMediaDir		(std::wstring bsMediaDir);
	void SetFontDir			(std::wstring bsFontDir);
	void SetThemesDir		(std::wstring bsDir);
	void SetUseSystemFonts	(bool useSystemFonts);
	
	void SetIsNoBase64			(bool val);
    void SetMacroEnabled		(bool val);
	
	_UINT32 OpenFileToPPTY		(std::wstring bsInput, std::wstring bsOutput);
    _UINT32 OpenDirectoryToPPTY	(std::wstring bsInput, std::wstring bsOutput);
    _UINT32 ConvertPPTYToPPTX	(std::wstring bsInput, std::wstring bsOutput, std::wstring bsThemesFolder);
};
