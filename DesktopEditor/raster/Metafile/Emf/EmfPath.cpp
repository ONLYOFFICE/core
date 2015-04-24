#include "EmfPath.h"
#include "EmfOutputDevice.h"

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
	bool CEmfPath::MoveTo(TEmfPointS& oPoint)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathMoveTo(oPoint);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::MoveTo(TEmfPointL& oPoint)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathMoveTo(oPoint);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::MoveTo(int lX, int lY)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathMoveTo(lX, lY);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::LineTo(TEmfPointS& oPoint)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathLineTo(oPoint);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::LineTo(TEmfPointL& oPoint)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathLineTo(oPoint);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::LineTo(int lX, int lY)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathLineTo(lX, lY);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::CurveTo(TEmfPointS& oPoint1, TEmfPointS& oPoint2, TEmfPointS& oPointE)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathCurveTo(oPoint1, oPoint2, oPointE);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::CurveTo(TEmfPointL& oPoint1, TEmfPointL& oPoint2, TEmfPointL& oPointE)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathCurveTo(oPoint1, oPoint2, oPointE);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::CurveTo(int lX1, int lY1, int lX2, int lY2, int lXE, int lYE)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathCurveTo(lX1, lY1, lX2, lY2, lXE, lYE);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::ArcTo(int lL, int lT, int lR, int lB, double dStart, double dSweep)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathArcTo(lL, lT, lR, lB, dStart, dSweep);
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
	void CEmfPath::Draw(CEmfOutputDevice* pOutput, bool bStroke, bool bFill, unsigned int unClipMode)
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