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
#include "OOXReaderBasic.h"

#include "../RtfDocument.h"
#include "../RtfShape.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Logic/Vml.h"

bool ParseVmlStyle(RtfShapePtr pShape, SimpleTypes::Vml::CCssProperty* prop);

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
	OOXShapeReader(OOX::WritingElement* ooxShape)
	{
		m_ooxShape	 = ooxShape;
		m_vmlElement = NULL;
		m_arrElement = NULL;
	}

	static bool WriteDataToPicture( std::wstring sPath, RtfPicture& pOutput, ReaderParameter& oParam );

	bool Parse			( ReaderParameter oParam , RtfShapePtr& oOutput, bool bUsedType = false);
	bool ParseVml		( ReaderParameter oParam , RtfShapePtr& oOutput, bool bUsedType = false);
	bool ParseVmlChild	( ReaderParameter oParam , RtfShapePtr& oOutput);
	bool ParseVmlObject	( ReaderParameter oParam , RtfShapePtr& oOutput);
	
	static bool ParseVmlStyle(RtfShapePtr pShape, SimpleTypes::Vml::CCssProperty* prop);
	
	void ParseAdjustment(RtfShape& oShape, std::wstring sAdjustment);

	static bool Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::BlipFill *oox_bitmap_fill);
	static void Parse(ReaderParameter oParam, PPTX::Logic::UniFill *fill, unsigned int & nColor, _CP_OPT(double) &opacity);
	static void Parse(ReaderParameter oParam, PPTX::Logic::ColorBase *oox_color, unsigned int & nColor, _CP_OPT(double) &opacity);
private:

	bool ParseShape( ReaderParameter oParam , RtfShapePtr& oOutput);
	bool ParsePic( ReaderParameter oParam , RtfShapePtr& oOutput);

	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::BodyPr *text_properties);
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::Xfrm *xfrm);
	
	void Parse(ReaderParameter oParam, PPTX::Logic::SolidFill	*oox_solid_fill,	unsigned int & nColor, _CP_OPT(double) &opacity);
	
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::StyleRef	*style_ref, int type);
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::Ln		*oox_line_prop,		std::wstring *change_sheme_color = NULL);

	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::UniFill	*oox_fill,			std::wstring *change_sheme_color = NULL);
	
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::GradFill	*oox_grad_fill,		std::wstring *change_sheme_color = NULL);
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::PattFill	*oox_pattern_fill,	std::wstring *change_sheme_color = NULL);
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::SolidFill	*oox_solid_fill,	std::wstring *change_sheme_color = NULL);
//---------------------------------------------------------------------------
	OOX::Vml::CVmlCommonElements						*m_vmlElement;
	OOX::WritingElementWithChilds<OOX::WritingElement>  *m_arrElement;

	OOX::WritingElement									*m_ooxShape;

	void ParseVmlPath	(RtfShapePtr& pShape, const std::wstring &custom_path);
	bool ParseVmlStyles	(RtfShapePtr& pShape, std::vector<SimpleTypes::Vml::CCssPropertyPtr> & props);

	void ConvertOle2ToOle1(POLE::Storage *storage, RtfOlePtr object);
	void ConvertOle2ToOle1(const std::wstring &oleFilePath, RtfOlePtr object);
};

class OOXShapeGroupReader
{
private:
	OOX::Vml::CGroup		*m_vmlGroup;
	PPTX::Logic::SpTree		*m_ooxGroup;
public: 
	OOXShapeGroupReader(OOX::Vml::CGroup *vmlGroup)
	{
		m_ooxGroup = NULL;
		m_vmlGroup = vmlGroup;
	}
	OOXShapeGroupReader(PPTX::Logic::SpTree *ooxGroup)
	{
		m_vmlGroup = NULL;
		m_ooxGroup = ooxGroup;
	}
	bool ParseVmlStyles(RtfShapePtr pGroupShape, std::vector<SimpleTypes::Vml::CCssPropertyPtr> & props)
	{
		for (size_t i = 0; i < props.size(); i++)
		{
			OOXShapeReader::ParseVmlStyle( pGroupShape, props[i].get());
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
