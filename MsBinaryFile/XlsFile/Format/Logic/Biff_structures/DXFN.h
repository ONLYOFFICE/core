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

#include "BiffStructure.h"

#include "DXFNum.h"
#include "DXFFntD.h"
#include "DXFALC.h"
#include "DXFBdr.h"
#include "DXFPat.h"
#include "DXFProt.h"
#include "XFExtNoFRT.h"

#include "../../../../Common/Utils/simple_xml_writer.h"

namespace XLS
{

class CFRecord;

class DXFN : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFN)
public:
	BiffStructurePtr clone();

	static const ElementType type = typeDXFN;
	
	virtual void load(CFRecord& record);

	int serialize(std::wostream & stream);

	bool alchNinch;
	bool alcvNinch;
	
	bool wrapNinch;
	bool trotNinch;
	bool kintoNinch;
	bool cIndentNinch;
	bool fShrinkNinch;
	bool fMergeCellNinch;
	
	bool lockedNinch;
	bool hiddenNinch;
	
	bool glLeftNinch;
	bool glRightNinch;
	bool glTopNinch;
	bool glBottomNinch;
	bool glDiagDownNinch;
	bool glDiagUpNinch;
	
	bool flsNinch;
	bool icvFNinch;
	bool icvBNinch;
	bool ifmtNinch;
	bool fIfntNinch;
	
	bool ibitAtrNum;
	bool ibitAtrFnt;
	bool ibitAtrAlc;
	bool ibitAtrBdr;
	bool ibitAtrPat;
	bool ibitAtrProt;
	
	bool iReadingOrderNinch;
	bool fIfmtUser;
	bool fNewBorder;
	bool fZeroInited;

	DXFNum	dxfnum;
	DXFFntD dxffntd;
	DXFALC	dxfalc;
	DXFBdr	dxfbdr;
	DXFPat	dxfpat;
	DXFProt dxfprot;

	XFExtNoFRTPtr	xfext;
};

typedef boost::shared_ptr<DXFN> DXFNPtr;

} // namespace XLS

