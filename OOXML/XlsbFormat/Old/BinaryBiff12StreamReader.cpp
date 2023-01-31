#include "BinaryBiff12StreamReader.h"

namespace XLSB
{
    CBinaryBiff12StreamReader::CBinaryBiff12StreamReader()
    {

    }

    DWORD CBinaryBiff12StreamReader::CBinaryBiff12StreamReader::GetDword(LPBYTE p)
    {
        return *(DWORD*)p;
    }

    WORD CBinaryBiff12StreamReader::GetWord(LPBYTE p)
    {
        return *(WORD*)p;
    }

    BYTE CBinaryBiff12StreamReader::GetByte(LPBYTE p)
    {
        return *(BYTE*)p;
    }

    double CBinaryBiff12StreamReader::GetFloat(LPBYTE p)
    {

        // When it's a simple precision float, Excel uses a special
        // encoding
        long rk = *(long*)p;
        double d = 0;
        if (rk & 0x02)
        {
            // int
            d = (double) (rk >> 2);
        }
        else
        {
            // hi words of IEEE num
            *((long *)&d+1) = rk & 0xfffffffc;
            *((long *)&d) = 0;
        }
        if (rk & 0x01)
        {
            // divide by 100
            d /= 100;
        }

        return d;
    }

    double CBinaryBiff12StreamReader::GetDouble(LPBYTE p)
    {
        return *(double*)p;
    }

    std::wstring CBinaryBiff12StreamReader::GetString(LPBYTE p, int len)
    {

        LONG lIndex = 0;
        std::wstring res;
        len *= 2;
        while (lIndex < len)
        {
           res.push_back((uint16_t)(p[lIndex]) | ((uint16_t)p[lIndex + 1] << 8));
           lIndex+=2;
        }

        return res;
    }

    bool CBinaryBiff12StreamReader::GetRecordId(LPBYTE& p, LPBYTE p_last, /*out*/DWORD& recid)
    {
        recid = 0;

        if (!p || p > p_last)
            return false;
        BYTE b1 = *(BYTE*)(p++);
        recid = DWORD(b1);

        if ((b1 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b2 = *(BYTE*)(p++);
        recid = (DWORD(b2) << 8) | recid;

        if ((b2 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b3 = *(BYTE*)(p++);
        recid = (DWORD(b3) << 16) | recid;

        if ((b3 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b4 = *(BYTE*)(p++);
        recid = (DWORD(b4) << 24) | recid;

        return true;
    }

    bool CBinaryBiff12StreamReader::GetRecordLength(LPBYTE& p, LPBYTE p_last, /*out*/DWORD& reclen)
    {
        reclen = 0;

        if (!p || p > p_last)
            return false;
        BYTE b1 = *(BYTE*)(p++);
        reclen = DWORD(b1 & 0x7F);

        if ((b1 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b2 = *(BYTE*)(p++);
        reclen = (DWORD(b2 & 0x7F) << 7) | reclen;

        if ((b2 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b3 = *(BYTE*)(p++);
        reclen = (DWORD(b3 & 0x7F) << 14) | reclen;

        if ((b3 & 0x80) == 0)
            return true;

        if (!p || p > p_last)
            return false;
        BYTE b4 = *(BYTE*)(p++);
        reclen = (DWORD(b4 & 0x7F) << 21) | reclen;

        return true;
    }

    bool CBinaryBiff12StreamReader::GetBit(unsigned char byte, int position) // position in range 0-7
    {
        return (byte >> position) & 0x1;
    }

    DWORD CBinaryBiff12StreamReader::GetBits(unsigned x,int p,int n)
    {
        return (x >> (p+1-n)) & ~(~0 << n);
    }
}
