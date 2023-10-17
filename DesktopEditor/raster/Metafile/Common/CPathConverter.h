#ifndef CPATHCONVERTER_H
#define CPATHCONVERTER_H

#include "CPath.h"
#include "../Emf/EmfPlusObjects.h"

namespace MetaFile
{
	class CPathConverter
	{
	public:
		CPathConverter();
		virtual ~CPathConverter();

		void GetUpdatedPath(CPath& oNewPath, CPath& oLineCapPath, const CPath& oPath, const CEmfPlusPen& oPen);
	private:
		bool AddLineStartCap(CPath& oPath, const CEmfPlusPen& oPen, double& dX, double& dY, double dAngle);
		bool AddLineEndCap(CPath& oPath, const CEmfPlusPen& oPen, double& dX, double& dY, double dAngle);
		bool AddLineCap(CPath& oPath, const CPath& oLineCapPath, double& dX, double& dY, double dAngle, double dPenWidth);
	};
}
#endif // CPATHCONVERTER_H
