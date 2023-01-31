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

#include "BiffRecordContinued.h"
#include "MsoDrawing.h"

#include "../Biff_structures/FtCmo.h"
#include "../Biff_structures/FtCf.h"
#include "../Biff_structures/FtPioGrbit.h"
#include "../Biff_structures/FtSbs.h"
#include "../Biff_structures/FtNts.h"
#include "../Biff_structures/FtMacro.h"
#include "../Biff_structures/FtPictFmla.h"
#include "../Biff_structures/ObjLinkFmla.h"
#include "../Biff_structures/FtCblsData.h"
#include "../Biff_structures/FtRboData.h"
#include "../Biff_structures/FtEdoData.h"
#include "../Biff_structures/FtLbsData.h"
#include "../Biff_structures/FtGboData.h"

namespace XLS
{

class Obj : public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Obj)
	BASE_OBJECT_DEFINE_CLASS_NAME(Obj)
public:
	Obj(MsoDrawingPtr mso_drawing)  : mso_drawing_(mso_drawing)
	{
		old_version.enabled = false;
	}
	~Obj();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);

	static const ElementType type = typeObj;

//-----------------------------
	FtCmo			cmo;
	
	FtCf			pictFormat;

	FtPioGrbit		pictFlags;
	FtSbs			sbs;
	FtNts			nts;
	FtMacro			macro;
	FtPictFmla		pictFmla;
	ObjLinkFmla		linkFmla;
	FtCblsData		checkBox;
	FtRboData		radioButton;
	FtEdoData		edit;
	FtLbsData		list;
	FtGboData		gbo;

	MsoDrawingPtr	mso_drawing_;

	struct _
	{
		bool						enabled = false;
		ODRAW::OfficeArtRecordPtr	anchor;
		std::vector<BaseObjectPtr>	additional;
		bool						bFill = false;
		ShortXLAnsiString			name;

		_UINT32						fill = 0;//(icvBack, icvFore, fls, fAuto)
		_UINT32						line = 0;//(icv, lns, lnw, fAuto)
		_UINT16						flag = 0;
		_UINT16						flag2 = 0;
	}old_version;
};

} // namespace XLS

