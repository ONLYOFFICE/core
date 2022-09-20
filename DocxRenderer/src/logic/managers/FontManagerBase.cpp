#include "FontManagerBase.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "src/resources/Constants.h"

namespace NSFontManager
{
    CUnicodeRange::CUnicodeRange(const int& _start, const int& _end,
                                 const BYTE& _range, const BYTE& _rangenum):
        RangeNum(_rangenum), Range(_range), Start(_start), End(_end)
    {
    }

    CUnicodeRanges::CUnicodeRanges()
    {
        // https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur

        m_arRanges.push_back(CUnicodeRange(0x0000, 0x007F, 0, 0)); // Basic Latin
        m_arRanges.push_back(CUnicodeRange(0x0080, 0x00FF, 1, 0)); // Latin-1 Supplement
        m_arRanges.push_back(CUnicodeRange(0x0100, 0x017F, 2, 0)); // Latin Extended-A
        m_arRanges.push_back(CUnicodeRange(0x0180, 0x024F, 3, 0)); // Latin Extended-B
        m_arRanges.push_back(CUnicodeRange(0x0250, 0x02AF, 4, 0)); // IPA Extensions
        m_arRanges.push_back(CUnicodeRange(0x1D00, 0x1D7F, 4, 0)); // Phonetic Extensions
        m_arRanges.push_back(CUnicodeRange(0x1D80, 0x1DBF, 4, 0)); // Phonetic Extensions Supplement
        m_arRanges.push_back(CUnicodeRange(0x02B0, 0x02FF, 5, 0)); // Spacing Modifier Letters
        m_arRanges.push_back(CUnicodeRange(0xA700, 0xA71F, 5, 0)); // Modifier Tone Letters
        m_arRanges.push_back(CUnicodeRange(0x0300, 0x036F, 6, 0)); // Combining Diacritical Marks
        m_arRanges.push_back(CUnicodeRange(0x1DC0, 0x1DFF, 6, 0)); // Combining Diacritical Marks Supplement
        m_arRanges.push_back(CUnicodeRange(0x0370, 0x03FF, 7, 0)); // Greek and Coptic
        m_arRanges.push_back(CUnicodeRange(0x2C80, 0x2CFF, 8, 0)); // Coptic
        m_arRanges.push_back(CUnicodeRange(0x0400, 0x04FF, 9, 0)); // Cyrillic
        m_arRanges.push_back(CUnicodeRange(0x0500, 0x052F, 9, 0)); // Cyrillic Supplement
        m_arRanges.push_back(CUnicodeRange(0x2DE0, 0x2DFF, 9, 0)); // Cyrillic Extended-A
        m_arRanges.push_back(CUnicodeRange(0xA640, 0xA69F, 9, 0)); // Cyrillic Extended-B
        m_arRanges.push_back(CUnicodeRange(0x0530, 0x058F, 10, 0)); // Armenian
        m_arRanges.push_back(CUnicodeRange(0x0590, 0x05FF, 11, 0)); // Hebrew
        m_arRanges.push_back(CUnicodeRange(0xA500, 0xA63F, 12, 0)); // Vai
        m_arRanges.push_back(CUnicodeRange(0x0600, 0x06FF, 13, 0)); // Arabic
        m_arRanges.push_back(CUnicodeRange(0x0750, 0x077F, 13, 0)); // Arabic Supplement
        m_arRanges.push_back(CUnicodeRange(0x07C0, 0x07FF, 14, 0)); // NKo
        m_arRanges.push_back(CUnicodeRange(0x0900, 0x097F, 15, 0)); // Devanagari
        m_arRanges.push_back(CUnicodeRange(0x0980, 0x09FF, 16, 0)); // Bengali
        m_arRanges.push_back(CUnicodeRange(0x0A00, 0x0A7F, 17, 0)); // Gurmukhi
        m_arRanges.push_back(CUnicodeRange(0x0A80, 0x0AFF, 18, 0)); // Gujarati
        m_arRanges.push_back(CUnicodeRange(0x0B00, 0x0B7F, 19, 0)); // Oriya
        m_arRanges.push_back(CUnicodeRange(0x0B80, 0x0BFF, 20, 0)); // Tamil
        m_arRanges.push_back(CUnicodeRange(0x0C00, 0x0C7F, 21, 0)); // Telugu
        m_arRanges.push_back(CUnicodeRange(0x0C80, 0x0CFF, 22, 0)); // Kannada
        m_arRanges.push_back(CUnicodeRange(0x0D00, 0x0D7F, 23, 0)); // Malayalam
        m_arRanges.push_back(CUnicodeRange(0x0E00, 0x0E7F, 24, 0)); // Thai
        m_arRanges.push_back(CUnicodeRange(0x0E80, 0x0EFF, 25, 0)); // Lao
        m_arRanges.push_back(CUnicodeRange(0x10A0, 0x10FF, 26, 0)); // Georgian
        m_arRanges.push_back(CUnicodeRange(0x2D00, 0x2D2F, 26, 0)); // Georgian Supplement
        m_arRanges.push_back(CUnicodeRange(0x1B00, 0x1B7F, 27, 0)); // Balinese
        m_arRanges.push_back(CUnicodeRange(0x1100, 0x11FF, 28, 0)); // Hangul Jamo
        m_arRanges.push_back(CUnicodeRange(0x1E00, 0x1EFF, 29, 0)); // Latin Extended Additional
        m_arRanges.push_back(CUnicodeRange(0x2C60, 0x2C7F, 29, 0)); // Latin Extended-C
        m_arRanges.push_back(CUnicodeRange(0xA720, 0xA7FF, 29, 0)); // Latin Extended-D
        m_arRanges.push_back(CUnicodeRange(0x1F00, 0x1FFF, 30, 0)); // Greek Extended
        m_arRanges.push_back(CUnicodeRange(0x2000, 0x206F, 31, 0)); // General Punctuation
        m_arRanges.push_back(CUnicodeRange(0x2E00, 0x2E7F, 31, 0)); // Supplemental Punctuation

        m_arRanges.push_back(CUnicodeRange(0x2070, 0x209F, 0, 1)); // Superscripts And Subscripts
        m_arRanges.push_back(CUnicodeRange(0x20A0, 0x20CF, 1, 1)); // Currency Symbols
        m_arRanges.push_back(CUnicodeRange(0x20D0, 0x20FF, 2, 1)); // Combining Diacritical Marks For Symbols
        m_arRanges.push_back(CUnicodeRange(0x2100, 0x214F, 3, 1)); // Letterlike Symbols
        m_arRanges.push_back(CUnicodeRange(0x2150, 0x218F, 4, 1)); // Number Forms
        m_arRanges.push_back(CUnicodeRange(0x2190, 0x21FF, 5, 1)); // Arrows
        m_arRanges.push_back(CUnicodeRange(0x27F0, 0x27FF, 5, 1)); // Supplemental Arrows-A
        m_arRanges.push_back(CUnicodeRange(0x2900, 0x297F, 5, 1)); // Supplemental Arrows-B
        m_arRanges.push_back(CUnicodeRange(0x2B00, 0x2BFF, 5, 1)); // Miscellaneous Symbols and Arrows
        m_arRanges.push_back(CUnicodeRange(0x2200, 0x22FF, 6, 1)); // Mathematical Operators
        m_arRanges.push_back(CUnicodeRange(0x2A00, 0x2AFF, 6, 1)); // Supplemental Mathematical Operators
        m_arRanges.push_back(CUnicodeRange(0x27C0, 0x27EF, 6, 1)); // Miscellaneous Mathematical Symbols-A
        m_arRanges.push_back(CUnicodeRange(0x2980, 0x29FF, 6, 1)); // Miscellaneous Mathematical Symbols-B
        m_arRanges.push_back(CUnicodeRange(0x2300, 0x23FF, 7, 1)); // Miscellaneous Technical
        m_arRanges.push_back(CUnicodeRange(0x2400, 0x243F, 8, 1)); // Control Pictures
        m_arRanges.push_back(CUnicodeRange(0x2440, 0x245F, 9, 1)); // Optical Character Recognition
        m_arRanges.push_back(CUnicodeRange(0x2460, 0x24FF, 10, 1)); // Enclosed Alphanumerics
        m_arRanges.push_back(CUnicodeRange(0x2500, 0x257F, 11, 1)); // Box Drawing
        m_arRanges.push_back(CUnicodeRange(0x2580, 0x259F, 12, 1)); // Block Elements
        m_arRanges.push_back(CUnicodeRange(0x25A0, 0x25FF, 13, 1)); // Geometric Shapes
        m_arRanges.push_back(CUnicodeRange(0x2600, 0x26FF, 14, 1)); // Miscellaneous Symbols
        m_arRanges.push_back(CUnicodeRange(0x2700, 0x27BF, 15, 1)); // Dingbats
        m_arRanges.push_back(CUnicodeRange(0x3000, 0x303F, 16, 1)); // CJK Symbols And Punctuation
        m_arRanges.push_back(CUnicodeRange(0x3040, 0x309F, 17, 1)); // Hiragana
        m_arRanges.push_back(CUnicodeRange(0x30A0, 0x30FF, 18, 1)); // Katakana
        m_arRanges.push_back(CUnicodeRange(0x31F0, 0x31FF, 18, 1)); // Katakana Phonetic Extensions
        m_arRanges.push_back(CUnicodeRange(0x3100, 0x312F, 19, 1)); // Bopomofo
        m_arRanges.push_back(CUnicodeRange(0x31A0, 0x31BF, 19, 1)); // Bopomofo Extended
        m_arRanges.push_back(CUnicodeRange(0x3130, 0x318F, 20, 1)); // Hangul Compatibility Jamo
        m_arRanges.push_back(CUnicodeRange(0xA840, 0xA87F, 21, 1)); // Phags-pa
        m_arRanges.push_back(CUnicodeRange(0x3200, 0x32FF, 22, 1)); // Enclosed CJK Letters And Months
        m_arRanges.push_back(CUnicodeRange(0x3300, 0x33FF, 23, 1)); // CJK Compatibility
        m_arRanges.push_back(CUnicodeRange(0xAC00, 0xD7AF, 24, 1)); // Hangul Syllables
        m_arRanges.push_back(CUnicodeRange(0x10000, 0x10FFFF, 25, 1)); // Non-Plane 0
        m_arRanges.push_back(CUnicodeRange(0x10900, 0x1091F, 26, 1)); // Phoenician
        m_arRanges.push_back(CUnicodeRange(0x4E00, 0x9FFF, 27, 1)); // CJK Unified Ideographs
        m_arRanges.push_back(CUnicodeRange(0x2E80, 0x2EFF, 27, 1)); // CJK Radicals Supplement
        m_arRanges.push_back(CUnicodeRange(0x2F00, 0x2FDF, 27, 1)); // Kangxi Radicals
        m_arRanges.push_back(CUnicodeRange(0x2FF0, 0x2FFF, 27, 1)); // Ideographic Description Characters
        m_arRanges.push_back(CUnicodeRange(0x3400, 0x4DBF, 27, 1)); // CJK Unified Ideographs Extension A
        m_arRanges.push_back(CUnicodeRange(0x3190, 0x319F, 27, 1)); // Kanbun
        m_arRanges.push_back(CUnicodeRange(0x20000, 0x2A6DF, 27, 1)); // CJK Unified Ideographs Extension B
        m_arRanges.push_back(CUnicodeRange(0xE000, 0xF8FF, 28, 1)); // Private Use Area (plane 0)
        m_arRanges.push_back(CUnicodeRange(0x31C0, 0x31EF, 29, 1)); // CJK Strokes
        m_arRanges.push_back(CUnicodeRange(0xF900, 0xFAFF, 29, 1)); // CJK Compatibility Ideographs
        m_arRanges.push_back(CUnicodeRange(0x2F800, 0x2FA1F, 29, 1)); // CJK Compatibility Ideographs Supplement
        m_arRanges.push_back(CUnicodeRange(0xFB00, 0xFB4F, 30, 1)); // Alphabetic Presentation Forms
        m_arRanges.push_back(CUnicodeRange(0xFB50, 0xFDFF, 31, 1)); // Arabic Presentation Forms-A

        m_arRanges.push_back(CUnicodeRange(0xFE20, 0xFE2F, 0, 2)); // Combining Half Marks
        m_arRanges.push_back(CUnicodeRange(0xFE10, 0xFE1F, 1, 2)); // Vertical Forms
        m_arRanges.push_back(CUnicodeRange(0xFE30, 0xFE4F, 1, 2)); // CJK Compatibility Forms
        m_arRanges.push_back(CUnicodeRange(0xFE50, 0xFE6F, 2, 2)); // Small Form Variants
        m_arRanges.push_back(CUnicodeRange(0xFE70, 0xFEFF, 3, 2)); // Arabic Presentation Forms-B
        m_arRanges.push_back(CUnicodeRange(0xFF00, 0xFFEF, 4, 2)); // Halfwidth And Fullwidth Forms
        m_arRanges.push_back(CUnicodeRange(0xFFF0, 0xFFFF, 5, 2)); // Specials
        m_arRanges.push_back(CUnicodeRange(0x0F00, 0x0FFF, 6, 2)); // Tibetan
        m_arRanges.push_back(CUnicodeRange(0x0700, 0x074F, 7, 2)); // Syriac
        m_arRanges.push_back(CUnicodeRange(0x0780, 0x07BF, 8, 2)); // Thaana
        m_arRanges.push_back(CUnicodeRange(0x0D80, 0x0DFF, 9, 2)); // Sinhala
        m_arRanges.push_back(CUnicodeRange(0x1000, 0x109F, 10, 2)); // Myanmar
        m_arRanges.push_back(CUnicodeRange(0x1200, 0x137F, 11, 2)); // Ethiopic
        m_arRanges.push_back(CUnicodeRange(0x1380, 0x139F, 11, 2)); // Ethiopic Supplement
        m_arRanges.push_back(CUnicodeRange(0x2D80, 0x2DDF, 11, 2)); // Ethiopic Extended
        m_arRanges.push_back(CUnicodeRange(0x13A0, 0x13FF, 12, 2)); // Cherokee
        m_arRanges.push_back(CUnicodeRange(0x1400, 0x167F, 13, 2)); // Unified Canadian Aboriginal Syllabics
        m_arRanges.push_back(CUnicodeRange(0x1680, 0x169F, 14, 2)); // Ogham
        m_arRanges.push_back(CUnicodeRange(0x16A0, 0x16FF, 15, 2)); // Runic
        m_arRanges.push_back(CUnicodeRange(0x1780, 0x17FF, 16, 2)); // Khmer
        m_arRanges.push_back(CUnicodeRange(0x19E0, 0x19FF, 16, 2)); // Khmer Symbols
        m_arRanges.push_back(CUnicodeRange(0x1800, 0x18AF, 17, 2)); // Mongolian
        m_arRanges.push_back(CUnicodeRange(0x2800, 0x28FF, 18, 2)); // Braille Patterns
        m_arRanges.push_back(CUnicodeRange(0xA000, 0xA48F, 19, 2)); // Yi Syllables
        m_arRanges.push_back(CUnicodeRange(0xA490, 0xA4CF, 19, 2)); // Yi Radicals
        m_arRanges.push_back(CUnicodeRange(0x1700, 0x171F, 20, 2)); // Tagalog
        m_arRanges.push_back(CUnicodeRange(0x1720, 0x173F, 20, 2)); // Hanunoo
        m_arRanges.push_back(CUnicodeRange(0x1740, 0x175F, 20, 2)); // Buhid
        m_arRanges.push_back(CUnicodeRange(0x1760, 0x177F, 20, 2)); // Tagbanwa
        m_arRanges.push_back(CUnicodeRange(0x10300, 0x1032F, 21, 2)); // Old Italic
        m_arRanges.push_back(CUnicodeRange(0x10330, 0x1034F, 22, 2)); // Gothic
        m_arRanges.push_back(CUnicodeRange(0x10400, 0x1044F, 23, 2)); // Deseret
        m_arRanges.push_back(CUnicodeRange(0x1D000, 0x1D0FF, 24, 2)); // Byzantine Musical Symbols
        m_arRanges.push_back(CUnicodeRange(0x1D100, 0x1D1FF, 24, 2)); // Musical Symbols
        m_arRanges.push_back(CUnicodeRange(0x1D200, 0x1D24F, 24, 2)); // Ancient Greek Musical Notation
        m_arRanges.push_back(CUnicodeRange(0x1D400, 0x1D7FF, 25, 2)); // Mathematical Alphanumeric Symbols
        m_arRanges.push_back(CUnicodeRange(0xF0000, 0xFFFFD, 26, 2)); // Private Use (plane 15)
        m_arRanges.push_back(CUnicodeRange(0x100000, 0x10FFFD, 26, 2)); // Private Use (plane 16)
        m_arRanges.push_back(CUnicodeRange(0xFE00, 0xFE0F, 27, 2)); // Variation Selectors
        m_arRanges.push_back(CUnicodeRange(0xE0100, 0xE01EF, 27, 2)); // Variation Selectors Supplement
        m_arRanges.push_back(CUnicodeRange(0xE0000, 0xE007F, 28, 2)); // Tags
        m_arRanges.push_back(CUnicodeRange(0x1900, 0x194F, 29, 2)); // Limbu
        m_arRanges.push_back(CUnicodeRange(0x1950, 0x197F, 30, 2)); // Tai Le
        m_arRanges.push_back(CUnicodeRange(0x1980, 0x19DF, 31, 2)); // New Tai Lue

        m_arRanges.push_back(CUnicodeRange(0x1A00, 0x1A1F, 0, 3)); // Buginese
        m_arRanges.push_back(CUnicodeRange(0x2C00, 0x2C5F, 1, 3)); // Glagolitic
        m_arRanges.push_back(CUnicodeRange(0x2D30, 0x2D7F, 2, 3)); // Tifinagh
        m_arRanges.push_back(CUnicodeRange(0x4DC0, 0x4DFF, 3, 3)); // Yijing Hexagram Symbols
        m_arRanges.push_back(CUnicodeRange(0xA800, 0xA82F, 4, 3)); // Syloti Nagri
        m_arRanges.push_back(CUnicodeRange(0x10000, 0x1007F, 5, 3)); // Linear B Syllabary
        m_arRanges.push_back(CUnicodeRange(0x10080, 0x100FF, 5, 3)); // Linear B Ideograms
        m_arRanges.push_back(CUnicodeRange(0x10100, 0x1013F, 5, 3)); // Aegean Numbers
        m_arRanges.push_back(CUnicodeRange(0x10140, 0x1018F, 6, 3)); // Ancient Greek Numbers
        m_arRanges.push_back(CUnicodeRange(0x10380, 0x1039F, 7, 3)); // Ugaritic
        m_arRanges.push_back(CUnicodeRange(0x103A0, 0x103DF, 8, 3)); // Old Persian
        m_arRanges.push_back(CUnicodeRange(0x10450, 0x1047F, 9, 3)); // Shavian
        m_arRanges.push_back(CUnicodeRange(0x10480, 0x104AF, 10, 3)); // Osmanya
        m_arRanges.push_back(CUnicodeRange(0x10800, 0x1083F, 11, 3)); // Cypriot Syllabary
        m_arRanges.push_back(CUnicodeRange(0x10A00, 0x10A5F, 12, 3)); // Kharoshthi
        m_arRanges.push_back(CUnicodeRange(0x1D300, 0x1D35F, 13, 3)); // Tai Xuan Jing Symbols
        m_arRanges.push_back(CUnicodeRange(0x12000, 0x123FF, 14, 3)); // Cuneiform
        m_arRanges.push_back(CUnicodeRange(0x12400, 0x1247F, 14, 3)); // Cuneiform Numbers and Punctuation
        m_arRanges.push_back(CUnicodeRange(0x1D360, 0x1D37F, 15, 3)); // Counting Rod Numerals
        m_arRanges.push_back(CUnicodeRange(0x1B80, 0x1BBF, 16, 3)); // Sundanese
        m_arRanges.push_back(CUnicodeRange(0x1C00, 0x1C4F, 17, 3)); // Lepcha
        m_arRanges.push_back(CUnicodeRange(0x1C50, 0x1C7F, 18, 3)); // Ol Chiki
        m_arRanges.push_back(CUnicodeRange(0xA880, 0xA8DF, 19, 3)); // Saurashtra
        m_arRanges.push_back(CUnicodeRange(0xA900, 0xA92F, 20, 3)); // Kayah Li
        m_arRanges.push_back(CUnicodeRange(0xA930, 0xA95F, 21, 3)); // Rejang
        m_arRanges.push_back(CUnicodeRange(0xAA00, 0xAA5F, 22, 3)); // Cham
        m_arRanges.push_back(CUnicodeRange(0x10190, 0x101CF, 23, 3)); // Ancient Symbols
        m_arRanges.push_back(CUnicodeRange(0x101D0, 0x101FF, 24, 3)); // Phaistos Disc
        m_arRanges.push_back(CUnicodeRange(0x102A0, 0x102DF, 25, 3)); // Carian
        m_arRanges.push_back(CUnicodeRange(0x10280, 0x1029F, 25, 3)); // Lycian
        m_arRanges.push_back(CUnicodeRange(0x10920, 0x1093F, 25, 3)); // Lydian
        m_arRanges.push_back(CUnicodeRange(0x1F030, 0x1F09F, 26, 3)); // Domino Tiles
        m_arRanges.push_back(CUnicodeRange(0x1F000, 0x1F02F, 26, 3)); // Mahjong Tiles
        // 27: "Reserved for process-internal usage"
        // 28: "Reserved for process-internal usage"
        // 29: "Reserved for process-internal usage"
        // 30: "Reserved for process-internal usage"
        // 31: "Reserved for process-internal usage"
    }

    void CUnicodeRanges::CheckRange(const int& symbol, BYTE& Range, BYTE& RangeNum)
    {
        // определяем range и двигаем его в начало.
        std::list<CUnicodeRange>::iterator iter = m_arRanges.begin();
        while (iter != m_arRanges.end())
        {
            CUnicodeRange& range = *iter;
            if (symbol >= range.Start && symbol <= range.End)
            {
                Range = range.Range;
                RangeNum = range.RangeNum;

                m_arRanges.splice(m_arRanges.begin(), m_arRanges, iter);
                return;
            }
            iter++;
        }
    }

    void CUnicodeRanges::CheckRange(const int& symbol, int& Range1, int& Range2, int& Range3, int& Range4)
    {
        BYTE nRange = 0xFF;
        BYTE nRangeNum = 0xFF;
        CheckRange(symbol, nRange, nRangeNum);

        switch (nRangeNum)
        {
        case 0: Range1 |= (1 << nRange); break;
        case 1: Range2 |= (1 << nRange); break;
        case 2: Range3 |= (1 << nRange); break;
        case 3: Range4 |= (1 << nRange); break;
        default:
            break;
        }
    }

    CFontAdvanced::CFontAdvanced()
    {
        m_oFont.SetDefaultParams();
        m_arSignature.clear();
    }

    CFontAdvanced::CFontAdvanced(const CFontAdvanced& oSrc)
    {
        *this = oSrc;
    }

    CFontAdvanced& CFontAdvanced::operator=(const CFontAdvanced& oSrc)
    {
        if (this == &oSrc)
        {
            return *this;
        }

        m_oFont				= oSrc.m_oFont;

        m_dAscent			= oSrc.m_dAscent;
        m_dDescent			= oSrc.m_dDescent;
        m_dLineSpacing		= oSrc.m_dLineSpacing;
        m_dEmHeight			= oSrc.m_dEmHeight;

        m_dBaselineOffset	= oSrc.m_dBaselineOffset;

        m_dSpaceWidthMM		= oSrc.m_dSpaceWidthMM;

        m_strFamilyName		= oSrc.m_strFamilyName;
        m_strPANOSE			= oSrc.m_strPANOSE;
        m_lStyle			= oSrc.m_lStyle;
        m_arSignature       = oSrc.m_arSignature;
        m_bIsFixedWidth		= oSrc.m_bIsFixedWidth;
        m_lAvgWidth			= oSrc.m_lAvgWidth;

        return *this;
    }

    CFontPickUp::CFontPickUp() : m_oFont()
    {
    }

    CFontPickUp::CFontPickUp(const CFontPickUp& oSrc)
    {
        *this = oSrc;
    }

    CFontPickUp& CFontPickUp::operator=(const CFontPickUp& oSrc)
    {
        if (this == &oSrc)
        {
            return *this;
        }

        m_oFont			= oSrc.m_oFont;
        m_lRange		= oSrc.m_lRange;
        m_lRangeNum		= oSrc.m_lRangeNum;
        m_strPickFont	= oSrc.m_strPickFont;
        m_lPickStyle	= oSrc.m_lPickStyle;

        return *this;
    }

    CFontManagerBase::CFontManagerBase(NSFonts::IApplicationFonts* pFonts)
    {
        m_pManager = NSFontManager::CreateFontManager(pFonts);

        SetDefaultFont(L"Arial");

        ClearPickUps();
    }

    CFontManagerBase::~CFontManagerBase()
    {
        RELEASEINTERFACE(m_pManager);
    }

    void CFontManagerBase::ClearPickUps()
    {
        m_arListPicUps.clear();
        m_strCurrentPickFont = L"";
        m_lCurrentPictFontStyle = 0;
    }

    void CFontManagerBase::SetDefaultFont(const std::wstring& strName)
    {
        m_strDefaultFont = strName;
    }

    std::wstring CFontManagerBase::GetDefaultFont()
    {
        return m_strDefaultFont;
    }

    void CFontManagerBase::LoadFont(long lFaceIndex, bool bIsNeedAddToMap)
    {
    }

    void CFontManagerBase::LoadFontByName(const std::wstring& strName, const double& dSize, const LONG& lStyle, const double& dDpiX, const double& dDpiY)
    {
        m_pManager->LoadFontByName(strName, (float)dSize, lStyle, dDpiX, dDpiY);
        m_pManager->AfterLoad();

        LoadFontMetrics();
        LoadFontParams();

        m_oFont.m_lAvgWidth = -1;
    }

    void CFontManagerBase::LoadFontByFile(const std::wstring& strPath, const double& dSize, const double& dDpiX, const double& dDpiY, const LONG& lFaceIndex)
    {
        m_pManager->LoadFontFromFile(strPath, (int)lFaceIndex, (float)dSize, dDpiX, dDpiY);
        m_pManager->AfterLoad();

        LoadFontMetrics();
        LoadFontParams();
        m_oFont.m_oFont.Name = m_pManager->GetName();
        m_oFont.m_strFamilyName = m_oFont.m_oFont.Name;

        m_oFont.m_lAvgWidth = -1;

        bool bIsCID = false;
        std::wstring sFileExt = NSFile::GetFileExtention(strPath);
        if (std::wstring::npos != sFileExt.find(L"cid"))
            bIsCID = true;

        std::wstring sFileName = NSFile::GetFileName(strPath);
        std::wstring::size_type pos = sFileName.rfind('.');
        if (std::wstring::npos != pos)
            sFileName = sFileName.substr(0, pos);
        std::wstring sEncFilePath = NSFile::GetDirectoryName(strPath) + L"/" + sFileName + L".enc";

        XmlUtils::CXmlNode oMainNode;
        oMainNode.FromXmlFile(sEncFilePath);

        if (L"PDF-resources" == oMainNode.GetName())
        {
            if (bIsCID)
            {
                XmlUtils::CXmlNode oType0Node;
                if ( oMainNode.GetNode( L"Type0", oType0Node ) )
                {
                    XmlUtils::CXmlNode oNode;
                    if ( oType0Node.GetNode( L"DescendantFonts", oNode ) )
                    {
                        XmlUtils::CXmlNode oDescNode;
                        if ( oNode.GetNode( L"FontDescriptor", oDescNode ) )
                        {
                            XmlUtils::CXmlNode oCurNode;
                            if ( oNode.GetNode( L"AvgWidth", oCurNode ) )
                            {
                                std::wstring sValue = oCurNode.GetAttribute(L"value");
                                try {
                                    m_oFont.m_lAvgWidth = (SHORT)std::stol(sValue);
                                } catch (std::invalid_argument &) {}
                            }
                        }
                    }
                }
            }
            else
            {
                XmlUtils::CXmlNode oNode;
                if ( oMainNode.GetNode( L"FontDescriptor", oNode ) )
                {
                    XmlUtils::CXmlNode oCurNode;
                    if ( oNode.GetNode( L"AvgWidth", oCurNode ) )
                    {
                        std::wstring sValue = oCurNode.GetAttribute(L"value");
                        try {
                            m_oFont.m_lAvgWidth = (SHORT)std::stol(sValue);
                        } catch (std::invalid_argument &) {}
                    }
                }
            }
        }
    }

    void CFontManagerBase::CalculateBaselineOffset()
    {
        LoadFont();

        double d1 = 3 * (m_oFont.m_dLineSpacing - m_oFont.m_dDescent) - m_oFont.m_dAscent;
        d1 /= 2.0;

        d1 *= (m_oFont.m_oFont.Size / m_oFont.m_dEmHeight);
        m_oFont.m_dBaselineOffset = d1;
    }

    void CFontManagerBase::LoadFontMetrics()
    {
        m_oFont.m_dAscent = m_pManager->GetAscender();
        m_oFont.m_dDescent = m_pManager->GetDescender();
        m_oFont.m_dLineSpacing = m_pManager->GetLineHeight();
        m_oFont.m_dEmHeight = m_pManager->GetUnitsPerEm();

        m_oFont.m_dBaselineOffset = (c_dPtToMM * m_oFont.m_dDescent * m_oFont.m_oFont.Size / m_oFont.m_dEmHeight);
    }

    std::wstring CFontManagerBase::ToHexString( BYTE uc )
    {
        std::wstring sRet = L"";
        char c1 = (char)(uc >> 4);
        char c2 = (char)(uc & 0x0F);
        sRet += (wchar_t)((c1 < 10) ? ('0' + c1) : ('A' + c1 - 10));
        sRet += (wchar_t)((c2 < 10) ? ('0' + c2) : ('A' + c2 - 10));
        return sRet;
    }

    BYTE CFontManagerBase::FromHexString( wchar_t c1, wchar_t c2 )
    {
        BYTE res = 0;
        res |= ((c1 >= 'A') ? (c1 - 'A' + 10) : (c1 - '0'));
        res <<= 4;
        res |= ((c2 >= 'A') ? (c2 - 'A' + 10) : (c2 - '0'));
        return res;
    }

    void CFontManagerBase::LoadFontParams(bool bIsPath)
    {
        // читаем и выставляем все настройки шрифта
        if (nullptr == m_pManager || nullptr == m_pManager->GetFile())
            return;

        m_oFont.m_strFamilyName = m_oFont.m_oFont.Name;

        m_oFont.m_lStyle = 0x00;
        if (m_pManager->GetFile()->IsBold())
        {
            m_oFont.m_lStyle |= 0x01;
        }
        if (m_pManager->GetFile()->IsItalic())
        {
            m_oFont.m_lStyle |= 0x02;
        }

        // PANOSE
        BYTE pPanose[10];
        m_pManager->GetFile()->GetPanose(pPanose);
        m_oFont.m_strPANOSE.clear();
        for ( int i = 0; i < 10; i++ )
        {
            m_oFont.m_strPANOSE += ToHexString(pPanose[i]);
        }

        // IsFixed
        m_oFont.m_bIsFixedWidth = m_pManager->GetFile()->IsFixedWidth();

        // Signature
        m_oFont.m_arSignature.clear();

        for ( unsigned int i = 0; i < 6; i++ )
        {
            DWORD value = 0;

            for ( unsigned long bit = 0; bit < 32; bit++ )
            {
                if (m_pManager->GetFile()->IsUnicodeRangeAvailable(bit, i))
                {
                    value |= ( 1 << bit );
                }
            }

            m_oFont.m_arSignature.push_back(value);
        }
    }

    void CFontManagerBase::CheckRanges(UINT& lRange1, UINT& lRange2, UINT& lRange3, UINT& lRange4, BYTE& lRangeNum, BYTE& lRange)
    {
        if (0 == lRangeNum)
            lRange1 |= 1 << lRange;
        else if (1 == lRangeNum)
            lRange2 |= 1 << lRange;
        else if (2 == lRangeNum)
            lRange3 |= 1 << lRange;
        else
            lRange4 |= 1 << lRange;
    }

    bool CFontManagerBase::GenerateFontName(NSStringUtils::CStringUTF32& oText)
    {
        if (m_oFont.m_oFont.Path.empty() || oText.empty())
        {
            m_strCurrentPickFont = m_oFont.m_strFamilyName;
            m_lCurrentPictFontStyle = m_oFont.m_lStyle;
            return false;
        }

        BYTE lRangeNum	= 0xFF;
        BYTE lRange		= 0xFF;

        m_oRanges.CheckRange(oText[0], lRange, lRangeNum);
        std::list<CFontPickUp>::iterator posStart, pos;
        posStart = pos = m_arListPicUps.begin();

        while (m_arListPicUps.end() != pos)
        {
            std::list<CFontPickUp>::iterator posOld = pos;
            CFontPickUp& oPick = *(pos++);
            if ((oPick.m_oFont.m_oFont.IsEqual2(&m_oFont.m_oFont)) && (lRangeNum == oPick.m_lRangeNum) && (lRange == oPick.m_lRange))
            {
                // нашли! ничего подбирать не нужно
                // нужно просто выкинуть этот шрифт наверх
                m_arListPicUps.splice(m_arListPicUps.begin(), m_arListPicUps, posOld);
                m_strCurrentPickFont = oPick.m_strPickFont;
                m_lCurrentPictFontStyle = oPick.m_lPickStyle;
                return false;
            }
        }

        // не нашли...
        CFontPickUp oPick;
        oPick.m_lRangeNum	= lRangeNum;
        oPick.m_lRange		= lRange;
        oPick.m_oFont		= m_oFont;
        oPick.m_strPickFont	= m_oFont.m_strFamilyName;
        oPick.m_lPickStyle	= m_oFont.m_lStyle;

        UINT dwR1 = m_oFont.m_arSignature[0];
        UINT dwR2 = m_oFont.m_arSignature[1];
        UINT dwR3 = m_oFont.m_arSignature[2];
        UINT dwR4 = m_oFont.m_arSignature[3];
        UINT dwCodePage1	= 0;
        UINT dwCodePage2	= 0;

        if ((lRangeNum == 1) && (lRange == 28))
        {
            dwCodePage1 = 0x80000000;
            //strText = (WCHAR)(strText[0] - 0xF000);
        }
        else if (((lRangeNum == 2) && (lRange == 3)) || ((lRangeNum == 1) && (lRange == 31)) || ((lRangeNum == 0) && (lRange == 13)))
        {
            // арабский язык!!!
            dwR1 = 1 << 13;
            dwR2 = 1 << 31;
            dwR3 = 1 << 3;
        }
        else
        {
            CheckRanges(dwR1, dwR2, dwR3, dwR4, lRangeNum, lRange);
        }

        NSFonts::CFontSelectFormat oFormat;

        std::wstring sFontNameSelect = L"";
        if (m_oFont.m_strFamilyName.empty() && !m_oFont.m_oFont.Path.empty())
            sFontNameSelect = m_strDefaultFont;
        else
            sFontNameSelect = m_oFont.m_strFamilyName;

        bool bSelectBold = false;
        bool bSelectItalic = false;
        CheckFontNamePDF(sFontNameSelect, bSelectBold, bSelectItalic);

        oFormat.wsName = new std::wstring(sFontNameSelect);

        if (!m_oFont.m_strPANOSE.empty())
        {
            oFormat.pPanose = new BYTE[10];

            const wchar_t* pPanoseStr = m_oFont.m_strPANOSE.c_str();
            for (int i = 0; i < 10; ++i)
            {
                oFormat.pPanose[i] = FromHexString(pPanoseStr[0], pPanoseStr[1]);
                pPanoseStr += 2;
            }
        }

        oFormat.bBold = new INT(((m_oFont.m_lStyle & 0x01) == 0x01) ? 1 : 0);
        oFormat.bItalic = new INT(((m_oFont.m_lStyle & 0x02) == 0x02) ? 1 : 0);
        oFormat.bFixedWidth = new INT(m_oFont.m_bIsFixedWidth ? 1 : 0);
        if (-1 != m_oFont.m_lAvgWidth)
            oFormat.shAvgCharWidth = new SHORT((SHORT)m_oFont.m_lAvgWidth);
        oFormat.ulRange1 = new UINT(dwR1);
        oFormat.ulRange2 = new UINT(dwR2);
        oFormat.ulRange3 = new UINT(dwR3);
        oFormat.ulRange4 = new UINT(dwR4);
        oFormat.ulCodeRange1 = new UINT(dwCodePage1);
        oFormat.ulCodeRange2 = new UINT(dwCodePage2);

        if (oFormat.bBold && *(oFormat.bBold) == 1 && oFormat.pPanose && oFormat.pPanose[2] < 7)
            oFormat.pPanose[2] = 7;

        oFormat.wsDefaultName = new std::wstring(L"Arial");

        NSFonts::CFontInfo* pInfo = m_pManager->GetFontInfoByParams(oFormat);

        oPick.m_strPickFont = pInfo->m_wsFontName;
        oPick.m_lPickStyle = 0;
        if (pInfo->m_bBold)
            oPick.m_lPickStyle |= 0x01;
        if (pInfo->m_bItalic)
            oPick.m_lPickStyle |= 0x02;

        m_strCurrentPickFont = oPick.m_strPickFont;
        m_lCurrentPictFontStyle = oPick.m_lPickStyle;

        m_arListPicUps.push_front(oPick);
        return true;
    }

    bool CFontManagerBase::CheckFontNameStyle(std::wstring& sName, const std::wstring& sStyle)
    {
        size_t nPos = 0;
        size_t nLenReplace = sStyle.length();
        bool bRet = false;

        std::wstring sName2 = sName;
        NSStringExt::ToLower(sName2);

        while (std::wstring::npos != (nPos = sName2.find(sStyle, nPos)))
        {
            size_t nOffset = 0;
            if ((nPos > 0) && sName2.at(nPos - 1) == '-')
            {
                --nPos;
                ++nOffset;
            }

            bRet = true;
            sName.erase(nPos, nLenReplace + nOffset);
            sName2.erase(nPos, nLenReplace + nOffset);
        }
        return bRet;
    }

    void CFontManagerBase::CheckFontNamePDF(std::wstring& sName, bool& bBold, bool& bItalic)
    {
        if (sName.length() > 7 && sName.at(6) == '+')
        {
            bool bIsRemove = true;
            for (int nIndex = 0; nIndex < 6; nIndex++)
            {
                wchar_t nChar = sName.at(nIndex);
                if (nChar < 'A' || nChar > 'Z')
                {
                    bIsRemove = false;
                    break;
                }
            }
            if (bIsRemove)
            {
                sName.erase(0, 7);
            }
        }

        CheckFontNameStyle(sName, L"regular");
        CheckFontNameStyle(sName, L"condensed");
        CheckFontNameStyle(sName, L"condensedlight");
        //CheckFontNameStyle(sName, L"light");

        CheckFontNameStyle(sName, L"condensedbold");
        CheckFontNameStyle(sName, L"semibold");
        if (CheckFontNameStyle(sName, L"boldmt")) bBold = true;
        if (CheckFontNameStyle(sName, L"bold")) bBold = true;

        if (CheckFontNameStyle(sName, L"italicmt")) bItalic = true;
        if (CheckFontNameStyle(sName, L"italic")) bItalic = true;
        if (CheckFontNameStyle(sName, L"oblique")) bItalic = true;

        if (CheckFontNameStyle(sName, L"bolditalicmt")) { bBold = true; bItalic = true; }
        if (CheckFontNameStyle(sName, L"bolditalic")) { bBold = true; bItalic = true; }
        if (CheckFontNameStyle(sName, L"bold_italic")) { bBold = true; bItalic = true; }
        if (CheckFontNameStyle(sName, L"boldoblique")) { bBold = true; bItalic = true; }
        if (CheckFontNameStyle(sName, L"bold_oblique")) { bBold = true; bItalic = true; }
    }
}
