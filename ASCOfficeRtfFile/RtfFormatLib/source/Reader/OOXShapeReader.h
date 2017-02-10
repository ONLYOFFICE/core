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
#pragma once
#include "OOXPictureGraphicReader.h"
#include "OOXReaderBasic.h"

#include "../RtfDocument.h"
#include "../RtfShape.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Logic/Vml.h"
#include "../../../../Common/DocxFormat/Source/DocxFormat/Logic/Shape.h"

bool ParseStyle(RtfShapePtr pShape, SimpleTypes::Vml::CCssProperty* prop);

class OOXShapeReader
{
public: 
	OOXShapeReader(OOX::Vml::CVmlCommonElements * vmlElem)
	{
		m_ooxShape	 = NULL;
		
		m_vmlElement = vmlElem;
		m_arrElement = vmlElem;
	}
	OOXShapeReader(OOX::WritingElementWithChilds<OOX::WritingElement> * elem);
	OOXShapeReader(OOX::Logic::CShape * ooxShape)
	{
		m_ooxShape	 = ooxShape;
		m_vmlElement = NULL;
		m_arrElement = NULL;
	}

	static bool WriteDataToPicture( std::wstring sPath, RtfPicture& pOutput, std::wstring sTempPath = L"" );

	bool Parse			( ReaderParameter oParam , RtfShapePtr& oOutput);
	bool ParseVmlChild	( ReaderParameter oParam , RtfShapePtr& oOutput);
	bool ParseVml		( ReaderParameter oParam , RtfShapePtr& oOutput);
	
	void ParseAdjustment(RtfShape& oShape, std::wstring sAdjustment);

	static bool Parse(ReaderParameter oParam, RtfShapePtr& pOutput, OOX::Drawing::CBlipFillProperties *oox_bitmap_fill);
private:

	bool Parse(ReaderParameter oParam, int indexSchemeColor, BYTE& ucA, BYTE& ucG, BYTE& ucB, BYTE& ucR);
	void Parse(ReaderParameter oParam, OOX::Drawing::CColor						*oox_color,			unsigned int & nColor, _CP_OPT(double) &opacity);
	void Parse(ReaderParameter oParam, OOX::Drawing::CSchemeColor				*oox_ShemeClr,		unsigned int & nColor, _CP_OPT(double) &opacity);
	void Parse(ReaderParameter oParam, OOX::Drawing::Colors::CColorTransform	*oox_ScrgbClr,		unsigned int & nColor, _CP_OPT(double) &opacity);
	void Parse(ReaderParameter oParam, OOX::Drawing::CSolidColorFillProperties	*oox_solid_fill,	unsigned int & nColor, _CP_OPT(double) &opacity);
	
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, OOX::Drawing::CStyleMatrixReference		*style_matrix_ref);
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, OOX::Drawing::CLineProperties				*oox_line_prop,		std::wstring *change_sheme_color = NULL);
    
    void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, OOX::Drawing::CGradientFillProperties		*oox_grad_fill,		std::wstring *change_sheme_color = NULL);
    void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, OOX::Drawing::CPatternFillProperties		*oox_pattern_fill,	std::wstring *change_sheme_color = NULL);
    void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, OOX::Drawing::CSolidColorFillProperties	*oox_solid_fill,	std::wstring *change_sheme_color = NULL);
//---------------------------------------------------------------------------
	OOX::Vml::CVmlCommonElements						*m_vmlElement;
	OOX::WritingElementWithChilds<OOX::WritingElement>  *m_arrElement;

	OOX::Logic::CShape									*m_ooxShape;

	bool ParseStyles(RtfShapePtr pShape, std::vector<SimpleTypes::Vml::CCssPropertyPtr> & props)
	{
		for (size_t i=0; i< props.size(); i++)
		{
			ParseStyle( pShape, props[i].get());
		}
		return true;
	}

};

class OOXShapeGroupReader
{
private:
	OOX::Vml::CGroup		*m_vmlGroup;
	OOX::Logic::CGroupShape	*m_ooxGroup;
public: 
	OOXShapeGroupReader(OOX::Vml::CGroup *vmlGroup)
	{
		m_ooxGroup = NULL;
		m_vmlGroup = vmlGroup;
	}
	OOXShapeGroupReader(OOX::Logic::CGroupShape *ooxGroup)
	{
		m_vmlGroup = NULL;
		m_ooxGroup = ooxGroup;
	}
	bool ParseStyles(RtfShapePtr pGroupShape, std::vector<SimpleTypes::Vml::CCssPropertyPtr> & props)
	{
		for (size_t i = 0; i < props.size(); i++)
		{
			ParseStyle( pGroupShape, props[i].get());
		}
		return true;
	}

	bool Parse( ReaderParameter oParam , RtfShapePtr& oOutput);
};

class OOXBackgroundReader
{
private:
	OOX::Logic::CBackground *m_ooxBackground;
public: 
	OOXBackgroundReader(OOX::Logic::CBackground *oox_background)
	{
		m_ooxBackground = oox_background;
	}

	bool Parse( ReaderParameter oParam , RtfShapePtr& oOutput);
};
