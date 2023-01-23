#ifndef CPATHCONVERTER_H
#define CPATHCONVERTER_H

#include "../Emf/EmfPath.h"
#include "../Emf/EmfPlusObjects.h"

namespace MetaFile
{
	class CPathConverter
	{
	public:
		CPathConverter();
		virtual ~CPathConverter();


		void GetUpdatedPath(CEmfPath& oNewPath, CEmfPath& oLineCapPath, const CEmfPath& oPath, const CEmfPlusPen& oPen);

		//                void GetLineCapPath(CEmfPath& oNewPath, const CEmfPath& oPath, const CEmfPlusPen& oPen);
	private:
		bool AddLineStartCap(CEmfPath& oPath, const CEmfPlusPen& oPen, double& dX, double& dY, double dAngle);
		bool AddLineEndCap(CEmfPath& oPath, const CEmfPlusPen& oPen, double& dX, double& dY, double dAngle);
		bool AddLineCap(CEmfPath& oPath, const CEmfPath& oLineCapPath, double& dX, double& dY, double dAngle, double dPenWidth);
	};
}
#endif // CPATHCONVERTER_H
