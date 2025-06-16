#pragma once

#define MAYBE_UNUSED(x) (void)(x)

const double c_dDpiX = 72.0;
const double c_dDpiY = 72.0;

constexpr double c_dMMToPix = 72.0 / 25.4;
constexpr double c_dPixToMM = 25.4 / 72.0;
constexpr double c_dMMToPt  = 72.0 / 25.4;
constexpr double c_dPtToMM  = 25.4 / 72.0;

const double c_dX_MIN_DEFAULT = 10000;
const double c_dY_MIN_DEFAULT = 10000;

const double c_dX_MAX_DEFAULT = -10000;
const double c_dY_MAX_DEFAULT = -10000;
