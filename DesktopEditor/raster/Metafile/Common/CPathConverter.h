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
                CPathConverter(double dKoefX, double dKoefY);
                ~CPathConverter();

                void UpdateKoefs(double dKoefX, double dKoefY);

                void GetUpdatedPath(CEmfPath& oNewPath, CEmfPath& oLineCapPath, const CEmfPath& oPath, const CEmfPlusPen& oPen);

//                void GetLineCapPath(CEmfPath& oNewPath, const CEmfPath& oPath, const CEmfPlusPen& oPen);
        private:
                bool AddLineStartCap(CEmfPath& oPath, const CEmfPlusPen& oPen, double& dX, double& dY);
                bool AddLineEndCap(CEmfPath& oPath, const CEmfPlusPen& oPen, double& dX, double& dY);
                bool AddLineCap(CEmfPath& oPath, const CEmfPath& oLineCapPath, double& dX, double& dY, double dAngle);

                double m_dKoefX;
                double m_dKoefY;
        };
}
#endif // CPATHCONVERTER_H
