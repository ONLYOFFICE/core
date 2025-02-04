#ifndef SECTOR_H
#define SECTOR_H

#include "SectorType.h"

namespace HWP
{
struct TSector
{
	ESectorType m_eType;
	int m_nSectorNum;
	int m_nNextNum;
};
}

#endif // SECTOR_H
