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

	static const ElementType type = typeDConn;

	FrtHeaderOld	frtHeaderOld;
	unsigned short	dbt;
	bool			fSavePwd;
	bool			fTablesOnlyHtml;
	bool			fTableNames;
	bool			fDeleted;
	bool			fStandAlone;
	bool			fAlwaysUseConnectionFile;
	bool			fBackgroundQuery;
	bool			fRefreshOnLoad;
	bool			fSaveData;
	unsigned short	cParams;
	bool			fMaintain;
	bool			fNewQuery;
	bool			fImportXmlSource;
	bool			fSPListSrc;
	bool			fSPListReinitCache;
	bool			fSrcIsXml;

	BiffStructurePtr	grbitDbt;
	unsigned char		bVerDbqueryEdit;
	unsigned char		bVerDbqueryRefreshed;
	unsigned char		bVerDbqueryRefreshableMin;
	unsigned short		wRefreshInterval;
	unsigned short		wHtmlFmt;
	_UINT32				rcc;
	unsigned char		credMethod;

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

