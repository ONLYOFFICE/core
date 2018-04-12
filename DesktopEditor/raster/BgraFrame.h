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
#ifndef _BUILD_BGRA_FRAME_
#define _BUILD_BGRA_FRAME_

#include <string>
#include "../common/Types.h"
#include "./../graphics/config.h"

class GRAPHICS_DECL CBgraFrame
{
private:
	int m_nFileType;

	int m_lWidth;
	int m_lHeight;
	int m_lStride;

	BYTE* m_pData;

	bool m_bIsGrayScale;

public:
    CBgraFrame();
    ~CBgraFrame();

public:
    void Destroy();
    void Clear();

public:
    void ClearNoAttack();
    int get_Width();
    int get_Height();
    void put_Width(const int& lWidth);
    void put_Height(const int& lHeight);
    int get_Stride();
    void put_Stride(const int& lStride);
    BYTE* get_Data();
    void put_Data(BYTE* pData);

    bool IsGrayScale();

public:
	bool OpenFile(const std::wstring& strFileName, unsigned int nFileType = 0);	//0 - detect
	bool SaveFile(const std::wstring& strFileName, unsigned int nFileType);
    
	bool Resize(const long& nNewWidth, const long& nNewHeight, bool bDestroyData = true);

	bool ReColorPatternImage(const std::wstring& strFileName, unsigned int rgbColorBack, unsigned int rgbColorFore);
};

#endif
