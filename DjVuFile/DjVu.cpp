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
#include "DjVu.h"
#include "DjVuFileImplementation.h"

CDjVuFile::CDjVuFile(NSFonts::IApplicationFonts* pFonts)
{
    m_pImplementation = new CDjVuFileImplementation(pFonts);
}
CDjVuFile::~CDjVuFile()
{
	if (m_pImplementation)
		delete m_pImplementation;
}
bool CDjVuFile::LoadFromFile(const std::wstring& file, const std::wstring& options,
                                const std::wstring& owner_password, const std::wstring& user_password)
{
    if (m_pImplementation)
        return m_pImplementation->LoadFromFile(file, options);

    return false;
}
bool CDjVuFile::LoadFromMemory(BYTE* data, DWORD length, const std::wstring& options,
                               const std::wstring& owner_password, const std::wstring& user_password)
{
    if (m_pImplementation)
        return m_pImplementation->LoadFromMemory(data, length, options);
    return false;
}

void CDjVuFile::Close()
{
	if (m_pImplementation)
		m_pImplementation->Close();
}

OfficeDrawingFileType CDjVuFile::GetType()
{
    return odftDJVU;
}
NSFonts::IApplicationFonts* CDjVuFile::GetFonts()
{
    return m_pImplementation->GetFonts();
}

std::wstring CDjVuFile::GetTempDirectory()
{
    return m_pImplementation ? m_pImplementation->GetTempDirectory() : L"";
}
void CDjVuFile::SetTempDirectory(const std::wstring& wsDirectory)
{
	if (m_pImplementation)
		m_pImplementation->SetTempDirectory(wsDirectory);
}

int CDjVuFile::GetPagesCount()
{
	if (m_pImplementation)
		return m_pImplementation->GetPagesCount();
	return 0;
}
void CDjVuFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	if (m_pImplementation)
		m_pImplementation->GetPageInfo(nPageIndex, pdWidth, pdHeight, pdDpiX, pdDpiY);
}
void CDjVuFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak)
{
	if (m_pImplementation)
		m_pImplementation->DrawPageOnRenderer(pRenderer, nPageIndex, pBreak);
}
std::wstring CDjVuFile::GetInfo()
{
    if (m_pImplementation)
        return m_pImplementation->GetInfo();
    return L"";
}
void CDjVuFile::ConvertToPdf(const std::wstring& wsDstPath)
{
	if (m_pImplementation)
        m_pImplementation->ConvertToPdf(wsDstPath);
}
BYTE* CDjVuFile::GetStructure()
{
    if (m_pImplementation)
        return m_pImplementation->GetStructure();
    return NULL;
}
BYTE* CDjVuFile::GetLinks (int nPageIndex)
{
    if (m_pImplementation)
        return m_pImplementation->GetPageLinks(nPageIndex);
    return NULL;
}
