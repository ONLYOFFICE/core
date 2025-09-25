#include "CPath.h"

namespace MetaFile
{
	CPathCommandBase::CPathCommandBase()
	{}

	CPathCommandBase::~CPathCommandBase()
	{}

	EPathCommandType CPathCommandBase::GetType() const
	{
		return PATH_COMMAND_UNKNOWN;
	}

	CPathCommandMoveTo::CPathCommandMoveTo(double dX, double dY)
		: m_dX(dX), m_dY(dY)
	{}

	CPathCommandMoveTo::CPathCommandMoveTo(const CPathCommandMoveTo &oCommandMoveTo)
		: m_dX(oCommandMoveTo.m_dX), m_dY(oCommandMoveTo.m_dY)
	{}

	CPathCommandMoveTo::~CPathCommandMoveTo()
	{}

	EPathCommandType CPathCommandMoveTo::GetType() const
	{
		return PATH_COMMAND_MOVETO;
	}

	double CPathCommandMoveTo::GetX() const
	{
		return m_dX;
	}

	double CPathCommandMoveTo::GetY() const
	{
		return m_dY;
	}

	CPathCommandLineTo::CPathCommandLineTo(double dX, double dY)
		: m_dX(dX), m_dY(dY)
	{}

	CPathCommandLineTo::CPathCommandLineTo(const CPathCommandLineTo &oCommandLineTo)
		: m_dX(oCommandLineTo.m_dX), m_dY(oCommandLineTo.m_dY)
	{}

	CPathCommandLineTo::~CPathCommandLineTo()
	{}

	EPathCommandType CPathCommandLineTo::GetType() const
	{
		return PATH_COMMAND_LINETO;
	}

	double CPathCommandLineTo::GetX() const
	{
		return m_dX;
	}

	double CPathCommandLineTo::GetY() const
	{
		return m_dY;
	}

	CPathCommandCurveTo::CPathCommandCurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE)
		: m_dX1(dX1), m_dY1(dY1), m_dX2(dX2), m_dY2(dY2), m_dXE(dXE), m_dYE(dYE)
	{}

	CPathCommandCurveTo::CPathCommandCurveTo(const CPathCommandCurveTo &oCommandCurveTo)
		: m_dX1(oCommandCurveTo.m_dX1), m_dY1(oCommandCurveTo.m_dY1), 
		  m_dX2(oCommandCurveTo.m_dX2), m_dY2(oCommandCurveTo.m_dY2),
		  m_dXE(oCommandCurveTo.m_dXE), m_dYE(oCommandCurveTo.m_dYE)
	{}

	CPathCommandCurveTo::~CPathCommandCurveTo()
	{}

	EPathCommandType CPathCommandCurveTo::GetType() const
	{
		return PATH_COMMAND_CURVETO;
	}

	double CPathCommandCurveTo::GetX1() const
	{
		return m_dX1;
	}

	double CPathCommandCurveTo::GetY1() const
	{
		return m_dY1;
	}

	double CPathCommandCurveTo::GetX2() const
	{
		return m_dX2;
	}

	double CPathCommandCurveTo::GetY2() const
	{
		return m_dY2;
	}

	double CPathCommandCurveTo::GetXE() const
	{
		return m_dXE;
	}

	double CPathCommandCurveTo::GetYE() const
	{
		return m_dYE;
	}

	CPathCommandArcTo::CPathCommandArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
		: m_dLeft(dLeft), m_dTop(dTop), m_dRight(dRight), m_dBottom(dBottom), m_dStartAngle(dStartAngle), m_dSweepAngle(dSweepAngle)
	{}

	CPathCommandArcTo::CPathCommandArcTo(const CPathCommandArcTo &oCommandArcTo)
		: m_dLeft(oCommandArcTo.m_dLeft),             m_dTop(oCommandArcTo.m_dTop),
		  m_dRight(oCommandArcTo.m_dRight),           m_dBottom(oCommandArcTo.m_dBottom),
		  m_dStartAngle(oCommandArcTo.m_dStartAngle), m_dSweepAngle(oCommandArcTo.m_dSweepAngle)
	{}

	CPathCommandArcTo::~CPathCommandArcTo()
	{}

	EPathCommandType CPathCommandArcTo::GetType() const
	{
		return PATH_COMMAND_ARCTO;
	}

	double CPathCommandArcTo::GetLeft() const
	{
		return m_dLeft;
	}

	double CPathCommandArcTo::GetTop() const
	{
		return m_dTop;
	}

	double CPathCommandArcTo::GetRight() const
	{
		return m_dRight;
	}

	double CPathCommandArcTo::GetBottom() const
	{
		return m_dBottom;
	}

	double CPathCommandArcTo::GetStartAngle() const
	{
		return m_dStartAngle;
	}

	double CPathCommandArcTo::GetSweepAngle() const
	{
		return m_dSweepAngle;
	}

	CPathCommandClose::CPathCommandClose()
	{}

	CPathCommandClose::~CPathCommandClose()
	{}

	EPathCommandType CPathCommandClose::GetType() const
	{
		return PATH_COMMAND_CLOSE;
	}

	CPath::CPath()
	{}

	CPath::CPath(const CPath &oPath)
	{
		Copy(oPath);
	}

	CPath::~CPath()
	{
		Clear();
	}
	
	bool CPath::Empty() const
	{
		return m_arCommands.empty();
	}

	bool CPath::MoveTo(double dX, double dY)
	{
		return AddCommand(new CPathCommandMoveTo(dX, dY));
	}

	bool CPath::LineTo(double dX, double dY)
	{
		return AddCommand(new CPathCommandLineTo(dX, dY));
	}

	bool CPath::CurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE)
	{
		return AddCommand(new CPathCommandCurveTo(dX1, dY1, dX2, dY2, dXE, dYE));
	}

	bool CPath::ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
	{
		return AddCommand(new CPathCommandArcTo(dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle));
	}

	bool CPath::Close()
	{
		return AddCommand(new CPathCommandClose());
	}

	void CPath::DrawOn(IOutputDevice *pOutput, bool bStroke, bool bFill, int nClipMode) const
	{
		if (NULL == pOutput)
			return;

		if (-1 == nClipMode && (bStroke || bFill))
			pOutput->StartPath();
		else if (-1 != nClipMode)
			pOutput->StartClipPath(nClipMode);
		
		for (const CPathCommandBase* pCommand : m_arCommands)
		{
			switch(pCommand->GetType())
			{
			case PATH_COMMAND_MOVETO:
			{
				CPathCommandMoveTo *pMoveTo = (CPathCommandMoveTo*)pCommand;
				pOutput->MoveTo(pMoveTo->GetX(), pMoveTo->GetY());
				break;
			}
			case PATH_COMMAND_LINETO:
			{
				CPathCommandLineTo *pLineTo = (CPathCommandLineTo*)pCommand;
				pOutput->LineTo(pLineTo->GetX(), pLineTo->GetY());
				break;
			}
			case PATH_COMMAND_CURVETO:
			{
				CPathCommandCurveTo *pCurveTo = (CPathCommandCurveTo*)pCommand;
				pOutput->CurveTo(pCurveTo->GetX1(), pCurveTo->GetY1(), pCurveTo->GetX2(), pCurveTo->GetY2(), pCurveTo->GetXE(), pCurveTo->GetYE());
				break;
			}
			case PATH_COMMAND_ARCTO:
			{
				CPathCommandArcTo *pAcrTo = (CPathCommandArcTo*)pCommand;
				pOutput->ArcTo(pAcrTo->GetLeft(), pAcrTo->GetTop(), pAcrTo->GetRight(), pAcrTo->GetBottom(), pAcrTo->GetStartAngle(), pAcrTo->GetSweepAngle());
				break;
			}
			case PATH_COMMAND_CLOSE:
			{
				pOutput->ClosePath();
				break;
			}
			case PATH_COMMAND_UNKNOWN:
			break;
			}
		}

		if (-1 == nClipMode && (bStroke || bFill))
		{
			pOutput->DrawPath((bStroke ? 1 : 0) + (bFill ? 2 : 0));
			pOutput->EndPath();
		}
		else if (-1 != nClipMode)
			pOutput->EndClipPath(nClipMode);
	}
	
	void CPath::Clear()
	{
		for (CPathCommandBase* pCommand : m_arCommands)
			delete pCommand;
		
		m_arCommands.clear();
	}

	void CPath::Copy(const CPath &oPath)
	{
		for (const CPathCommandBase* pCommand : oPath.m_arCommands)
		{
			switch(pCommand->GetType())
			{
			case PATH_COMMAND_MOVETO:
			{
				AddCommand(new CPathCommandMoveTo(*(CPathCommandMoveTo*)pCommand));
				break;
			}
			case PATH_COMMAND_LINETO:
			{
				AddCommand(new CPathCommandLineTo(*(CPathCommandLineTo*)pCommand));
				break;
			}
			case PATH_COMMAND_CURVETO:
			{
				AddCommand(new CPathCommandCurveTo(*(CPathCommandCurveTo*)pCommand));
				break;
			}
			case PATH_COMMAND_ARCTO:
			{
				AddCommand(new CPathCommandArcTo(*(CPathCommandArcTo*)pCommand));
				break;
			}
			case PATH_COMMAND_CLOSE:
			{
				AddCommand(new CPathCommandClose());
				break;
			}
			case PATH_COMMAND_UNKNOWN: 
			break;
			}
		}
	}
	
	const std::vector<CPathCommandBase *> CPath::GetCommands() const
	{
		return m_arCommands;
	}

	bool CPath::AddCommand(CPathCommandBase *pCommand)
	{
		if (NULL == pCommand)
			return false;
		
		if (PATH_COMMAND_MOVETO == pCommand->GetType() && !m_arCommands.empty() && PATH_COMMAND_MOVETO == m_arCommands.back()->GetType())
		{
			CPathCommandBase* pLastCommand = m_arCommands.back();
			m_arCommands.pop_back();
			delete pLastCommand;
		}

		m_arCommands.push_back(pCommand);
		return true;
	}
}
