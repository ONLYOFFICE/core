#ifndef CPATH_H
#define CPATH_H

#include "IOutputDevice.h"

namespace MetaFile
{
	typedef enum
	{
		PATH_COMMAND_UNKNOWN,
		PATH_COMMAND_MOVETO,
		PATH_COMMAND_LINETO,
		PATH_COMMAND_CURVETO,
		PATH_COMMAND_ARCTO,
		PATH_COMMAND_CLOSE
	} EPathCommandType;

	class CPathCommandBase
	{
	public:
		CPathCommandBase();
		virtual ~CPathCommandBase();
		virtual EPathCommandType GetType() const;
	};
	
	class CPathCommandMoveTo : public CPathCommandBase
	{
	public:
		CPathCommandMoveTo(double dX, double dY);
		CPathCommandMoveTo(const CPathCommandMoveTo& oCommandMoveTo);
		virtual ~CPathCommandMoveTo();
		virtual EPathCommandType GetType() const override;

		double GetX() const;
		double GetY() const;
	private:
		double m_dX;
		double m_dY;
	};

	class CPathCommandLineTo : public CPathCommandBase
	{
	public:
		CPathCommandLineTo(double dX, double dY);
		CPathCommandLineTo(const CPathCommandLineTo& oCommandLineTo);
		virtual ~CPathCommandLineTo();
		virtual EPathCommandType GetType() const override;

		double GetX() const;
		double GetY() const;
	private:
		double m_dX;
		double m_dY;
	};

	class CPathCommandCurveTo : public CPathCommandBase
	{
	public:
		CPathCommandCurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE);
		CPathCommandCurveTo(const CPathCommandCurveTo& oCommandCurveTo);
		virtual ~CPathCommandCurveTo();
		virtual EPathCommandType GetType() const override;

		double GetX1() const;
		double GetY1() const;
		double GetX2() const;
		double GetY2() const;
		double GetXE() const;
		double GetYE() const;
	private:
		double m_dX1;
		double m_dY1;
		double m_dX2;
		double m_dY2;
		double m_dXE;
		double m_dYE;
	};

	class CPathCommandArcTo : public CPathCommandBase
	{
	public:
		CPathCommandArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle);
		CPathCommandArcTo(const CPathCommandArcTo& oCommandArcTo);
		virtual ~CPathCommandArcTo();
		virtual EPathCommandType GetType() const override;

		double GetLeft()       const;
		double GetTop()        const;
		double GetRight()      const;
		double GetBottom()     const;
		double GetStartAngle() const;
		double GetSweepAngle() const;
	private:
		double m_dLeft;
		double m_dTop;
		double m_dRight;
		double m_dBottom;
		double m_dStartAngle;
		double m_dSweepAngle;
	};

	class CPathCommandClose : public CPathCommandBase
	{
	public:
		CPathCommandClose();
		virtual ~CPathCommandClose();
		virtual EPathCommandType GetType() const override;
	};

	class CPath
	{
	public:
		CPath();
		CPath(const CPath& oPath);
		~CPath();

		bool Empty() const;

		bool MoveTo(double dX, double dY);
		bool LineTo(double dX, double dY);
		bool CurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE);
		bool ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle);
		bool Close();

		void DrawOn(IOutputDevice* pOutput, bool bStroke, bool bFill, int nClipMode = -1) const;
		void Clear();
		void Copy(const CPath& oPath);
		
		const std::vector<CPathCommandBase*> GetCommands() const;
	private:
		bool AddCommand(CPathCommandBase* pCommand);
		std::vector<CPathCommandBase*> m_arCommands;
	};
}

#endif // CPATH_H
