#ifndef CBINARYBIFF12STREAMREADER_H
#define CBINARYBIFF12STREAMREADER_H

#include "../../../../../DesktopEditor/common/Types.h"
#include "../../Base/Types_32.h"
#include <string>
#include <memory.h>
#include <iostream>
#include "../../../../../ASCOfficePPTXFile/Editor/BinaryFileReaderWriter.h"
typedef BYTE *LPBYTE;

namespace XLSB
{
    class CBinaryBiff12StreamReader: public NSBinPptxRW::CBinaryFileReader
    {
    public:
        CBinaryBiff12StreamReader();

        static DWORD GetDword(LPBYTE p);

        static WORD GetWord(LPBYTE p);

        static BYTE GetByte(LPBYTE p);

        static double GetFloat(LPBYTE p);

        static double GetDouble(LPBYTE p);

        static std::wstring GetString(LPBYTE p, int len);

        static bool GetRecordId(LPBYTE& p, LPBYTE p_last, /*out*/DWORD& recid);

        static bool GetRecordLength(LPBYTE& p, LPBYTE p_last, /*out*/DWORD& reclen);
        
        static bool GetBit(unsigned char byte, int position); // position in range 0-7
        
        static DWORD GetBits(unsigned x,int p,int n);
      
    };
}
#endif // CBINARYBIFF12STREAMREADER_H
