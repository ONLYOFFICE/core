#ifndef _CPDOCCORE_MEASURE_DIGITS_H_
#define _CPDOCCORE_MEASURE_DIGITS_H_

namespace cpdoccore {
namespace utils {

std::pair<float,float> GetMaxDigitSizePixels(const wchar_t * fontName, float fontSize, float dpi = 96.0, long fontStyle = 0);

}
}

#endif
