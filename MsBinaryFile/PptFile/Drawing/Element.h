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
#include "TextAttributesEx.h"
#include "Interactive.h"
#include "Animations.h"

namespace PPT
{
enum ElementType
{
    etGroup		 = 0,
    etVideo		 = 1,
    etAudio		 = 2,
    etPicture    = 3,
    etShape      = 4,
    etText		 = 5,
    etTable      = 6
};

class CTheme;
class CLayout;
class CSlide;

class CElement;
typedef boost::shared_ptr<CElement> CElementPtr;

class CElement
{
public:
    ElementType			m_etType;

    ODRAW::CDoubleRect			m_rcChildAnchor;
	ODRAW::CDoubleRect			m_rcAnchor;
	ODRAW::CDoubleRect			m_rcGroupAnchor;

    bool				m_bChildAnchorEnabled;
    bool				m_bAnchorEnabled;
    bool				m_bGroupAnchorEnabled;

    double				m_dStartTime;
    double				m_dEndTime;

    CTextInteractiveInfo m_oTextActions;

    std::vector<CInteractiveInfo>	m_arrActions;
    std::vector<CInteractiveInfo>   m_textHyperlinks;

    std::vector<CBulletBlip>        m_arrBlip;

    CAnimationInfo		m_oAnimations;
    CEffects			m_oEffects;

    ODRAW::CPen				m_oPen;
    ODRAW::CBrush				m_oBrush;
    ODRAW::CShadow				m_oShadow;

    int					m_lID;
    int					m_lLayoutID;

    int					m_lPlaceholderID;
    int					m_lPlaceholderType;
    bool				m_bPlaceholderSet;
    int					m_lPlaceholderSizePreset;
    int					m_lPlaceholderUserStr;
    int					m_nFormatDate;

    double				m_dRotate;			// угол поворота в градусах
    bool				m_bFlipH;			// симметричное отражение относительно горизонтальной оси
    bool				m_bFlipV;			// симметричное отражение относительно вертикальной оси

    bool				m_bLine;
    bool				m_bIsFilled;

    bool				m_bIsBackground;
    bool				m_bHaveAnchor;
    bool				m_bHidden;

    CTheme*				m_pTheme;
    CLayout*			m_pLayout;

    std::wstring		m_sName;
    std::wstring		m_sDescription;

    std::wstring		m_sHyperlink;

    std::vector<CElementPtr>*	m_pParentElements;
    std::vector<CElementPtr>	m_pChildElements;

    CElement();
    virtual ~CElement();

    virtual CElementPtr CreateDublicate() = 0;

    virtual void SetProperiesToDublicate(CElementPtr pDublicate);

    void NormalizeCoordsByMetric();
};
}
