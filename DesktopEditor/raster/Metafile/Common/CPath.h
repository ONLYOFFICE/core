/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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
