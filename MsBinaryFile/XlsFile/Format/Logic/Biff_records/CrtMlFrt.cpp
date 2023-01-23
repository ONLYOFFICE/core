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

