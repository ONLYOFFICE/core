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

namespace NSStrings
{
    GString* CreateString(const std::wstring& str)
    {
        std::string sUtf8 = U_TO_UTF8(str);
        return new GString(sUtf8.c_str(), sUtf8.length());
    }
    GString* CreateString(const std::string& str)
    {
        return new GString(str.c_str(), str.length());
    }

    std::wstring GetString(GString* str)
    {
        return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)str->getCString(), (LONG)str->getLength());
    }
    std::string GetStringA(GString* str)
    {
        return std::string(str->getCString(), str->getLength());
    }
}
