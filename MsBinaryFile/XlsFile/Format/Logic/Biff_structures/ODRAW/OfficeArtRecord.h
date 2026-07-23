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

#include "../BiffStructure.h"
#include "OfficeArtRecordHeader.h"

namespace XLS
{
	class CFRecord;
	
	class GlobalWorkbookInfo;
	typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;
}


namespace ODRAW
{

class OfficeArtRecord : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtRecord)
public:
	OfficeArtRecord(const unsigned char recVer, const unsigned short recType);

	virtual void load(XLS::CFRecord& record);

	bool IsThisHeaderMine(const OfficeArtRecordHeader& rh);

	// For overriding:
	virtual const unsigned short GetInstanceToStore();
	virtual void loadFields(XLS::CFRecord& record) = 0;

	static const XLS::ElementType	type = XLS::typeOfficeArtRecord;
	OfficeArtRecordHeader rh_own;

	enum OfficeArtRecordType
	{
		DggContainer	= 0xF000,
		BStoreContainer	= 0xF001, 
		DgContainer		= 0xF002, 
		SpgrContainer	= 0xF003,
		SpContainer		= 0xF004,
		SolverContainer	= 0xF005,
		FDGGBlock		= 0xF006,
		FBSE			= 0xF007, 
		FDG				= 0xF008, 
		FSPGR			= 0xF009,
		FSP				= 0xF00A, 
		FOPT			= 0xF00B, 
		ClientTextbox	= 0xF00D, 
		ChildAnchor		= 0xF00F, 
		ClientAnchor	= 0xF010, 
		ClientData		= 0xF011, 
		FConnectorRule	= 0xF012,
		FArcRule		= 0xF014,
		FCalloutRule	= 0xF017,
		BlipEMF			= 0xF01A,
		BlipWMF			= 0xF01B,
		BlipPICT		= 0xF01C,
		BlipJPEG		= 0xF01D,
		BlipPNG			= 0xF01E,
		BlipDIB			= 0xF01F,
		BlipTIFF		= 0xF020,
		BlipJPEG_clone	= 0xF02A,
		FRITContainer			= 0xF118, 
		FDGSL					= 0xF119,
		ColorMRUContainer		= 0xF11A,
		FPSPL					= 0xF11D,
		SplitMenuColorContainer	= 0xF11E,
		SecondaryFOPT			= 0xF121,
		TertiaryFOPT			= 0xF122, 


	};
	
	enum OfficeArtClientAnchorType
	{
		CA_Sheet,
		CA_Chart,
		CA_HF
	};

	XLS::GlobalWorkbookInfoPtr	m_pGlobalWorkbookInfoPtr;
};

typedef boost::shared_ptr<OfficeArtRecord> OfficeArtRecordPtr;


} // namespace ODRAW
