#ifndef _CMAP_WASM_SERIALIZE_H
#define _CMAP_WASM_SERIALIZE_H

#include "serialize.h"

static DWORD GetLength(BYTE* x)
{
    return x ? (x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24) : 4;
}

namespace NSWasm
{
void CMapDirToFile()
{
    CData oRes;
    oRes.SkipLen();


    oRes.WriteLen();
    BYTE* bRes = oRes.GetBuffer();
    oRes.ClearWithoutAttack();

    DWORD nLength = GetLength(bRes);
    DWORD i = 4;
    nLength -= 4;
    while (i < nLength)
    {

    }
}
}

#endif // _CMAP_WASM_SERIALIZE_H
