/*
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

#include "BiffStructure.h"
#include "BorderFillInfo.h"

namespace XLS
{
class XFProp;
class ExtProp;
class CFRecord;
class Font;

class GlobalWorkbookInfo;
typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;

class StyleXF : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(StyleXF)
public:
	StyleXF(size_t& cell_xf_current_id, size_t& style_xf_current_id);
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);

	static const ElementType	type = typeStyleXF;

	int serialize(std::wostream & stream);

	unsigned char	alc;
	bool			fWrap;
	unsigned char	alcV;
	bool			fJustLast;
	unsigned short	trot;
	unsigned char	cIndent;
	bool			fShrinkToFit;
	unsigned char	iReadOrder;

//------------------------------------------------------------------
	GlobalWorkbookInfoPtr global_info;
	
	BorderInfo		border;
	FillInfo		fill;
	FillInfoExt		font_color;
	size_t			font_id;
	Font			*font;

	BiffStructurePtrVector ext_props;
	BiffStructurePtrVector xf_props;

	void Update(ExtProp* extProp); // xls style
	void Update(XFProp* xfProp); //xlsx style

	void RegisterFillBorder();

	size_t border_x_id;
	size_t fill_x_id;

	size_t& cell_xf_current_id_;
	size_t& style_xf_current_id_;
};

} // namespace XLS

