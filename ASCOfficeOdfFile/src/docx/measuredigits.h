#pragma once

#include <utility>
#include <string>

class CApplicationFonts;

namespace cpdoccore {
namespace utils {

    std::pair<float, float> GetMaxDigitSizePixels(const std::wstring & fontName, double fontSize, double dpi, long fontStyle, CApplicationFonts *appFonts);

}
}
