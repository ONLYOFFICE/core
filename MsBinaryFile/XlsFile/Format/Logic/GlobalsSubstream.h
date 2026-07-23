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

#include "CompositeObject.h"

namespace XLS
{;

class StreamCacheReader;
class BOF;

class GlobalsSubstream;
typedef boost::shared_ptr<GlobalsSubstream>		GlobalsSubstreamPtr;

class GlobalsSubstream: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(GlobalsSubstream)
public:
	GlobalsSubstream(const unsigned short code_page);
	~GlobalsSubstream();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);
    virtual const bool saveContent(BinProcessor& proc);

	static const ElementType type = typeGlobalsSubstream;

	int serialize_format(std::wostream & _stream);
	int serialize_protection(std::wostream & _stream);
	
	BaseObjectPtr					m_FileSharing;
	BaseObjectPtr					m_THEME;
	BaseObjectPtr					m_Formating;
	BaseObjectPtr					m_Template;
	BaseObjectPtr					m_SHAREDSTRINGS;
	BaseObjectPtr					m_ExtSST;
	BaseObjectPtr					m_CodePage;
	BaseObjectPtr					m_Country;
	BaseObjectPtr					m_WriteProtect;
	BaseObjectPtr					m_PROTECTION;
	BaseObjectPtr					m_RRTabId;
	BaseObjectPtr					m_FNGROUPS;
	BaseObjectPtr					m_BookExt;
	BaseObjectPtr					m_CodeName;
	BaseObjectPtr					m_METADATA;
	BaseObjectPtr					m_MTRSettings;
	BaseObjectPtr					m_Date1904;
	BaseObjectPtr					m_CalcPrecision;

	std::vector<BaseObjectPtr>		m_arBIGNAME;
	std::vector<BaseObjectPtr>		m_arHFPicture;
	std::vector<BaseObjectPtr>		m_arLBL;
	std::vector<BaseObjectPtr>		m_arMSODRAWINGGROUP;
	std::vector<BaseObjectPtr>		m_arWindow1;
	std::vector<BaseObjectPtr>		m_arUserBView;
	std::vector<BaseObjectPtr>		m_arBUNDLESHEET;
	std::vector<BaseObjectPtr>		m_arSUPBOOK;
	std::vector<BaseObjectPtr>		m_arPIVOTCACHEDEFINITION;
	std::vector<BaseObjectPtr>		m_arDConn;

	std::vector<BiffStructurePtr>	m_arHFPictureDrawing; // here OfficeArtDggContainer - images ...
	
	unsigned short					code_page_;
	GlobalWorkbookInfoPtr			global_info_;

	void LoadHFPicture();
	void UpdateXti();
	void UpdateXFC();
	void UpdateDefineNames();
	void UpdateExternalDefineNames();
};

} // namespace XLS
