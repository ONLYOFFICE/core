/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "../DesktopEditor/common/Types.h"

#define USING_DELETE_DUPLICATING_CONTS 0 // 0 - все сточки-дубликаты превращаются в shape, 1 - строчки дубликаты удаляются

const double c_dDpiX		= 72.0;
const double c_dDpiY		= 72.0;

const double c_dInchToMM	= 25.4;
constexpr double c_dPixToMM	= 25.4 / 72.0;
constexpr double c_dPtToMM  = 25.4 / 72.0;
constexpr double c_dMMToPt	= 72.0 / 25.4;
constexpr double c_dMMToDx	= 72 * 20 / 25.4;
const double c_dMMToEMU	     = 36000.0;
const double c_dInchToEMU	= 914400.0;
const double c_dPtToEMU  	= 12700.0;
const double c_dDegreeToAngle = 60000.0;

const double c_dSTANDART_STRING_HEIGHT_MM       = 4.2333333333333334;
const double c_dTHE_SAME_STRING_Y_PRECISION_MM  = 0.01;
const double c_dLINE_DISTANCE_ERROR_MM          = 0.5;
const double c_dERROR_OF_RIGHT_BORDERS_MM       = 0.5;
const double c_dERROR_OF_LEFT_BORDERS_MM        = 0.1;
const double c_dCENTER_POSITION_ERROR_MM        = 1.5;
const double c_dTHE_STRING_X_PRECISION_MM       = 0.5;
const double c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM = 0.1;
const double c_dGRAPHICS_ERROR_MM               = 0.5;
const double c_dGRAPHICS_ERROR_IN_LINES_MM      = 0.3;
const double c_dMAX_LINE_HEIGHT_MM              = 2.5;
const double c_dCORRECTION_FOR_FIRST_PARAGRAPH  = -1.5;

const UINT   c_iWhiteColor = 0xFFFFFF;
const UINT   c_iBlackColor = 0x000000;
const UINT   c_iBlackColor2 = 0x111111;
const UINT   c_iGreyColor  = 0xC0C0C0;
const UINT   c_iGreyColor2 = 0x999999;

const double c_dSTANDART_LEFT_INDENT_MM = 30;
const double c_dSTANDART_RIGHT_INDENT_MM = 15;
const double c_dSTANDART_FIRSTLINE_INDENT_MM = 12.5;

const UINT   c_iStartingIdForImages = 6;
constexpr size_t c_nAntiZero = ~0;
const UINT   c_iStandartRelativeHeight = 0x0400;
