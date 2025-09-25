#include "CPathConverter.h"

namespace MetaFile
{
	CPathConverter::CPathConverter()
	{}

	CPathConverter::~CPathConverter()
	{}

	void CPathConverter::GetUpdatedPath(CPath &oNewPath, CPath &oLineCapPath, const CPath &oPath, const CEmfPlusPen &oPen)
	{
		TEmfPlusPointF oLastPoint(0, 0);

		for (const CPathCommandBase* pCommand : oPath.GetCommands())
		{
			switch (pCommand->GetType())
			{
				case PATH_COMMAND_MOVETO:
				{
					CPathCommandMoveTo *pCommandMoveTo = (CPathCommandMoveTo*)pCommand;
					oNewPath.MoveTo(pCommandMoveTo->GetX(), pCommandMoveTo->GetY());

					oLastPoint.X = pCommandMoveTo->GetX();
					oLastPoint.Y = pCommandMoveTo->GetY();

					break;
				}
				case PATH_COMMAND_LINETO:
				{
					CPathCommandLineTo *pCommandLineTo = (CPathCommandLineTo*)pCommand;

					double dAngle = -90 + (atan((pCommandLineTo->GetY() - oLastPoint.Y) /
					                            (pCommandLineTo->GetX() - oLastPoint.X)) * 180 / 3.14);

					if (AddLineStartCap(oLineCapPath, oPen, oLastPoint.X, oLastPoint.Y, dAngle + 180))
						oNewPath.MoveTo(oLastPoint.X, oLastPoint.Y);

					oLastPoint.X = pCommandLineTo->GetX();
					oLastPoint.Y = pCommandLineTo->GetY();

					AddLineEndCap(oLineCapPath, oPen, oLastPoint.X, oLastPoint.Y, dAngle);

					oNewPath.LineTo(oLastPoint.X,  oLastPoint.Y);

					break;
				}
				case PATH_COMMAND_CURVETO:
				{
					CPathCommandCurveTo *pCommandCurveTo = (CPathCommandCurveTo*)pCommand;

					oNewPath.CurveTo(pCommandCurveTo->GetX1(), pCommandCurveTo->GetY1(),
					                 pCommandCurveTo->GetX2(), pCommandCurveTo->GetY2(),
					                 pCommandCurveTo->GetXE(), pCommandCurveTo->GetYE());

					break;
				}
				case PATH_COMMAND_ARCTO:
				{
					CPathCommandArcTo *pCommandArcTo = (CPathCommandArcTo*)pCommand;

					oNewPath.ArcTo(pCommandArcTo->GetLeft(),       pCommandArcTo->GetTop(),
					               pCommandArcTo->GetRight(),      pCommandArcTo->GetBottom(),
					               pCommandArcTo->GetStartAngle(), pCommandArcTo->GetSweepAngle());

					break;
				}
				case PATH_COMMAND_CLOSE:
				{
					oNewPath.Close();
					break;
				}
				case PATH_COMMAND_UNKNOWN: break;
			}
		}

	}

	bool CPathConverter::AddLineStartCap(CPath &oPath, const CEmfPlusPen &oPen, double& dX, double& dY, double dAngle)
	{
		if (NULL != oPen.pLineStartCapData)
		{
			if (CustomLineCapDataTypeDefault == oPen.pLineStartCapData->GetType() && NULL != ((TEmfPlusCustomLineCapData*)oPen.pLineStartCapData)->pPath)
			{
				return AddLineCap(oPath, *(((TEmfPlusCustomLineCapData*)oPen.pLineStartCapData)->pPath), dX, dY, dAngle, oPen.dWidth);
			}
		}

		return false;
	}

	bool CPathConverter::AddLineEndCap(CPath &oPath, const CEmfPlusPen &oPen, double &dX, double &dY, double dAngle)
	{
		if (NULL != oPen.pLineEndCapData)
		{
			if (CustomLineCapDataTypeDefault == oPen.pLineEndCapData->GetType() && NULL != ((TEmfPlusCustomLineCapData*)oPen.pLineEndCapData)->pPath)
			{
				return AddLineCap(oPath, *((TEmfPlusCustomLineCapData*)oPen.pLineEndCapData)->pPath, dX, dY, dAngle, oPen.dWidth);
			}
		}

		return false;
	}

	bool CPathConverter::AddLineCap(CPath &oPath, const CPath &oLineCapPath, double &dX, double &dY, double dAngle, double dPenWidth)
	{
		double dNewX = 0;
		double dNewY = 0;

		double dSin = sin(dAngle * 3.14 / 180);
		double dCos = cos(dAngle * 3.14 / 180);

		const std::vector<CPathCommandBase*> arCommands(oLineCapPath.GetCommands());
		
		for (const CPathCommandBase* pCommand : arCommands)
		{
			switch (pCommand->GetType())
			{
			case PATH_COMMAND_MOVETO:
			{
				CPathCommandMoveTo *pCommandMoveTo = (CPathCommandMoveTo*)pCommand;

				double dTempX = pCommandMoveTo->GetX() * dPenWidth * dCos - pCommandMoveTo->GetY() * dPenWidth * dSin;
				double dTempY = pCommandMoveTo->GetX() * dPenWidth * dSin + pCommandMoveTo->GetY() * dPenWidth * dCos;

				dNewX += dTempX;
				dNewY += dTempY;

				oPath.MoveTo(dX + dTempX, dY + dTempY);
				break;
			}
			case PATH_COMMAND_LINETO:
			{
				CPathCommandLineTo *pCommandLineTo = (CPathCommandLineTo*)pCommand;

				double dTempX = pCommandLineTo->GetX() * dPenWidth * dCos - pCommandLineTo->GetY() * dPenWidth * dSin;
				double dTempY = pCommandLineTo->GetX() * dPenWidth * dSin + pCommandLineTo->GetY() * dPenWidth * dCos;

				dNewX += dTempX;
				dNewY += dTempY;

				oPath.LineTo(dX + dTempX, dY + dTempY);
				break;
			}
			case PATH_COMMAND_CURVETO:
			{
				break;
			}
			case PATH_COMMAND_ARCTO:
			{
				break;
			}
			case PATH_COMMAND_CLOSE:
			{
				oPath.Close();
				break;
			}
			case PATH_COMMAND_UNKNOWN: break;
			}
		}

		if (arCommands.size() > 1)
		{
			dX = dX + dNewX / (arCommands.size() - 1);
			dY = dY + dNewY / (arCommands.size() - 1);

			return true;
		}

		return false;
	}
}
