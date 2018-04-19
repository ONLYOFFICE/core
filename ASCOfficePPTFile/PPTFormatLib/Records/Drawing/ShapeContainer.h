﻿/*
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
#include "../InteractiveInfoAtom.h"
#include "../TextInteractiveInfoAtom.h"
#include "../MasterTextPropAtom.h"
#include "../HeadersFootersAtom.h"

#include "../../Reader/ClassesAtom.h"
#include "../../Reader/SlideInfo.h"

#include "../../../../ASCOfficePPTXFile/Editor/Drawing/Document.h"
#include "../../../../ASCOfficePPTXFile/Editor/Drawing/Shapes/BaseShape/PPTShape/ElementSettings.h"

#include "../../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../../Common/DocxFormat/Source/Base/Types_32.h"

#define FIXED_POINT_unsigned(val) (double)((WORD)(val >> 16) + ((WORD)(val) / 65536.0))

using namespace NSOfficeDrawing;
using namespace NSPresentationEditor;

class CPPTElement
{
public:
	inline double FixedPointToDouble(unsigned int op)
	{
		short			Integral	= op >> 16;
		unsigned short Fractional	= op - (Integral << 16);

		return Integral + (Fractional / 65536.0);
	}

	bool ChangeBlack2ColorImage(std::wstring image_path, int rgbColor1, int rgbColor2);
	CColor CorrectSysColor(int nColorCode, CElementPtr pElement, CTheme* pTheme);
	
	void SetUpProperties(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pWrapper, CSlide* pSlide, CProperties* pProperties);
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

	CRecordShapeContainer()
	{
		bGroupShape = false;

		m_pStream = NULL;

	}

	~CRecordShapeContainer()
	{
		m_pStream = NULL;
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_pStream = pStream;
		CRecordsContainer::ReadFromStream(oHeader, pStream);
	}


	CElementPtr GetElement (bool inGroup, CExMedia* pMapIDs,
							CTheme* pTheme, CLayout* pLayout, 
							CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide = NULL);

	NSPresentationEditor::ElementType GetTypeElem(SPT eType)
	{
		switch (eType)
		{
			//case sptMin:
		case sptMax:
		case sptNil:
			{
				return etShape;
			}
		case sptPictureFrame:
			{
				return etPicture;
			}
		default:
			{
				return etShape;
			}
		};
		return etShape;
	}
    AVSINLINE std::wstring GetFileName(std::wstring strFilePath)
	{
        int nIndex = strFilePath.rfind(wchar_t('\\'));
		if (-1 != nIndex)
		{
            return strFilePath.substr(nIndex + 1);
		}
		return strFilePath;
	}

protected:

	void ApplyThemeStyle(CElementPtr pElem, CTheme* pTheme, CRecordMasterTextPropAtom* master_levels);
	void SetUpTextStyle(std::wstring& strText, CTheme* pTheme, CLayout* pLayout, CElementPtr pElem, CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide, CRecordMasterTextPropAtom* master_levels);
	void ApplyHyperlink(CShapeElement* pShape, CColor& oColor);
};
