#include "CPathConverter.h"

namespace MetaFile
{
        CPathConverter::CPathConverter() : m_dKoefX(1), m_dKoefY(1)
        {}

        CPathConverter::CPathConverter(double dKoefX, double dKoefY) : m_dKoefX(dKoefX), m_dKoefY(dKoefY)
        {}

        CPathConverter::~CPathConverter()
        {}

        void CPathConverter::UpdateKoefs(double dKoefX, double dKoefY)
        {
               m_dKoefX = dKoefX;
               m_dKoefY = dKoefY;
        }

        void CPathConverter::GetUpdatedPath(CEmfPath &oNewPath, CEmfPath &oLineCapPath, const CEmfPath &oPath, const CEmfPlusPen &oPen)
        {
                TEmfPlusPointF oLastPoint(0, 0);

                for (const CEmfPathCommandBase* pCommand : oPath.m_pCommands)
                {
                        switch (pCommand->GetType())
                        {
                                case EMF_PATHCOMMAND_UNKNOWN: break;
                                case EMF_PATHCOMMAND_MOVETO:
                                {
                                        CEmfPathMoveTo *pCommandMoveTo = (CEmfPathMoveTo*)pCommand;
                                        oNewPath.MoveTo(pCommandMoveTo->x, pCommandMoveTo->y);

                                        oLastPoint.X = pCommandMoveTo->x;
                                        oLastPoint.Y = pCommandMoveTo->y;

                                        break;
                                }
                                case EMF_PATHCOMMAND_LINETO:
                                {
                                        CEmfPathLineTo *pCommandLineTo = (CEmfPathLineTo*)pCommand;

                                        if (AddLineStartCap(oLineCapPath, oPen, oLastPoint.X, oLastPoint.Y))
                                                oNewPath.MoveTo(oLastPoint.X, oLastPoint.Y);

                                        oLastPoint.X = pCommandLineTo->x;
                                        oLastPoint.Y = pCommandLineTo->y;

                                        AddLineEndCap(oLineCapPath, oPen, oLastPoint.X, oLastPoint.Y);

                                        oNewPath.LineTo(oLastPoint.X, oLastPoint.Y);

                                        break;
                                }
                                case EMF_PATHCOMMAND_CURVETO:
                                {
                                        break;
                                }
                                case EMF_PATHCOMMAND_ARCTO:
                                {
                                        break;
                                }
                                case EMF_PATHCOMMAND_CLOSE:
                                {
                                        oNewPath.Close();
                                        break;
                                }
                        }
                }

        }

        bool CPathConverter::AddLineStartCap(CEmfPath &oPath, const CEmfPlusPen &oPen, double& dX, double& dY)
        {
                if (NULL != oPen.LineStartCapData)
                {
                        if (CustomLineCapDataTypeDefault == oPen.LineStartCapData->GetType() && NULL != ((TEmfPlusCustomLineCapData*)oPen.LineStartCapData)->pPath)
                        {
                                return AddLineCap(oPath, ((TEmfPlusCustomLineCapData*)oPen.LineStartCapData)->pPath, dX, dY, 180);
                        }
                }

                return false;
        }

        bool CPathConverter::AddLineEndCap(CEmfPath &oPath, const CEmfPlusPen &oPen, double &dX, double &dY)
        {
                if (NULL != oPen.LineEndCapData)
                {
                        if (CustomLineCapDataTypeDefault == oPen.LineEndCapData->GetType() && NULL != ((TEmfPlusCustomLineCapData*)oPen.LineEndCapData)->pPath)
                        {
                                return AddLineCap(oPath, ((TEmfPlusCustomLineCapData*)oPen.LineEndCapData)->pPath, dX, dY, 0);
                        }
                }

                return false;
        }

        bool CPathConverter::AddLineCap(CEmfPath &oPath, const CEmfPath &oLineCapPath, double &dX, double &dY, double dAngle)
        {
                double dNewX = 0;
                double dNewY = 0;

                double dSin = sin(dAngle * 3.14 / 180);
                double dCos = cos(dAngle * 3.14 / 180);

                for (const CEmfPathCommandBase* pCommand : oLineCapPath.m_pCommands)
                {
                        switch (pCommand->GetType())
                        {
                                case EMF_PATHCOMMAND_UNKNOWN: break;
                                case EMF_PATHCOMMAND_MOVETO:
                                {
                                        CEmfPathMoveTo *pCommandMoveTo = (CEmfPathMoveTo*)pCommand;

                                        double dTempX = pCommandMoveTo->x / m_dKoefX * dCos - pCommandMoveTo->y / m_dKoefY * dSin;
                                        double dTempY = pCommandMoveTo->x / m_dKoefY * dSin + pCommandMoveTo->y / m_dKoefY * dCos;

                                        dNewX += dX + dTempX;
                                        dNewY += dY + dTempY;

                                        oPath.MoveTo(dX + dTempX, dY + dTempY);
                                        break;
                                }
                                case EMF_PATHCOMMAND_LINETO:
                                {
                                        CEmfPathLineTo *pCommandLineTo = (CEmfPathLineTo*)pCommand;

                                        double dTempX = pCommandLineTo->x / m_dKoefX * dCos - pCommandLineTo->y / m_dKoefY * dSin;
                                        double dTempY = pCommandLineTo->x / m_dKoefY * dSin + pCommandLineTo->y / m_dKoefY * dCos;

                                        oPath.LineTo(dX + dTempX, dY + dTempY);
                                        break;
                                }
                                case EMF_PATHCOMMAND_CURVETO:
                                {
                                        break;
                                }
                                case EMF_PATHCOMMAND_ARCTO:
                                {
                                        break;
                                }
                                case EMF_PATHCOMMAND_CLOSE:
                                {
                                        oPath.Close();
                                        break;
                                }
                        }
                }

                if (!oPath.m_pCommands.empty())
                {
                        dX = dNewX;
                        dY = dNewY;

                        return true;
                }

                return false;
        }
}
