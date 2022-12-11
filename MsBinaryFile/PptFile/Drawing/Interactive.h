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
#include "Attributes.h"

#include "../../../DesktopEditor/common/ASCVariant.h"

namespace PPT
{
class CTextRange
{
public:
    int m_lStart;
    int m_lEnd;

public:
    CTextRange()
    {
        m_lStart	= 0;
        m_lEnd		= 0;
    }
    CTextRange& operator=(const CTextRange& oSrc)
    {
        m_lStart	= oSrc.m_lStart;
        m_lEnd		= oSrc.m_lEnd;

        return *this;
    }
    CTextRange(const CTextRange& oSrc)
    {
        *this = oSrc;
    }
    bool inRange(const int pos)const
    {
        return (pos >= m_lStart && pos < m_lEnd);
    }
};
class CInteractiveInfo
{
public:
    enum : bool
    {
        click,
        over
    };
    CInteractiveInfo()
    {
        m_bPresent = false;
    }

    CInteractiveInfo& operator=(const CInteractiveInfo& oSrc)
    {
        m_bPresent			= oSrc.m_bPresent;
        m_lType				= oSrc.m_lType;

        m_lOleVerb			= oSrc.m_lOleVerb;
        m_lJump				= oSrc.m_lJump;
        m_lHyperlinkType	= oSrc.m_lHyperlinkType;
        m_bAnimated			= oSrc.m_bAnimated;
        m_bStopSound		= oSrc.m_bStopSound;
        m_bCustomShowReturn	= oSrc.m_bCustomShowReturn;
        m_bVisited			= oSrc.m_bVisited;

        m_strAudioFileName	= oSrc.m_strAudioFileName;
        m_strHyperlink		= oSrc.m_strHyperlink;
        m_strAudioName          = oSrc.m_strAudioName;
        m_eActivation           = oSrc.m_eActivation;
        m_macro                 = oSrc.m_macro;
        return *this;
    }
    CInteractiveInfo(const CInteractiveInfo& oSrc)
    {
        *this = oSrc;
    }

    bool			m_bPresent;

    int				m_lType;
    int				m_lOleVerb;
    int				m_lJump;
    int				m_lHyperlinkType;
    bool			m_bAnimated;
    bool			m_bStopSound;
    bool			m_bCustomShowReturn;
    bool			m_bVisited;
    bool                        m_eActivation; // Click or over

    std::wstring	m_strAudioFileName;
    std::wstring	m_strHyperlink;
    std::wstring        m_strAudioName;
    std::wstring        m_macro;

};
class CTextInteractiveInfo
{
private:
    long        m_lType;
    ASC_VARIANT m_varParameter;

public:
    bool	m_bPresent;

    std::vector<CTextRange> m_arRanges;

    CTextInteractiveInfo()
    {
        m_bPresent = false;
    }
    ~CTextInteractiveInfo()
    {
    }

    CTextInteractiveInfo& operator=(const CTextInteractiveInfo& oSrc)
    {
        m_lType			= oSrc.m_lType;
        m_varParameter	= oSrc.m_varParameter;

        m_bPresent		= oSrc.m_bPresent;

        m_arRanges.insert(m_arRanges.end(), oSrc.m_arRanges.begin(), oSrc.m_arRanges.end());

        return *this;
    }
    CTextInteractiveInfo(const CTextInteractiveInfo& oSrc)
    {
        *this = oSrc;
    }
};
}
