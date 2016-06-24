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

#include "OfficeArtRecord.h"
#include "../BitMarkedStructs.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class OfficeArtClientAnchorSheet : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtClientAnchorSheet)
public:
	OfficeArtClientAnchorSheet();
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtClientAnchorSheet;
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	bool fMove;
	bool fSize;

	XLS::Col256U	colL;
	short			dxL;
	XLS::RwU		rwT;
	short			dyT;
	XLS::Col256U	colR;
	short			dxR;
	XLS::RwU		rwB;
	short			dyB;

//-----------------------------------------------------------------------------
	XLS::GlobalWorkbookInfoPtr global_info;

	void calculate();

	_UINT32		_dxL;
	_UINT32		_dyT;
	_UINT32		_dxR;
	_UINT32		_dyB;

	int _x;
	int _y;
	int _cx;
	int _cy;
};

typedef boost::shared_ptr<OfficeArtClientAnchorSheet> OfficeArtClientAnchorSheetPtr;

class OfficeArtChildAnchor : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtChildAnchor)
public:
	OfficeArtChildAnchor();
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtChildAnchor;
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record){}

	_INT32 dxL;
	_INT32 dyT;
	_INT32 dxR;
	_INT32 dyB;

	int _x;
	int _y;
	int _cx;
	int _cy;
};

typedef boost::shared_ptr<OfficeArtChildAnchor> OfficeArtChildAnchorPtr;
} // namespace XLS
