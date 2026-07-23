/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "Theme.h"
#include "Shapes/Shape.h"
#include "../../Common/Vml/PPTShape/Ppt2PptxShapeConverter.h"


namespace PPT
{
class CImageElement : public CElement
{
public:
    std::wstring	m_strImageFileName;

    BYTE			m_nAlpha;

    bool			m_bCropEnabled;

    _INT32			m_lcropFromRight;
    _INT32			m_lcropFromLeft;
    _INT32			m_lcropFromTop;
    _INT32			m_lcropFromBottom;

    _INT32			m_lpictureContrast;
    _INT32			m_lpictureBrightness;

    bool			m_bStretch;
    bool			m_bTile;

    bool			m_bOLE;
    bool			m_bImagePresent;

    std::wstring	m_sImageName;

    CImageElement();
    virtual ~CImageElement();
    virtual CElementPtr CreateDublicate();

    std::wstring ConvertPPTShapeToPPTX(bool bIsNamespace = false);
    std::wstring ConvertPPTtoPPTX(/*CPPTShape* pPPTShape, */const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace = false);

    std::wstring DownloadImage(const std::wstring& strFile);
};

class CShapeElement : public CElement
{
public:
    NSBaseShape::ClassType m_ClassType;

    int			m_lShapeType;
    CShapePtr	m_pShape;
    bool		m_bShapePreset; // or rect (

    CShapeElement(NSBaseShape::ClassType ClassType, int eType);

    CShapeElement();

    CShapeElement(const std::wstring& str);
    virtual ~CShapeElement();

    virtual CElementPtr CreateDublicate();
    bool SetUpTextPlaceholder(std::wstring newText);

    virtual void SetupProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout);

    virtual void SetupTextProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout);

    void CalculateColor(CColor& oColor, CSlide* pSlide, CTheme* pTheme, CLayout* pLayout);

    std::wstring ConvertPPTShapeToPPTX(bool bIsNamespace = false);
    std::wstring ConvertPPTtoPPTX(CPPTShape* pPPTShape, const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace = false);
};
class COleObjectElement : public CImageElement
{
public:
    std::wstring	m_strBinFileName;

    std::wstring	m_strOleName;
    std::wstring	m_strProgId;

    COleObjectElement();
    virtual ~COleObjectElement();

    virtual CElementPtr CreateDublicate();
};
class CAudioElement : public CImageElement
{
public:
    std::wstring	m_strAudioFileName;

    BYTE			m_nAmplify;
    bool			m_bWithVideo;
    double			m_dAudioDuration;

    double			m_dClipStartTime;
    double			m_dClipEndTime;
    bool			m_bLoop;

    CAudioElement();

    virtual ~CAudioElement();

    virtual CElementPtr CreateDublicate();
};

class CVideoElement : public CImageElement
{
public:
    std::wstring	m_strVideoFileName;
    double			m_dVideoDuration;
    BYTE			m_nAlpha;

    double			m_dClipStartTime;
    double			m_dClipEndTime;

    bool			m_bLoop;

    CVideoElement();

    virtual ~CVideoElement();

    virtual CElementPtr CreateDublicate();
};

class CGroupElement : public CElement
{
public:

public:
    CGroupElement();
    virtual ~CGroupElement();
    virtual CElementPtr CreateDublicate();
    std::wstring ConvertPPTShapeToPPTX(bool bIsNamespace = false);
};

class CTableElement : public CElement
{
public:
    std::wstring m_xmlRawData;
public:
    CTableElement();
    virtual ~CTableElement();
    virtual CElementPtr CreateDublicate();
    int GetWidth()const;
    int GetHeight()const;
    std::wstring ConvertPPTShapeToPPTX(bool bIsNamespace = false);
};
}

