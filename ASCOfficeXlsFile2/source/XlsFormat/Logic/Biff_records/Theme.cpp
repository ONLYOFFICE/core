/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "Theme.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../Common/DocxFormat/Source/Base/Base.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Theme.h"

namespace XLS
{

Theme::Theme()
{
	nThemeDataSize = 0;
}

Theme::~Theme()
{
}

BaseObjectPtr Theme::clone()
{
	return BaseObjectPtr(new Theme(*this));
}

void Theme::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();

	record >> frtHeader >> dwThemeVersion;

	if (dwThemeVersion == 124226)
	{
		//default theme
	}
	else
	{
		nThemeDataSize = record.getDataSize() - record.getRdPtr();
		pThemeData = boost::shared_array<char>(new char[nThemeDataSize]);

		memcpy(pThemeData.get(), record.getCurData<char>(), nThemeDataSize);
		record.skipNunBytes(nThemeDataSize);

		std::wstring tempThemePath = global_info->tempDirectory + FILE_SEPARATOR_STR + L"theme.temp";
		
		NSFile::CFileBinary file;	
		if (!file.CreateFileW(tempThemePath)) return;

		file.WriteFile((BYTE*)pThemeData.get(), nThemeDataSize);
		file.CloseFile();

		COfficeUtils OfficeUtils(NULL);
		
		ULONG nBufferSize = 0;
		BYTE *pBuffer = NULL;

		HRESULT hresult = OfficeUtils.LoadFileFromArchive(tempThemePath, L"theme1.xml", &pBuffer, nBufferSize);// todooo - parsing ThemeManager

		if (hresult != S_OK || pBuffer == NULL)
			hresult = OfficeUtils.LoadFileFromArchive(tempThemePath, L"theme/theme1.xml", &pBuffer, nBufferSize);
		
		if (hresult != S_OK || pBuffer == NULL)
			hresult = OfficeUtils.LoadFileFromArchive(tempThemePath, L"theme/theme/theme1.xml", &pBuffer, nBufferSize);
		//???  переписать по нормальному
		if (hresult == S_OK && pBuffer != NULL)
		{
			global_info->sTheme = std::string((char*)pBuffer, nBufferSize);

			global_info->m_pTheme = boost::shared_ptr<PPTX::Theme>(new PPTX::Theme(NULL, global_info->sTheme));

			delete []pBuffer;
			pBuffer = NULL;
		}

	}
}

} // namespace XLS

