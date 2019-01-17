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
#ifndef _ASC_HTMLRENDERER_FM_H_
#define _ASC_HTMLRENDERER_FM_H_

#include "FontManagerBase.h"

namespace NSHtmlRenderer
{
	using namespace NSFontManager;

	const long g_lNewNoJustifySpace = 5;

	class CFontManager : public CFontManagerBase
	{
	public:
        NSStructures::CFont*	m_pFont;
        Aggplus::CMatrix*		m_pTransform;

        double					m_dSpaceWidthMM;

	public:
		CFontManager() : m_pFont(NULL), CFontManagerBase()
		{
			m_pTransform = NULL;
			m_dSpaceWidthMM = 0;
		}
		virtual ~CFontManager()
		{
		}

	public:
		virtual void LoadFont(long lFaceIndex = 0, bool bIsNeedAddToMap = true)
		{
			if (NULL == m_pManager)
				return;

			double dSize = m_pFont->Size;
            double dSizeFont = dSize * ((m_pTransform->sx() + m_pTransform->sy()) / 2);

			m_pFont->Size = dSizeFont;

            if (IsEqual2(m_pFont, &m_oFont.m_oFont))
			{
				m_pFont->Size = dSize;
				return;
			}

			m_oFont.m_oFont	= *m_pFont;
			m_pFont->Size = dSize;

			bool bIsPath = false;

            if (L"" == m_pFont->Path)
			{
				CFontManagerBase::LoadFontByName(m_oFont.m_oFont.Name, m_oFont.m_oFont.Size, m_oFont.m_oFont.GetStyle());
			}
			else
			{
				CFontManagerBase::LoadFontByFile(m_oFont.m_oFont.Path, m_oFont.m_oFont.Size, lFaceIndex);

				m_pFont->SetStyle(m_oFont.m_oProperties.m_lStyle);
				m_oFont.m_oFont.SetStyle(m_oFont.m_oProperties.m_lStyle);

				bIsPath = true;
			}

			CalculateSpace();
		}

        inline void CalculateSpace()
		{
            LONG lGid = m_pManager->GetStringGID();
            m_pManager->SetStringGID(FALSE);
			
            m_pManager->LoadString1(L" ", 0, 0);

            TBBox _box = m_pManager->MeasureString2();

            m_dSpaceWidthMM = (double)(_box.fMaxX - _box.fMinX) * c_dPixToMM;
			if (0 >= m_dSpaceWidthMM)
			{
				m_dSpaceWidthMM = 1.0;
			}

			m_pManager->SetStringGID(lGid);
		}
	};
}

#endif // _ASC_HTMLRENDERER_FM_H_
