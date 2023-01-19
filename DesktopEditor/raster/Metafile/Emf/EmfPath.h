/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#ifndef _METAFILE_EMF_EMFPATH_H
#define _METAFILE_EMF_EMFPATH_H

#include "../Common/MetaFileObjects.h"
#include "EmfTypes.h"
#include <vector>

namespace MetaFile
{
	typedef enum
	{
		EMF_PATHCOMMAND_UNKNOWN = 0x00,
		EMF_PATHCOMMAND_MOVETO  = 0x01,
		EMF_PATHCOMMAND_LINETO  = 0x02,
		EMF_PATHCOMMAND_CURVETO = 0x03,
		EMF_PATHCOMMAND_ARCTO   = 0x04,
		EMF_PATHCOMMAND_CLOSE   = 0x05
	} EEmfPathCommandType;

	class CEmfPathCommandBase
	{
	public:
		CEmfPathCommandBase()
		{
		}
		virtual ~CEmfPathCommandBase()
		{
		}
		virtual EEmfPathCommandType GetType() const = 0;
	};
	class CEmfPathMoveTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathMoveTo(double dX, double dY)
		{
			x = dX;
			y = dY;
		}
		virtual ~CEmfPathMoveTo()
		{
		}
		virtual EEmfPathCommandType GetType() const override
		{
			return EMF_PATHCOMMAND_MOVETO;
		}

	public:
		
		double x;
		double y;
	};
	class CEmfPathLineTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathLineTo(double dX, double dY)
		{
			x = dX;
			y = dY;
		}
		virtual ~CEmfPathLineTo()
		{
		}
		virtual EEmfPathCommandType GetType() const override
		{
			return EMF_PATHCOMMAND_LINETO;
		}

	public:

		double x;
		double y;
	};
	class CEmfPathCurveTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathCurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE)
		{
			x1 = dX1;
			y1 = dY1;
			x2 = dX2;
			y2 = dY2;
			xE = dXE;
			yE = dYE;
		}
		virtual ~CEmfPathCurveTo()
		{
		}
		virtual EEmfPathCommandType GetType() const override
		{
			return EMF_PATHCOMMAND_CURVETO;
		}

	public:

		double x1;
		double y1;
		double x2;
		double y2;
		double xE;
		double yE;
	};
	class CEmfPathArcTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathArcTo(double dL, double dT, double dR, double dB, double dStart, double dSweep)
		{
			left   = dL;
			top    = dT;
			right  = dR;
			bottom = dB;
			start  = dStart;
			sweep  = dSweep;
		}
		virtual ~CEmfPathArcTo()
		{
		}
		virtual EEmfPathCommandType GetType() const override
		{
			return EMF_PATHCOMMAND_ARCTO;
		}

	public:

		double left;
		double top;
		double right;
		double bottom;
		double start;
		double sweep;
	};
	class CEmfPathClose : public CEmfPathCommandBase
	{
	public:
		CEmfPathClose()
		{
		}
		virtual ~CEmfPathClose()
		{
		}
		virtual EEmfPathCommandType GetType() const override
		{
			return EMF_PATHCOMMAND_CLOSE;
		}

	public:
	};

	class CEmfPath : public IPath
	{
	public:

		CEmfPath();
		CEmfPath(CEmfPath* pPath);
		~CEmfPath();

		bool MoveTo(double dX, double dY);
		bool LineTo(double dX, double dY);
		bool CurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE);
		bool ArcTo(double dL, double dT, double dR, double dB, double dStart, double dSweep);
		bool Close();
		void Draw(IOutputDevice* pOutput, bool bStroke, bool bFill, int nClipMode = -1) override;
		void DrawWithoutClean(IOutputDevice* pOutput, bool bStroke, bool bFill);
	protected:

		void Clear();

	public:

		std::vector<CEmfPathCommandBase*> m_pCommands;
	};
}

#endif //_METAFILE_EMF_EMFPATH_H
