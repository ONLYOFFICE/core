#ifndef PUBLICRES_H
#define PUBLICRES_H

#include "../../OfficeUtils/src/ZipFolder.h"

namespace OFD
{
class CPublicRes
{
public:
	CPublicRes();

	bool Read(IFolder* pFolder);
};
}

#endif // PUBLICRES_H
