#pragma once


struct GUID
{
    unsigned int Data1 = 0;
    unsigned short Data2 = 0;
    unsigned short Data3 = 0;
    unsigned long long Data4 = 0;

    unsigned char* getData4()
    {
        return reinterpret_cast<unsigned char*>(&Data4);
    }

    GUID (const GUID& o) : Data1(o.Data1), Data2(o.Data2), Data3(o.Data3)
    {
    }

    GUID& operator=(const GUID& o)
    {
        Data1 = o.Data1;
        Data2 = o.Data2;
        Data3 = o.Data3;
        Data4 = o.Data4;

        return *this;
    }

    bool operator!=(const GUID& oth)const
    {
        return Data1 != oth.Data1 || Data2 != oth.Data2 || Data3 != oth.Data3 || Data4 != oth.Data4;
    }

    bool operator==(const GUID& oth)const
    {
        return !operator!=(oth);
    }

    GUID (){}
};
