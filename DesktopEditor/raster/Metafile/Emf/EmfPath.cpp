#include "EmfPath.h"

namespace MetaFile
{
	CEmfPath::CEmfPath()
	{
	}
	CEmfPath::CEmfPath(CEmfPath* pPath)
	{
		if (pPath)
		{
			for (unsigned int unIndex = 0; unIndex < pPath->m_pCommands.size(); unIndex++)
			{
				CEmfPathCommandBase* pCommand = pPath->m_pCommands.at(unIndex);
				switch (pCommand->GetType())
				{
					case EMF_PATHCOMMAND_MOVETO:
					{
						CEmfPathMoveTo* pMoveTo = (CEmfPathMoveTo*)pCommand;
						MoveTo(pMoveTo->x, pMoveTo->y);
						break;
					}
					case EMF_PATHCOMMAND_LINETO:
					{
						CEmfPathLineTo* pLineTo = (CEmfPathLineTo*)pCommand;
						LineTo(pLineTo->x, pLineTo->y);
						break;
					}
					case EMF_PATHCOMMAND_CURVETO:
					{
						CEmfPathCurveTo* pCurveTo = (CEmfPathCurveTo*)pCommand;
						CurveTo(pCurveTo->x1, pCurveTo->y1, pCurveTo->x2, pCurveTo->y2, pCurveTo->xE, pCurveTo->yE);
						break;
					}
					case EMF_PATHCOMMAND_ARCTO:
					{
						CEmfPathArcTo* pArcTo = (CEmfPathArcTo*)pCommand;
						ArcTo(pArcTo->left, pArcTo->top, pArcTo->right, pArcTo->bottom, pArcTo->start, pArcTo->sweep);
						break;
					}
					case EMF_PATHCOMMAND_CLOSE:
					{
						Close();
						break;
					}
				}
			}
		}
	}
	CEmfPath::~CEmfPath()
	{
		Clear();
	}	
	bool CEmfPath::MoveTo(double dX, double dY)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathMoveTo(dX, dY);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}	
	bool CEmfPath::LineTo(double dX, double dY)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathLineTo(dX, dY);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}	
	bool CEmfPath::CurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathCurveTo(dX1, dY1, dX2, dY2, dXE, dYE);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::ArcTo(double dL, double dT, double dR, double dB, double dStart, double dSweep)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathArcTo(dL, dT, dR, dB, dStart, dSweep);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::Close()
	{
		CEmfPathCommandBase* pCommand = new CEmfPathClose();
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	void CEmfPath::Draw(IOutputDevice* pOutput, bool bStroke, bool bFill, unsigned int unClipMode)
	{
		if (pOutput)
		{	
			if (-1 != unClipMode)
				pOutput->StartClipPath(unClipMode);
			else
				pOutput->StartPath();

			for (unsigned int ulIndex = 0; ulIndex < m_pCommands.size(); ulIndex++)
			{
				CEmfPathCommandBase* pCommand = m_pCommands.at(ulIndex);
				switch (pCommand->GetType())
				{
					case EMF_PATHCOMMAND_MOVETO:
					{
						CEmfPathMoveTo* pMoveTo = (CEmfPathMoveTo*)pCommand;
						pOutput->MoveTo(pMoveTo->x, pMoveTo->y);
						break;
					}
					case EMF_PATHCOMMAND_LINETO:
					{
						CEmfPathLineTo* pLineTo = (CEmfPathLineTo*)pCommand;
						pOutput->LineTo(pLineTo->x, pLineTo->y);
						break;
					}
					case EMF_PATHCOMMAND_CURVETO:
					{
						CEmfPathCurveTo* pCurveTo = (CEmfPathCurveTo*)pCommand;
						pOutput->CurveTo(pCurveTo->x1, pCurveTo->y1, pCurveTo->x2, pCurveTo->y2, pCurveTo->xE, pCurveTo->yE);
						break;
					}
					case EMF_PATHCOMMAND_ARCTO:
					{
						CEmfPathArcTo* pArcTo = (CEmfPathArcTo*)pCommand;
						pOutput->ArcTo(pArcTo->left, pArcTo->top, pArcTo->right, pArcTo->bottom, pArcTo->start, pArcTo->sweep);
						break;
					}
					case EMF_PATHCOMMAND_CLOSE:
					{
						pOutput->ClosePath();
						break;
					}
				}
			}

			if (-1 == unClipMode)
			{
				int lType = (bStroke ? 1 : 0) + (bFill ? 2 : 0);
				pOutput->DrawPath(lType);
				pOutput->EndPath();
			}
			else
				pOutput->EndClipPath(unClipMode);
		}

		// При клипе пат не очищаем
		if (-1 == unClipMode)
			Clear();
	}
	void CEmfPath::Clear()
	{
		for (unsigned int ulIndex = 0; ulIndex < m_pCommands.size(); ulIndex++)
		{
			CEmfPathCommandBase* pCommand = m_pCommands.at(ulIndex);
			delete pCommand;
		}
		m_pCommands.clear();
	}
}