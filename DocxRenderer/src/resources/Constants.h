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
#include <type_traits>
#include <cstdlib>
#include <limits>

#include "../../../DesktopEditor/common/Types.h"


#define USING_DELETE_DUPLICATING_CONTS 0 // 0 - all duplicate lines become shapes, 1 - duplicate lines are deleted
// #define USE_DEFAULT_FONT_TO_RECALC

const double c_dCOMPARE_EPSILON = 0.1;

const double c_dDpiX = 72.0;
const double c_dDpiY = 72.0;

constexpr double c_dMMToPix       = 72.0 / 25.4;
constexpr double c_dPixToMM       = 25.4 / 72.0;
constexpr double c_dMMToPt        = 72.0 / 25.4;
constexpr double c_dPtToMM        = 25.4 / 72.0;
constexpr double c_dInchToMM      = 25.4;
constexpr double c_dMMToDx        = 72 * 20 / 25.4;
constexpr double c_dMMToEMU       = 36000.0;
constexpr double c_dInchToEMU     = 914400.0;
constexpr double c_dPtToEMU       = 12700.0;
constexpr double c_dDegreeToAngle = 60000.0;

const double c_dSTANDART_STRING_HEIGHT_MM       = 4.2333333333333334;
const double c_dTHE_SAME_STRING_Y_PRECISION_MM  = 0.015;
const double c_dTHE_SAME_STRING_X_PRECISION_MM  = 0.03;
const double c_dTHE_SAME_SPACING_ERROR          = 0.1;
const double c_dLINE_DISTANCE_ERROR_MM          = 0.3;
const double c_dOVERLAP_TEXT_LINE_ERROR_MM      = 0.7;
const double c_dERROR_OF_PARAGRAPH_BORDERS_MM   = 1.0;
const double c_dCENTER_POSITION_ERROR_MM        = 1.5;
const double c_dTHE_STRING_X_PRECISION_MM       = 0.5;
const double c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM = 0.1;
const double c_dGRAPHICS_ERROR_MM               = 1.0;
const double c_dMAX_TABLE_LINE_WIDTH_MM         = 2.2;
const double c_dMAX_TABLE_CELL_DIFF_MM          = 7.0;
const double c_dMIN_TABLE_DIFF_MM               = 0.3;
const double c_dGRAPHICS_ERROR_IN_LINES_MM      = 0.3;
const double c_dMAX_LINE_HEIGHT_MM              = 2.5;
const double c_dMAX_LINE_WITH_TEXT_ERROR_MM     = 2.5;
const double c_dCELL_GROUP_SPLIT_MM             = 12.0;
const double c_dLINE_GROUP_SPLIT_MM             = 12.0;
const double c_dCORRECTION_FIRST_PARAGRAPH_MM   = -1.5;
const double c_dLINE_DISTANCE_MAX_MM            = 50.0;
const double c_dSHAPE_TROUGH_MAX_MM             = 80.0;
const double c_dLINE_SPLIT_DISTANCE_MM          = 10.0;
const double c_dSHAPE_X_OFFSET_MM               = 1.5;
const double c_dAVERAGE_SPACE_WIDTH_COEF        = 0.9;
const double c_dSPACE_WIDTH_COEF                = 0.39;
const double c_dMIN_ROTATION                    = 0.01;
const double c_dMAX_FIRST_LINE_INDENT           = 20.0;
const double c_dMIN_RIGHT_MARGIN                = 0.4;
constexpr double c_dSTANDART_TABLE_SPACING_MM   = 108.0 / c_dMMToDx;

const UINT   c_iWhiteColor = 0xFFFFFF;
const UINT   c_iBlackColor = 0x000000;
const UINT   c_iBlackColor2 = 0x111111;
const UINT   c_iGreyColor  = 0xC0C0C0;
const UINT   c_iGreyColor2 = 0x999999;

const double c_dSTANDART_LEFT_INDENT_MM = 30;
const double c_dSTANDART_RIGHT_INDENT_MM = 15;
const double c_dSTANDART_FIRSTLINE_INDENT_MM = 12.5;

const uint32_t c_SPACE_SYM = 0x20;

const UINT   c_iStartingIdForImages = 6;
constexpr size_t c_nAntiZero = ~0;
const UINT   c_iStandartRelativeHeight = 0x0400;

constexpr int32_t c_iST_PositionOffsetMax = std::numeric_limits<int32_t>::max();
constexpr int32_t c_iST_PositionOffsetMin = std::numeric_limits<int32_t>::min();
constexpr uint64_t c_iST_PositiveCoordinatetMin = 0;
constexpr uint64_t c_iST_PositiveCoordinatetMax = 27273042316900;

constexpr double c_dST_PositionOffsetMax = c_iST_PositionOffsetMax / c_dMMToEMU;
constexpr double c_dST_PositionOffsetMin = c_iST_PositionOffsetMin / c_dMMToEMU;
constexpr double c_dST_PositiveCoordinatetMin = c_iST_PositiveCoordinatetMin / c_dMMToEMU;
constexpr double c_dST_PositiveCoordinatetMax = c_iST_PositiveCoordinatetMax / c_dMMToEMU;

