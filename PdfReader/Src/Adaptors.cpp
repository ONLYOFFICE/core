//
// Created by danya on 23.06.2021.
//

#include "Adaptors.h"

void GlobalParamsAdaptor::SetFontManager(NSFonts::IFontManager* pFontManager)
{
    m_pFontManager = pFontManager;
}

bool operator==(const Ref &a, const Ref &b) {
    return a.gen == b.gen && a.num == b.gen;
}

bool operator<(const Ref &a, const Ref &b) {
    if (a.num < b.num)
        return true;
    else if (a.num == b.num)
        return a.gen < b.gen;
    else
        return false;
}

bool operator<=(const Ref &a, const Ref &b) {
    return (a < b) || (a == b);
}

bool operator>=(const Ref &a, const Ref &b) {
    return !(a < b);
}

bool operator>(const Ref &a, const Ref &b) {
    return !(a <= b);
}