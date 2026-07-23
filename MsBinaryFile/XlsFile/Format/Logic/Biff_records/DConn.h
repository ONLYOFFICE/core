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

#include "TxtQry.h"
#include "../Biff_structures/FrtHeaderOld.h"
#include "../Biff_structures/DConnParameter.h"
#include "../Biff_structures/DConnId.h"
#include "../Biff_structures/DConnStringSequence.h"

namespace XLS
{

class DConn: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DConn)
	BASE_OBJECT_DEFINE_CLASS_NAME(DConn)
public:
	DConn();
	~DConn();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);
	void writeFields(CFRecord& record);

	static const ElementType type = typeDConn;

	FrtHeaderOld	frtHeaderOld;
	unsigned short	dbt = 0;
	bool			fSavePwd = false;
	bool			fTablesOnlyHtml = false;
	bool			fTableNames = false;
	bool			fDeleted = false;
	bool			fStandAlone = false;
	bool			fAlwaysUseConnectionFile = false;
	bool			fBackgroundQuery = false;
	bool			fRefreshOnLoad = false;
	bool			fSaveData = false;
	unsigned short	cParams = 0;
	bool			fMaintain = false;
	bool			fNewQuery = false;
	bool			fImportXmlSource = false;
	bool			fSPListSrc = false;
	bool			fSPListReinitCache = false;
	bool			fSrcIsXml = false;

	BiffStructurePtr	grbitDbt;
	unsigned char		bVerDbqueryEdit = 0;
	unsigned char		bVerDbqueryRefreshed = 0;
	unsigned char		bVerDbqueryRefreshableMin = 0;
	unsigned short		wRefreshInterval = 0;
	unsigned short		wHtmlFmt = 0;
	_UINT32				rcc = 0;
	unsigned char		credMethod = 0;

	XLUnicodeStringSegmented	rgchSourceDataFile;
	XLUnicodeStringSegmented	rgchSourceConnectionFile;
	XLUnicodeStringSegmented	rgchConnectionName;
	XLUnicodeStringSegmented	rgchConnectionDesc;
	XLUnicodeStringSegmented	rgchSSOApplicationID;
	XLUnicodeStringSegmented	tableNames;
	std::vector<DConnParameter>	params;

	BiffStructurePtr			connection;
	TxtQry						connection_txtQry;

	DConnStringSequence			rgbSQL;
	DConnStringSequence			rgbSQLSav;
	DConnStringSequence			rgbEditWebPage;
	DConnId						id;
};

} // namespace XLS

