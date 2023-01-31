#include "CPathConverter.h"

namespace MetaFile
{
	CPathConverter::CPathConverter()
	{}

	CPathConverter::~CPathConverter()
	{}

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

					double dAngle = -90 + (atan((pCommandLineTo->y - oLastPoint.Y) /
					                            (pCommandLineTo->x - oLastPoint.X)) * 180 / 3.14);

					if (AddLineStartCap(oLineCapPath, oPen, oLastPoint.X, oLastPoint.Y, dAngle + 180))
						oNewPath.MoveTo(oLastPoint.X, oLastPoint.Y);

					oLastPoint.X = pCommandLineTo->x;
					oLastPoint.Y = pCommandLineTo->y;

					AddLineEndCap(oLineCapPath, oPen, oLastPoint.X, oLastPoint.Y, dAngle);

					oNewPath.LineTo(oLastPoint.X,  oLastPoint.Y);

					break;
				}
				case EMF_PATHCOMMAND_CURVETO:
				{
					CEmfPathCurveTo *pCommandCurveTo = (CEmfPathCurveTo*)pCommand;

					oNewPath.CurveTo(pCommandCurveTo->x1, pCommandCurveTo->y1,
					                 pCommandCurveTo->x2, pCommandCurveTo->y2,
					                 pCommandCurveTo->xE, pCommandCurveTo->yE);

					break;
				}
				case EMF_PATHCOMMAND_ARCTO:
				{
					CEmfPathArcTo *pCommandArcTo = (CEmfPathArcTo*)pCommand;

					oNewPath.ArcTo(pCommandArcTo->left,  pCommandArcTo->top,
					               pCommandArcTo->right, pCommandArcTo->bottom,
					               pCommandArcTo->start, pCommandArcTo->sweep);

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

	bool CPathConverter::AddLineStartCap(CEmfPath &oPath, const CEmfPlusPen &oPen, double& dX, double& dY, double dAngle)
	{
		if (NULL != oPen.LineStartCapData)
		{
			if (CustomLineCapDataTypeDefault == oPen.LineStartCapData->GetType() && NULL != ((TEmfPlusCustomLineCapData*)oPen.LineStartCapData)->pPath)
			{
				return AddLineCap(oPath, ((TEmfPlusCustomLineCapData*)oPen.LineStartCapData)->pPath, dX, dY, dAngle, oPen.Width);
			}
		}

		return false;
	}

	bool CPathConverter::AddLineEndCap(CEmfPath &oPath, const CEmfPlusPen &oPen, double &dX, double &dY, double dAngle)
	{
		if (NULL != oPen.LineEndCapData)
		{
			if (CustomLineCapDataTypeDefault == oPen.LineEndCapData->GetType() && NULL != ((TEmfPlusCustomLineCapData*)oPen.LineEndCapData)->pPath)
			{
				return AddLineCap(oPath, ((TEmfPlusCustomLineCapData*)oPen.LineEndCapData)->pPath, dX, dY, dAngle, oPen.Width);
			}
		}

		return false;
	}

	bool CPathConverter::AddLineCap(CEmfPath &oPath, const CEmfPath &oLineCapPath, double &dX, double &dY, double dAngle, double dPenWidth)
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

				double dTempX = pCommandMoveTo->x * dPenWidth * dCos - pCommandMoveTo->y * dPenWidth * dSin;
				double dTempY = pCommandMoveTo->x * dPenWidth * dSin + pCommandMoveTo->y * dPenWidth * dCos;

				dNewX += dTempX;
				dNewY += dTempY;

				oPath.MoveTo(dX + dTempX, dY + dTempY);
				break;
			}
			case EMF_PATHCOMMAND_LINETO:
			{
				CEmfPathLineTo *pCommandLineTo = (CEmfPathLineTo*)pCommand;

				double dTempX = pCommandLineTo->x * dPenWidth * dCos - pCommandLineTo->y * dPenWidth * dSin;
				double dTempY = pCommandLineTo->x * dPenWidth * dSin + pCommandLineTo->y * dPenWidth * dCos;

				dNewX += dTempX;
				dNewY += dTempY;

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

		if (oPath.m_pCommands.size() > 1)
		{
			dX = dX + dNewX / (oPath.m_pCommands.size() - 1);
			dY = dY + dNewY / (oPath.m_pCommands.size() - 1);

			return true;
		}

		return false;
	}
}
