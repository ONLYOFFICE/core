#pragma once

struct GUID
{
    unsigned long Data1 = 0;
    unsigned short Data2 = 0;
    unsigned short Data3 = 0;
    unsigned char Data4[8] = {0,0,0,0,0,0,0,0};

    GUID (const GUID& o) : Data1(o.Data1), Data2(o.Data2), Data3(o.Data3)
    {
        for (unsigned i = 0; i < sizeof(Data4); i++)
            Data4[i] = o.Data4[i];
    }

    GUID& operator=(const GUID& o)
    {
        Data1 = o.Data1;
        Data2 = o.Data2;
        Data3 = o.Data3;
        for (unsigned i = 0; i < sizeof(Data4); i++)
            Data4[i] = o.Data4[i];

        return *this;
    }

    bool operator!=(const GUID& oth)
    {
        for (int i = 0; i < 8; i++)
            if (Data4[i] != oth.Data4[i])
                return true;

        return Data1 != oth.Data1 || Data2 != oth.Data2 || Data3 != oth.Data3;
    }

    GUID (){}
};
