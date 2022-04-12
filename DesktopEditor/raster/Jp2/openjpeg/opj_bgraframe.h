#ifndef OO_DECOMPRESS_H
#define OO_DECOMPRESS_H

#include "../../BgraFrame.h"

namespace openjpeg
{
	bool GetData(BYTE* pFildData, const unsigned int& nFileSize, BYTE*& ppData, int& nComponentsCount, int& nWidth, int& nHeight, const bool& isFlip);
	void DestroyData(BYTE*& pData);
}

#endif
