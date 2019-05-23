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
#include "SimpleTypes_Word.h"

namespace SimpleTypes
{	
    template<>
    EHexColor CHexColor<EHexColor::hexcolorAuto>::FromString (std::wstring &sValue)
    {
        if ( _T("auto") == sValue || _T("none") == sValue )
            this->m_eValue = hexcolorAuto;
        else
        {
			size_t split = sValue.find(L" ["); //index ala #ff9 [43]
			if (std::wstring::npos != split)
			{
				sValue = sValue.substr(0, split);
			}
            //В документации не написано, что цвет может приходить строкой, но в реальных докуентах встречается и word это разруливает.
            //CHighlightColor<highlightcolorNone> oHighlightColor(sValue);
            CHighlightColor<> oHighlightColor(sValue);
            if(SimpleTypes::highlightcolorNone != oHighlightColor.GetValue())
            {
                this->m_eValue = hexcolorRGB;
                m_unR = oHighlightColor.Get_R();
                m_unG = oHighlightColor.Get_G();
                m_unB = oHighlightColor.Get_B();
            }
            else if ( 6 <= sValue.length() )
            {
                this->m_eValue = hexcolorRGB;
                m_sValue = sValue.substr( 0, 6 );
                Parse();
            }
            else if ( 3 == sValue.length() )// a la #339 (Compo 3AP.docx)
            {
                this->m_eValue = hexcolorRGB;
                m_sValue = sValue;
                Parse3();
            }
            else   this->m_eValue = EHexColor::hexcolorAuto;//eDefValue;

        }

        return this->m_eValue;
    }
    template<>
    EHexColor CHexColor<EHexColor::hexcolorRGB>::FromString (std::wstring &sValue)
    {
        if ( _T("auto") == sValue || _T("none") == sValue )
            this->m_eValue = hexcolorAuto;
        else
        {
            //В документации не написано, что цвет может приходить строкой, но в реальных докуентах встречается и word это разруливает.
            //CHighlightColor<highlightcolorNone> oHighlightColor(sValue);
            CHighlightColor<> oHighlightColor(sValue);
            if(SimpleTypes::highlightcolorNone != oHighlightColor.GetValue())
            {
                this->m_eValue = hexcolorRGB;
                m_unR = oHighlightColor.Get_R();
                m_unG = oHighlightColor.Get_G();
                m_unB = oHighlightColor.Get_B();
            }
            else if ( 6 <= sValue.length() )
            {
                this->m_eValue = hexcolorRGB;
                m_sValue = sValue.substr( 0, 6 );
                Parse();
            }
            else if ( 3 == sValue.length() )// a la #339 (Compo 3AP.docx)
            {
                this->m_eValue = hexcolorRGB;
                m_sValue = sValue;
                Parse3();
            }
            else   this->m_eValue = EHexColor::hexcolorAuto;//eDefValue;

        }

        return this->m_eValue;

    }
} // SimpleTypes
