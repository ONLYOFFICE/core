/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

class CxImage;
class CBgraFrame
{
private:
	int m_lWidth;
	int m_lHeight;
	int m_lStride;

	BYTE* m_pData;

	bool m_bIsGrayScale;

public:
	CBgraFrame()
	{
		Clear();
	}
	~CBgraFrame()
	{
		Destroy();
	}

private:
	inline void Destroy()
	{
		if (NULL != m_pData)
			delete []m_pData;

		Clear();
	}
	inline void Clear()
	{
		m_lWidth	= 0;
		m_lHeight	= 0;
		m_lStride	= 0;
		m_pData		= NULL;
		m_bIsGrayScale  = false;
	}

public:
	inline void ClearNoAttack()
	{
		Clear();
	}
	inline int get_Width()
	{
		return m_lWidth;
	}
	inline int get_Height()
	{
		return m_lHeight;
	}
	inline void put_Width(const int& lWidth)
	{
		m_lWidth = lWidth;
	}
	inline void put_Height(const int& lHeight)
	{
		m_lHeight = lHeight;
	}
	inline int get_Stride()
	{
		return m_lStride;
	}
	inline void put_Stride(const int& lStride)
	{
		m_lStride = lStride;
	}
	inline BYTE* get_Data()
	{
		return m_pData;
	}
	inline void put_Data(BYTE* pData)
	{
		m_pData = pData;
	}

	inline bool IsGrayScale()
	{
		return m_bIsGrayScale;
	}

public:
	bool OpenFile(const std::wstring& strFileName, unsigned int nFileType = 0);	//0 - detect
	bool SaveFile(const std::wstring& strFileName, unsigned int nFileType);
    bool Resize(const long& nNewWidth, const long& nNewHeight, bool bDestroyData = true);
private:

	void CxImageToMediaFrame( CxImage& img );
};

#endif
