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
#ifndef DOC_FORMAT_LIB
#define DOC_FORMAT_LIB

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#endif

#include "../../DesktopEditor/common/ASCVariant.h"

struct ProgressCallback;

class COfficeDocFile 
{
public:
	COfficeDocFile()
	{
	}

	virtual ~COfficeDocFile()
	{
	}

	std::wstring m_sTempFolder;

    _UINT32 LoadFromFile(const std::wstring & sSrcFileName, const std::wstring & sDstFileName, const std::wstring & password, bool &bMacros, ProgressCallback *ffCallBack = NULL);
    _UINT32 SaveToFile(const std::wstring &  sDstFileName, const std::wstring &  sSrcFileName, ProgressCallback *ffCallBack = NULL);

};

#endif //DOC_FORMAT_LIB
