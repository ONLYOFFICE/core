/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

class CFRecord;
class GlobalWorkbookInfo;
typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;

class CellXF : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CellXF)
public:
	CellXF(size_t& cell_xf_current_id, size_t& style_xf_current_id);

	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeCellXF;

	GlobalWorkbookInfoPtr m_GlobalWorkbookInfo;
	
	unsigned char	alc;
	bool			fWrap;
	unsigned char	alcV;
	bool			fJustLast;
	unsigned char	trot;
	unsigned char	cIndent;
	bool			fShrinkToFit;
	unsigned char	iReadOrder;
	
	bool			fAtrNum;
	bool			fAtrFnt;
	bool			fAtrAlc;
	bool			fAtrBdr;
	bool			fAtrPat;
	bool			fAtrProt;

	BorderInfo border;
	FillInfo fill;

	BiffStructurePtrVector ext_props;

	FillInfoExt		font_color;
	size_t			font_id;

	bool			fHasXFExt;
	bool			fsxButton;

	size_t border_x_id;
	size_t fill_x_id;

	size_t& cell_xf_current_id_;
	size_t& style_xf_current_id_;

	void RegisterFillBorder();
};

} // namespace XLS

