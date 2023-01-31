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

