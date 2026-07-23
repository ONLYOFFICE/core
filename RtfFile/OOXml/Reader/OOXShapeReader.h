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

#include "OOXReaderBasic.h"

#include "../../Format/RtfDocument.h"
#include "../../Format/RtfShape.h"

#include "../../../OOXML/DocxFormat/Logic/Vml.h"
#include "../../../OOXML/PPTXFormat/Logic/SpTree.h"

namespace OOX
{
	namespace Logic
	{
		class CBackground;
	}
}
namespace PPTX
{
	namespace Logic
	{
		class SchemeClr;
	}
}
class OOXShapeReader
{
public: 
	OOXShapeReader(OOX::Vml::CVmlCommonElements * vmlElem);
	OOXShapeReader(OOX::WritingElementWithChilds<OOX::WritingElement> * elem);
	OOXShapeReader(OOX::WritingElement* ooxShape);

	static bool WriteDataToPicture( std::wstring sPath, RtfPicture& pOutput, ReaderParameter& oParam );

	bool Parse			( ReaderParameter oParam , RtfShapePtr& oOutput, bool bUsedType = false);
	bool ParseVml		( ReaderParameter oParam , RtfShapePtr& oOutput, bool bUsedType = false);
	bool ParseVmlChild	( ReaderParameter oParam , RtfShapePtr& oOutput);
	bool ParseVmlObject	( ReaderParameter oParam , RtfShapePtr& oOutput);
	
	static bool Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::BlipFill *oox_bitmap_fill);
	static void Parse(ReaderParameter oParam, PPTX::Logic::UniFill *fill, unsigned long & nColor, _CP_OPT(double) &opacity);
	static void Parse(ReaderParameter oParam, PPTX::Logic::ColorBase *oox_color, unsigned long& nColor, _CP_OPT(double) &opacity);
	
	static bool ParseVmlStyle(RtfShapePtr pShape, SimpleTypes::Vml::CCssProperty* prop);

private:
	void ParseVmlPath(RtfShapePtr& pShape, const std::wstring &custom_path);
	bool ParseVmlStyles(RtfShapePtr& pShape, std::vector<SimpleTypes::Vml::CCssPropertyPtr> & props);

	void ParseAdjustment(RtfShape& oShape, std::wstring sAdjustment);
	bool ParseShape( ReaderParameter oParam , RtfShapePtr& oOutput);

	void ConvertOle2ToOle1(POLE::Storage *storage, RtfOlePtr object);
	void ConvertOle2ToOle1(const std::wstring &oleFilePath, RtfOlePtr object);
	std::wstring ConvertPackageToStorage(const std::wstring & msPackage, const std::wstring & Program);

	bool ParsePic( ReaderParameter oParam , RtfShapePtr& oOutput);

	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::BodyPr *text_properties);
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::Xfrm *xfrm);
	
	void Parse(ReaderParameter oParam, PPTX::Logic::SolidFill *oox_solid_fill, unsigned long & nColor, _CP_OPT(double) &opacity);
	
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::StyleRef	*style_ref, int type);
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::Ln		*oox_line_prop, PPTX::Logic::SchemeClr* change_color = NULL);

	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::UniFill	*oox_fill, PPTX::Logic::SchemeClr* change_color = NULL);
	
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::GradFill	*oox_grad_fill, PPTX::Logic::SchemeClr* change_color = NULL);
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::PattFill	*oox_pattern_fill, PPTX::Logic::SchemeClr* change_color = NULL);
	void Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::SolidFill	*oox_solid_fill, PPTX::Logic::SchemeClr* change_color = NULL);
//---------------------------------------------------------------------------
	OOX::Vml::CVmlCommonElements						*m_vmlElement = NULL;
	OOX::WritingElementWithChilds<OOX::WritingElement>  *m_arrElement = NULL;

	OOX::WritingElement									*m_ooxShape = NULL;
};

class OOXShapeGroupReader
{
private:
	OOX::Vml::CGroup		*m_vmlGroup = NULL;
	PPTX::Logic::SpTree		*m_ooxGroup = NULL;

public: 
	OOXShapeGroupReader(OOX::Vml::CGroup *vmlGroup);
	OOXShapeGroupReader(PPTX::Logic::SpTree *ooxGroup);
	bool ParseVmlStyles(RtfShapePtr pGroupShape, std::vector<SimpleTypes::Vml::CCssPropertyPtr> & props);

	bool Parse( ReaderParameter oParam , RtfShapePtr& oOutput);
};

class OOXBackgroundReader
{
private:
	OOX::Logic::CBackground *m_ooxBackground = NULL;

public: 
	OOXBackgroundReader(OOX::Logic::CBackground *oox_background);

	bool Parse( ReaderParameter oParam , RtfShapePtr& oOutput);
};
