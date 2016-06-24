/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#include "OOXFontReader.h"
#include "../RtfProperty.h"


class OOXFontSchemeReader
{
private:
	OOX::Drawing::CFontScheme * m_ooxFontScheme;
public: 
	OOXFontSchemeReader(OOX::Drawing::CFontScheme * ooxFontScheme)
	{
		m_ooxFontScheme = ooxFontScheme;
	}
	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxFontScheme == NULL) return false;
		
		if (m_ooxFontScheme->m_oMajorFont.m_oLatin.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_smajorAscii = m_ooxFontScheme->m_oMajorFont.m_oLatin.m_oTypeFace->GetValue();
			oParam.oReader->m_smajorHAnsi = m_ooxFontScheme->m_oMajorFont.m_oLatin.m_oTypeFace->GetValue();
		}
		if (m_ooxFontScheme->m_oMajorFont.m_oEa.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_smajorEastAsia = m_ooxFontScheme->m_oMajorFont.m_oEa.m_oTypeFace->GetValue();
		}
		if (m_ooxFontScheme->m_oMajorFont.m_oCs.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_smajorBidi = m_ooxFontScheme->m_oMajorFont.m_oCs.m_oTypeFace->GetValue();
		}
		//for (long i=0 ; i < m_ooxFontScheme->m_oMajorFont.m_arrFont.size(); i++)
		//{
		//}

		if (m_ooxFontScheme->m_oMinorFont.m_oLatin.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_sminorAscii = m_ooxFontScheme->m_oMinorFont.m_oLatin.m_oTypeFace->GetValue();
			oParam.oReader->m_sminorHAnsi = m_ooxFontScheme->m_oMinorFont.m_oLatin.m_oTypeFace->GetValue();
		}
		if (m_ooxFontScheme->m_oMinorFont.m_oEa.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_sminorEastAsia = m_ooxFontScheme->m_oMinorFont.m_oEa.m_oTypeFace->GetValue();
		}
		if (m_ooxFontScheme->m_oMinorFont.m_oCs.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_sminorBidi = m_ooxFontScheme->m_oMinorFont.m_oCs.m_oTypeFace->GetValue();
		}
		//for (long i=0 ; i < m_ooxFontScheme->m_oMinorFont.m_arrFont.size(); i++)
		//{
		//}


		return true;
	}
};
