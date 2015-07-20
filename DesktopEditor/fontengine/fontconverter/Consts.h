#ifndef _ASC_FONT_CONVERTER_CONSTS_H_
#define _ASC_FONT_CONVERTER_CONSTS_H_

namespace NSFontConverter
{
    const long c_lFromAll = 0xFFFF; // Конвертировать из любого формата
    const long c_lFromTT  = 0x0010; // Конвертировать из TrueType формата
    const long c_lFromT1  = 0x0020; // Конвертировать из Type 1 формата
    const long c_lFromCFF = 0x0040; // Конвертировать из CFF формата
    const long c_lFlagsGids = 0x0080;
}

#endif // _ASC_FONT_CONVERTER_CONSTS_H_
