﻿/*
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
#pragma once

#include "../../DesktopEditor/common/ASCVariant.h"

#include <vector>

namespace CRYPT
{
	class Decryptor;
}

namespace POLE
{
	class Storage;
}

class COfficePPTFile 
{
public:
    COfficePPTFile()
	{
		m_pReader = NULL;
		m_Status = 0;
	}
    ~COfficePPTFile();

    HRESULT put_TempDirectory(std::wstring sDir)
	{
		m_strTempDirectory = sDir;
		return S_OK;
	}
	
    std::wstring get_TempDirectory()
	{
		return m_strTempDirectory;
	}

    _UINT32 LoadFromFile(std::wstring sSrcFileName, std::wstring sDstPath, std::wstring password, bool &bMacros);
   
	_UINT32 OpenFile(const std::wstring & fileName, const std::wstring & password, bool &bMacros);
    bool CloseFile();
private:

	int							m_Status;
	void*						m_pReader;
    std::vector<std::wstring>	m_arUserSources;
    std::wstring				m_strTempDirectory;
	std::wstring				m_sTempDecryptFileName;
	


	std::wstring GetDirectory(std::wstring strFileName);
};

