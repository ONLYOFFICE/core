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

#include "Shape.h"
#include "ShapeProperties.h"

#include "../ExObjRefAtom.h"
#include "../TextBytesAtom.h"
#include "../TextCharsAtom.h"
#include "../TextHeaderAtom.h"
#include "../PlaceHolderAtom.h"
#include "../StyleTextPropAtom.h"
#include "../OutlineTextRefAtom.h"
#include "../MouseInteractiveInfoContainer.h"
#include "../TextInteractiveInfoAtom.h"
#include "../MasterTextPropAtom.h"
#include "../HeadersFootersAtom.h"

#include "../../Reader/ClassesAtom.h"
#include "../../Reader/SlideInfo.h"

#include "../../Drawing/Document.h"
#include "../../../Common/Vml/PPTShape/ElementSettings.h"

#include "../../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../../OOXML/Base/Base.h"


#define FIXED_POINT_unsigned(val) (double)((WORD)(val >> 16) + ((WORD)(val) / 65536.0))

namespace PPT
{
class CPPTElement
{
public:
    bool ChangeBlack2ColorImage(std::wstring image_path, int rgbColor1, int rgbColor2);
    CColor CorrectSysColor(int nColorCode, CElementPtr pElement, CTheme* pTheme);

    void SetUpProperties(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pWrapper, CSlide* pSlide, CProperties* pProperties, bool reset_default);
    void SetUpProperty(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty);
    void SetUpPropertyVideo(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty);
    void SetUpPropertyAudio(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty);
    void SetUpPropertyImage(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty);
    void SetUpPropertyShape(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty);
};

class CRecordShapeContainer : public CRecordsContainer
{
private:
    POLE::Stream* m_pStream;

public:

    bool bGroupShape;

    CRecordShapeContainer();

    ~CRecordShapeContainer();

    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream);

    CElementPtr GetElement (bool inGroup, CExMedia* pMapIDs,
                            CTheme* pTheme, CLayout* pLayout,
                            CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide = NULL);

    ElementType GetTypeElem(eSPT eType);
    std::wstring GetFileName(std::wstring strFilePath);

protected:

    bool isTable()const;
    std::wstring getTableXmlStr()const;
    void ApplyThemeStyle(CElementPtr pElem, CTheme* pTheme, CRecordMasterTextPropAtom* master_levels);
    void SetUpTextStyle(std::wstring& strText, CTheme* pTheme, CLayout* pLayout, CElementPtr pElem, CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide, CRecordMasterTextPropAtom* master_levels);
    void ApplyHyperlink(CShapeElement* pShape, CColor& oColor);
    static void addHyperlinkToSpan(CSpan& oSpan, const std::vector<CInteractiveInfo> &arrInteractive, const CColor& oColor);
    static bool isRealHyperlink( const std::vector<CInteractiveInfo> &arrInteractive);
    static std::vector<std::vector<CInteractiveInfo> > splitInteractive(const std::vector<CInteractiveInfo>& arrInteractive);
    static void ConvertInteractiveInfo(CInteractiveInfo& interactiveInfo, const CRecordMouseInteractiveInfoContainer* interactiveCont, CExMedia* pMapIDs);
    void ConvertStyleTextProp9(CTextAttributesEx *pText);
};

}
