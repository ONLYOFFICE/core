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

#include "CrtMlFrt.h"

//0x0001 chainRecords = [XmlTkMaxFrt] [XmlTkMinFrt] [XmlTkLogBaseFrt]
//
//0x0002 chainRecords = [XmlTkStyle] [XmlTkThemeOverride] [XmlTkColorMappingOverride]
//
//0x0004 chainRecords = [XmlTkNoMultiLvlLbl] [XmlTkTickLabelSkipFrt] [XmlTkTickMarkSkipFrt] 
//[XmlTkMajorUnitFrt] [XmlTkMinorUnitFrt] [XmlTkTickLabelPositionFrt] [XmlTkBaseTimeUnitFrt] [XmlTkFormatCodeFrt] [XmlTkMinorUnitTypeFrt]
//
//0x0005 chainRecords = [XmlTkShowDLblsOverMax] [XmlTkBackWallThicknessFrt] [XmlTkFloorThicknessFrt] [XmlTkDispBlanksAsFrt] [SURFACE]
//
//SURFACE = XmlTkStartSurface [XmlTkFormatCodeFrt [XmlTkSpb]] [XmlTkTpb] XmlTkEndSurface
//
//0x000F chainRecords = [XmlTkOverlay]
//0x0013 chainRecords = [XmlTkSymbolFrt]
//0x0016 chainRecords = [XmlTkPieComboFrom12Frt]
//0x0019 chainRecords = [XmlTkOverlay]
//0x0037 chainRecords = [XmlTkRAngAxOffFrt] [XmlTkPerspectiveFrt] [XmlTkRotYFrt] [XmlTkRotXFrt] [XmlTkHeightPercent][XmlTkMajorUnitTypeFrt]
namespace XLS
{

CrtMlFrt::CrtMlFrt()
{
}


CrtMlFrt::~CrtMlFrt()
{
}


BaseObjectPtr CrtMlFrt::clone()
{
	return BaseObjectPtr(new CrtMlFrt(*this));
}

void CrtMlFrt::readFields(CFRecord& record)
{
	record >> frtHeader >> cbXmltkChain;

	//xmltkChain.cb = cbXmltkChain;
	//record >> xmltkChain;

	unsigned char recordVersion, unused;
	unsigned short xmltkParent;

	record >> recordVersion >> unused >> xmltkParent;
	
	record.skipNunBytes(cbXmltkChain - 4);

	record.skipNunBytes(4);
}

} // namespace XLS

