//
// Contains classes and functions which are necessary to xpdf to work with PDFReader
//

#ifndef CORE_ADAPTORS_H
#define CORE_ADAPTORS_H

#include "../lib/xpdf/GlobalParams.h"
#include "../lib/xpdf/Object.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"

class GlobalParamsAdaptor : public GlobalParams
{
public:
    NSFonts::IFontManager *m_pFontManager;
    GlobalParamsAdaptor(const char *filename) : GlobalParams(filename) {}

    void SetFontManager(NSFonts::IFontManager* pFontManager);


};

// Ref operators

bool operator==(const Ref &a, const Ref &b)
{
    return a.gen == b.gen && a.num == b.gen;
}
bool operator<(const Ref &a, const Ref &b)
{
    if (a.num < b.num)
        return true;
    else if (a.num == b.num)
        return a.gen < b.gen;
    else
        return false;
}
bool operator<=(const Ref &a, const Ref &b)
{
    return (a < b) || (a == b);
}
bool operator>=(const Ref &a, const Ref &b)
{
    return !(a < b);
}
bool operator>(const Ref &a, const Ref &b)
{
    return !(a <= b);
}



#endif //CORE_ADAPTORS_H
