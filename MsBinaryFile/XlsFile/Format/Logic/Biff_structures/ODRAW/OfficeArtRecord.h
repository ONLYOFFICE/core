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
