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

#include "OfficeArtContainer.h"

namespace ODRAW
{

class OfficeArtDggContainer : public OfficeArtContainer
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtDggContainer)
public:
	OfficeArtDggContainer(const OfficeArtClientAnchorType anchor_type) : OfficeArtContainer(0x0F, DggContainer, anchor_type) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtDggContainer(*this)); }

	static const XLS::ElementType type = XLS::typeOfficeArtDggContainer;

	void loadFields(XLS::CFRecord& record);
	void save(XLS::CFRecord& record);

	OfficeArtRecordPtr		m_OfficeArtBStoreContainer;
	OfficeArtRecordPtr		m_OfficeArtColorMRUContainer;
	OfficeArtRecordPtr		m_OfficeArtSplitMenuColorContainer;
	OfficeArtRecordPtr		m_OfficeArtFDGGBlock;

	//+ OfficeArtFOPT + OfficeArtTertiaryFOPT
};
typedef boost::shared_ptr<OfficeArtDggContainer> OfficeArtDggContainerPtr;

class OfficeArtSpgrContainer : public OfficeArtContainer
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtSpgrContainer)
public:
	OfficeArtSpgrContainer(const OfficeArtClientAnchorType anchor_type) : OfficeArtContainer(0x0F, SpgrContainer, anchor_type) {}
	XLS::BiffStructurePtr clone()override { return XLS::BiffStructurePtr(new OfficeArtSpgrContainer(*this)); }

	void loadFields(XLS::CFRecord& record) override;
	void save(XLS::CFRecord& record) override;

	static const XLS::ElementType type = XLS::typeOfficeArtSpgrContainer;

	std::vector<OfficeArtContainerPtr> m_OfficeArtSpgrContainerFileBlock;
};

class OfficeArtSolverContainer : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtSolverContainer)
public:
	OfficeArtSolverContainer() : OfficeArtRecord(0x0F, SolverContainer) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtSolverContainer(*this)); }

	void loadFields(XLS::CFRecord& record);
};

class OfficeArtSpContainer : public OfficeArtContainer
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtSpContainer)
public:
	OfficeArtSpContainer(const OfficeArtClientAnchorType anchor_type) : OfficeArtContainer(0x0F, SpContainer, anchor_type) {}
	XLS::BiffStructurePtr clone()override { return XLS::BiffStructurePtr(new OfficeArtSpContainer(*this)); }

	static const XLS::ElementType	type = XLS::typeOfficeArtSpContainer;

	void loadFields(XLS::CFRecord& record)override;
	void save(XLS::CFRecord& record) override;

	OfficeArtRecordPtr m_OfficeArtFSPGR;
	OfficeArtRecordPtr m_OfficeArtFSP;
	OfficeArtRecordPtr m_OfficeArtAnchor;
	OfficeArtRecordPtr m_oOfficeArtFOPT;
	OfficeArtRecordPtr m_oOfficeArtClientData;
	unsigned int extraSize = 0;
};

class OfficeArtClientData : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtClientData)
public:
	OfficeArtClientData() : OfficeArtRecord(0, ClientData) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtClientData(*this)); }
	
	void loadFields(XLS::CFRecord& record);
	void save(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeOfficeArtClientData;

};

class OfficeArtFConnectorRule : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFConnectorRule)
public:
	OfficeArtFConnectorRule() : OfficeArtRecord(0, FConnectorRule) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtFConnectorRule(*this)); }
	
	virtual void loadFields(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeOfficeArtClientData;//FConnectorRule

	_UINT32 ruid;
	_UINT32 spidA;
	_UINT32 spidB;
	_UINT32 spidC;
	_UINT32 cptiA;
	_UINT32 cptiB;
};

class OfficeArtFCalloutRule : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFCalloutRule)
public:
	OfficeArtFCalloutRule() : OfficeArtRecord(0, FCalloutRule) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtFCalloutRule(*this)); }
	
	virtual void loadFields(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeOfficeArtClientData;//FCalloutRule

	_UINT32 ruid;
	_UINT32 spid;
};

class OfficeArtFArcRule : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFArcRule)
public:
	OfficeArtFArcRule() : OfficeArtRecord(0, FArcRule) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtFArcRule(*this)); }
	
	virtual void loadFields(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeOfficeArtClientData;//FArcRule

	_UINT32 ruid;
	_UINT32 spid;
};

class OfficeArtClientTextbox : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtClientTextbox)
public:
	OfficeArtClientTextbox() : OfficeArtRecord(0, ClientTextbox) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtClientTextbox(*this)); }
	
	void loadFields(XLS::CFRecord& record);
	void save(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeOfficeArtClientTextbox;
};

typedef boost::shared_ptr<OfficeArtSpgrContainer> OfficeArtSpgrContainerPtr;
typedef boost::shared_ptr<OfficeArtSpContainer> OfficeArtSpContainerPtr;
typedef boost::shared_ptr<OfficeArtClientData> OfficeArtClientDataPtr;
typedef boost::shared_ptr<OfficeArtClientTextbox> OfficeArtClientTextboxPtr;


} // namespace XLS
