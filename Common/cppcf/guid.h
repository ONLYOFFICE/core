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

    GUID (){}
};
