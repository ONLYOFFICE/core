#pragma once
#include <type_traits>

#include "../../../DesktopEditor/common/Types.h"


#define USING_DELETE_DUPLICATING_CONTS 0 // 0 - все сточки-дубликаты превращаются в shape, 1 - строчки дубликаты удаляются
// #define USE_DEFAULT_FONT_TO_RECALC

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
const double c_dTHE_SAME_STRING_Y_PRECISION_MM  = 0.02;
const double c_dTHE_SAME_STRING_X_PRECISION_MM  = 0.02;
const double c_dLINE_DISTANCE_ERROR_MM          = 0.3;
const double c_dERROR_OF_PARAGRAPH_BORDERS_MM   = 1.0;
const double c_dERROR_GAP                       = 1.5;
const double c_dCENTER_POSITION_ERROR_MM        = 1.5;
const double c_dTHE_STRING_X_PRECISION_MM       = 0.5;
const double c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM = 0.1;
const double c_dGRAPHICS_ERROR_MM               = 0.5;
const double c_dGRAPHICS_ERROR_IN_LINES_MM      = 0.3;
const double c_dMAX_LINE_HEIGHT_MM              = 2.5;
const double c_dMAX_LINE_WITH_TEXT_ERROR_MM     = 2.5;
const double c_dCORRECTION_FOR_FIRST_PARAGRAPH  = -1.5;
const double c_dLINE_DISTANCE_MAX_MM            = 50.0;
const double c_dSHAPE_TROUGH_MAX_MM             = 120.0;
const double c_dLINE_SPLIT_DISTANCE_MM          = 10.0;
const double c_dSHAPE_X_OFFSET                  = 1.5;
const double c_dAVERAGE_SPACE_WIDTH_COEF        = 0.9;
const double c_dSPACE_WIDTH_COEF                = 0.4;
const double c_dMIN_ROTATION                    = 0.01;

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
